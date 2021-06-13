// MIT License
// 
// Copyright(c) 2019-2021 Filippos Gleglakos
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <AEON/Graphics/internal/Shape.h>

#include <AEON/Graphics/internal/Renderer2D.h>
#include <AEON/Graphics/GLResourceFactory.h>
#include <AEON/Graphics/Texture2D.h>

namespace ae
{
	// Public constructor(s)
	Shape::~Shape()
	{
	}

	// Public method(s)
	void Shape::setTexture(const Texture2D* const texture, bool resetRect)
	{
		// Assign the new texture
		mTexture = texture;

		// Set the new texture rect if requested or if there was no texture rect before
		if (mTexture) {
			if (resetRect || mTextureRect == Box2f()) {
				setTextureRect(Box2f(Vector2f(), Vector2f(mTexture->getSize())));
			}
			else {
				mUpdateUVs = true;
			}
		}
	}

	void Shape::setTextureRect(const Box2f& rect)
	{
		// Check if rect provided has valid values (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (rect.min.x < 0.f || rect.min.y < 0.f || rect.max.x < 0.f || rect.max.y < 0.f) {
				AEON_LOG_ERROR("Texture rect invalid", "The texture rect provided possesses negative values.");
			}
		}

		// Set the new texture and indicate that the uv coordinates need to be updated
		mTextureRect = rect;
		mUpdateUVs = true;
	}

	void Shape::setFillColor(const Color& color) noexcept
	{
		mFillColor = color;
		mUpdateFillColors = true;
	}

	void Shape::setOutlineColor(const Color& color) noexcept
	{
		mOutlineColor = color;
		mUpdateOutlineColors = true;
	}

	void Shape::setOutlineThickness(float thickness) noexcept
	{
		mOutlineThickness = thickness;
		mUpdateOutlinePositions = true;
	}

	const Texture2D* const Shape::getTexture() const noexcept
	{
		return mTexture;
	}

	const Box2f& Shape::getTextureRect() const noexcept
	{
		return mTextureRect;
	}

	const Color& Shape::getFillColor() const noexcept
	{
		return mFillColor;
	}

	const Color& Shape::getOutlineColor() const noexcept
	{
		return mOutlineColor;
	}

	float Shape::getOutlineThickness() const noexcept
	{
		return mOutlineThickness;
	}

	// Public virtual method(s)
	Box2f Shape::getModelBounds() const
	{
		return mModelBounds;
	}

	// Protected constructor(s)
	Shape::Shape()
		: Actor2D()
		, mModelBounds(0.f, 0.f, 0.f, 0.f)
		, mUpdatePositions(true)
		, mOutlineVertices()
		, mOutlineIndices()
		, mInnerBounds(0.f, 0.f, 0.f, 0.f)
		, mTextureRect(0.f, 0.f, 0.f, 0.f)
		, mFillColor(Color::White)
		, mOutlineColor(Color::White)
		, mTexture(nullptr)
		, mOutlineThickness(0.f)
		, mUpdateUVs(true)
		, mUpdateFillColors(true)
		, mUpdateOutlinePositions(true)
		, mUpdateOutlineColors(true)
	{
	}

	Shape::Shape(Shape&& rvalue) noexcept
		: Actor2D(std::move(rvalue))
		, mModelBounds(std::move(rvalue.mModelBounds))
		, mUpdatePositions(rvalue.mUpdatePositions)
		, mOutlineVertices(std::move(rvalue.mOutlineVertices))
		, mOutlineIndices(std::move(rvalue.mOutlineIndices))
		, mInnerBounds(std::move(rvalue.mInnerBounds))
		, mTextureRect(std::move(rvalue.mTextureRect))
		, mFillColor(std::move(rvalue.mFillColor))
		, mOutlineColor(std::move(rvalue.mOutlineColor))
		, mTexture(rvalue.mTexture)
		, mOutlineThickness(rvalue.mOutlineThickness)
		, mUpdateUVs(rvalue.mUpdateUVs)
		, mUpdateFillColors(rvalue.mUpdateFillColors)
		, mUpdateOutlinePositions(rvalue.mUpdateOutlinePositions)
		, mUpdateOutlineColors(rvalue.mUpdateOutlineColors)
	{
	}

	// Protected operator(s)
	Shape& Shape::operator=(Shape&& rvalue) noexcept
	{
		// Copy the rvalue's trivial data and move the rest
		Actor2D::operator=(std::move(rvalue));
		mModelBounds = std::move(rvalue.mModelBounds);
		mUpdatePositions = rvalue.mUpdatePositions;
		mOutlineVertices = std::move(rvalue.mOutlineVertices);
		mOutlineIndices = std::move(rvalue.mOutlineIndices);
		mInnerBounds = std::move(rvalue.mInnerBounds);
		mTextureRect = std::move(rvalue.mTextureRect);
		mFillColor = std::move(rvalue.mFillColor);
		mOutlineColor = std::move(rvalue.mOutlineColor);
		mTexture = rvalue.mTexture;
		mOutlineThickness = rvalue.mOutlineThickness;
		mUpdateUVs = rvalue.mUpdateUVs;
		mUpdateFillColors = rvalue.mUpdateFillColors;
		mUpdateOutlinePositions = rvalue.mUpdateOutlinePositions;
		mUpdateOutlineColors = rvalue.mUpdateOutlineColors;

		return *this;
	}

	// Private method(s)
	void Shape::updatePositions()
	{
		// Retrieve the total number of points and raise the flags indicating that uv coordinates and colors need to
		const size_t COUNT = getPointCount();

		// Raise the flags indicating that uv coordinates and colors will need to be updated if the point count is different
		std::vector<Vertex2D>& vertices = getVertices();
		if (COUNT != vertices.size() + 1) {
			mUpdateUVs = true;
			mUpdateFillColors = true;
		}

		// Update vertices
			// Resize list to the total number of points
		vertices.resize(COUNT + 1); // + 1 for the center point

			// Update the vertex positions
		for (size_t i = 0; i < COUNT; ++i) {
			vertices[i + 1].position = Vector3f(getPoint(i), getPosition().z);
		}

			// Update the inner bounding box
		Vector2f minPos = vertices[1].position.xy, maxPos = vertices[1].position.xy;
		for (const Vertex2D& vertex : vertices) {
			minPos = min(minPos, vertex.position.xy);
			maxPos = max(maxPos, vertex.position.xy);
		}
		mInnerBounds = Box2f(minPos, maxPos - minPos);
		mModelBounds = mInnerBounds;

			// Compute the center for the first vertex
		vertices[0].position = Vector3f(mInnerBounds.min + mInnerBounds.max / 2.f, getPosition().z);

		// Update indices (if necessary)
		const size_t INDEX_COUNT = vertices.size() * 2 + 2;
		std::vector<unsigned int>& indices = getIndices();
		if (indices.size() != INDEX_COUNT) {
			indices.clear();
			indices.reserve(INDEX_COUNT);

			// Add the indices
			for (size_t i = 1; i < vertices.size(); ++i) {
				size_t next = (i + 1) % vertices.size();
				next = (next == 0) ? 1 : next;

				indices.emplace_back(0);
				indices.emplace_back(i);
				indices.emplace_back(next);
			}
		}
	}

	void Shape::updateUVs()
	{
		const bool DIVISIBLEX = mInnerBounds.max.x > 0.f;
		const bool DIVISIBLEY = mInnerBounds.max.y > 0.f;
		const Vector2f TEXTURE_SIZE = (mTexture) ? Vector2f(mTexture->getSize()) : Vector2f(1.f, 1.f);

		std::vector<Vertex2D>& vertices = getVertices();
		for (Vertex2D& vertex : vertices) {
			Vector2f ratio((DIVISIBLEX) ? (vertex.position.x - mInnerBounds.min.x) / mInnerBounds.max.x : 0.f,
			               (DIVISIBLEY) ? (vertex.position.y - mInnerBounds.min.y) / mInnerBounds.max.y : 0.f);
			vertex.uv = (mTextureRect.min + mTextureRect.max * ratio) / TEXTURE_SIZE;
		}
	}

	void Shape::updateFillColors()
	{
		const Vector4f FILL_COLOR = mFillColor.normalize();
		std::vector<Vertex2D>& vertices = getVertices();
		for (Vertex2D& vertex : vertices) {
			vertex.color = FILL_COLOR;
		}
	}

	void Shape::updateShape()
	{
		// Updates the vertices' properties
		if (mUpdatePositions) {
			updatePositions();
			mUpdateOutlinePositions = true;
			correctProperties();
			setDirty(std::exchange(mUpdatePositions, false));
		}
		if (mUpdateUVs) {
			updateUVs();
			setDirty(std::exchange(mUpdateUVs, false));
		}
		if (mUpdateFillColors) {
			updateFillColors();
			setDirty(std::exchange(mUpdateFillColors, false));
		}
	}

	void Shape::updateOutlinePositions()
	{
		// Update the outline vertices
		const std::vector<Vertex2D>& vertices = getVertices();
		const size_t VERTEX_COUNT = vertices.size() - 1;
		mOutlineVertices.resize(VERTEX_COUNT * 2);
		for (size_t i = 0; i < VERTEX_COUNT; ++i) {
			size_t index = i + 1;
			size_t prev = (i == 0) ? VERTEX_COUNT : index - 1;
			size_t next = (i + 1 == VERTEX_COUNT) ? 1 : index + 1;

			// Get the two segments shared by the current point
			Vector2f p0 = vertices[prev].position.xy;
			Vector2f p1 = vertices[index].position.xy;
			Vector2f p2 = vertices[next].position.xy;

			// Compute the segments' normals
			Vector2f n1 = normal(p0, p1);
			Vector2f n2 = normal(p1, p2);

			// Make sure that the normals point outwards of the shape
			if (dot(n1, vertices[0].position.xy - p1) > 0.f) {
				n1 = -n1;
			}
			if (dot(n2, vertices[0].position.xy - p1) > 0.f) {
				n2 = -n2;
			}

			// Combine them to get the extrusion direction
			float factor = 1.f + (n1.x * n2.x + n1.y * n2.y);
			Vector2f normal = (n1 + n2) / factor;

			// Update the outline vertices
			mOutlineVertices[i * 2 + 0].position = Vector3f(p1, getPosition().z);
			mOutlineVertices[i * 2 + 1].position = Vector3f(p1 + normal * mOutlineThickness, getPosition().z);
		}

		// Update the model bounding box by taking into account the outline
		Vector2f minPos = mOutlineVertices[0].position.xy, maxPos = mOutlineVertices[0].position.xy;
		for (const Vertex2D& vertex : mOutlineVertices) {
			minPos = min(minPos, vertex.position.xy);
			maxPos = max(maxPos, vertex.position.xy);
		}
		mModelBounds = Box2f(minPos, maxPos - minPos);

		// Update the indices (if necessary)
		const size_t INDEX_COUNT = VERTEX_COUNT * 2;
		if (mOutlineIndices.size() != INDEX_COUNT) {
			mOutlineIndices.clear();
			mOutlineIndices.reserve(INDEX_COUNT * 6);

			// Add in indices
			for (size_t i = 0; i < INDEX_COUNT; i += 2) {
				const size_t NEXT = (i + 1) % INDEX_COUNT;
				const size_t NEXT2 = (i + 2) % INDEX_COUNT;
				const size_t NEXT3 = (i + 3) % INDEX_COUNT;

				mOutlineIndices.emplace_back(i);
				mOutlineIndices.emplace_back(NEXT);
				mOutlineIndices.emplace_back(NEXT2);

				mOutlineIndices.emplace_back(NEXT);
				mOutlineIndices.emplace_back(NEXT3);
				mOutlineIndices.emplace_back(NEXT2);
			}
		}

		// Indicate that the outline colors need to be updated as well
		mUpdateOutlineColors = true;
	}

	void Shape::updateOutlineColors()
	{
		const Vector4f OUTLINE_COLOR = mOutlineColor.normalize();
		for (Vertex2D& vertex : mOutlineVertices) {
			vertex.color = OUTLINE_COLOR;
		}
	}

	void Shape::updateOutline()
	{
		// Update the outline's vertices' properties
		if (mUpdateOutlinePositions) {
			updateOutlinePositions();
			correctProperties();
			setDirty(std::exchange(mUpdateOutlinePositions, false));
		}
		if (mUpdateOutlineColors) {
			updateOutlineColors();
			setDirty(std::exchange(mUpdateOutlineColors, false));
		}
	}

	// Private virtual method(s)
	void Shape::updateSelf(const Time& dt)
	{
		// Update the shape's properties which may raise the dirty render flag
		updateShape();

		// Update the outline's (if there is one) properties which may raise the dirty render flag
		if (mOutlineThickness != 0.f) {
			updateOutline();
		}
	}

	void Shape::renderSelf(RenderStates states) const
	{
		if (!getVertices().empty())
		{
			// Setup the render states used by both the outline and the shape
			if (!states.shader) {
				states.shader = GLResourceFactory::getInstance().get<Shader>("_AEON_Basic2D").get();
			}
			states.dirty = isDirty();

			// Render the outline (if there is one)
			if (mOutlineThickness != 0.f) {
				// Setup the appropriate render states
				states.blendMode = BlendMode::BlendNone;
				states.texture = nullptr;

				// Send the outline to the renderer
				Renderer2D::getActiveInstance()->submit(mOutlineVertices, mOutlineIndices, states);
			}

			// Setup the shape's render states
			states.blendMode = BlendMode::BlendAlpha;
			states.texture = mTexture;

			// Send the shape to the renderer
			Renderer2D::getActiveInstance()->submit(getVertices(), getIndices(), states);

			// Drop the dirty render flag
			setDirty(false);
		}
	}
}