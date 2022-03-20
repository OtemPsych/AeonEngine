// MIT License
// 
// Copyright(c) 2019-2022 Filippos Gleglakos
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
	// Public method(s)
	void Shape::setTexture(const Texture2D* const texture, bool resetRect)
	{
		mTexture = texture;
		if (mTexture) {
			if (resetRect || mTextureRect == Box2f()) {
				setTextureRect(Box2f(Vector2f(0.f, 0.f), Vector2f(mTexture->getSize())));
			}
			else {
				updateUVs();
			}
		}
	}

	void Shape::setTextureRect(const Box2f& rect)
	{
		assert(rect.min.x >= 0.f && rect.min.y >= 0.f && rect.max.x >= 0.f && rect.max.y >= 0.f);

		// Set the new texture rect and update the uv coordinates
		mTextureRect = rect;
		updateUVs();
	}

	void Shape::setFillColor(const Color& color) noexcept
	{
		mFillColor = color;
		updateFillColors();
	}

	void Shape::setOutlineColor(const Color& color) noexcept
	{
		mOutlineColor = color;
		updateOutlineColors();
	}

	void Shape::setOutlineThickness(float thickness) noexcept
	{
		mOutlineThickness = thickness;
		updateOutlinePositions();
	}

	// Protected constructor(s)
	Shape::Shape()
		: Actor()
		, mOutlineVertices()
		, mOutlineIndices()
		, mInnerBounds(0.f, 0.f, 0.f, 0.f)
		, mTextureRect(0.f, 0.f, 0.f, 0.f)
		, mFillColor(Color::White)
		, mOutlineColor(Color::White)
		, mTexture(nullptr)
		, mOutlineThickness(0.f)
	{
		// Add components
		addComponent<Transform2DComponent>();
		addComponent<Collider2DComponent>();
		addComponent<Render2DComponent>();
	}

	Shape::Shape(const Shape& copy)
		: Actor(copy)
		, mOutlineVertices(copy.mOutlineVertices)
		, mOutlineIndices(copy.mOutlineIndices)
		, mInnerBounds(copy.mInnerBounds)
		, mTextureRect(copy.mTextureRect)
		, mFillColor(copy.mFillColor)
		, mOutlineColor(copy.mOutlineColor)
		, mTexture(copy.mTexture)
		, mOutlineThickness(copy.mOutlineThickness)
	{
		// Add components
		addComponent<Transform2DComponent>();
		addComponent<Collider2DComponent>();
		addComponent<Render2DComponent>();
	}

	// Protected method(s)
	void Shape::updatePositions()
	{
		Transform2DComponent* const transform2DComponent = getComponent<Transform2DComponent>();
		Collider2DComponent* const collider2DComponent = getComponent<Collider2DComponent>();
		Render2DComponent* const render2DComponent = getComponent<Render2DComponent>();

		std::vector<Vertex2D>& vertices = render2DComponent->getVertices();
		const size_t COUNT = getPointCount();
		const bool MUST_UPDATE_FILLCOLORS = COUNT != vertices.size() + 1;

		// Update vertices
			// Resize list to the total number of points
		vertices.resize(COUNT + 1); // + 1 for the center point

			// Update the vertex positions and the inner bounding box
		const float Z_POS = transform2DComponent->getPosition().z;
		Vector2f minPos(std::numeric_limits<float>().max()), maxPos(std::numeric_limits<float>().min());
		for (size_t i = 0; i < COUNT; ++i) {
			vertices[i + 1].position = Vector3f(getPoint(i), Z_POS);
			if (minPos.x > vertices[i + 1].position.x) {
				minPos.x = vertices[i + 1].position.x;
			}
			if (minPos.y > vertices[i + 1].position.y) {
				minPos.y = vertices[i + 1].position.y;
			}
			if (maxPos.x < vertices[i + 1].position.x) {
				maxPos.x = vertices[i + 1].position.x;
			}
			if (maxPos.y < vertices[i + 1].position.y) {
				maxPos.y = vertices[i + 1].position.y;
			}
		}
		mInnerBounds = Box2f(minPos, maxPos - minPos);
		collider2DComponent->setModelBounds(mInnerBounds);

			// Compute the center for the first vertex
		vertices[0].position = Vector3f(mInnerBounds.position + mInnerBounds.size / 2.f, Z_POS);

		// Update indices (if necessary)
		const size_t INDEX_COUNT = vertices.size() * 2 + 2;
		std::vector<uint32_t>& indices = render2DComponent->getIndices();
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

		// Update the UV coordinates, the outline, and the colors if the point count changed
		updateUVs();
		updateOutlinePositions();
		if (MUST_UPDATE_FILLCOLORS) {
			updateFillColors();
		}
	}

	// Protected virtual method(s)
	void Shape::renderSelf(RenderStates states) const
	{
		const Render2DComponent* const render2DComponent = getComponent<Render2DComponent>();
		const std::vector<Vertex2D>& vertices = render2DComponent->getVertices();

		if (!vertices.empty())
		{
			// Setup the render states used by both the outline and the shape
			if (!states.shader) {
				states.shader = GLResourceFactory::getInstance().get<Shader>("_AEON_Basic2D").get();
			}

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
			Renderer2D::getActiveInstance()->submit(*render2DComponent, states);
		}
	}

	// Private method(s)
	void Shape::updateUVs()
	{
		const bool DIVISIBLEX = mInnerBounds.max.x > 0.f;
		const bool DIVISIBLEY = mInnerBounds.max.y > 0.f;
		const Vector2f TEXTURE_SIZE = (mTexture) ? Vector2f(mTexture->getSize()) : Vector2f(1.f, 1.f);

		std::vector<Vertex2D>& vertices = getComponent<Render2DComponent>()->getVertices();
		for (Vertex2D& vertex : vertices) {
			Vector2f ratio((DIVISIBLEX) ? (vertex.position.x - mInnerBounds.min.x) / mInnerBounds.max.x : 0.f,
			               (DIVISIBLEY) ? (vertex.position.y - mInnerBounds.min.y) / mInnerBounds.max.y : 0.f);
			vertex.uv = (mTextureRect.min + mTextureRect.max * ratio) / TEXTURE_SIZE;
		}
	}

	void Shape::updateFillColors()
	{
		const Vector4f FILL_COLOR = mFillColor.normalize();
		std::vector<Vertex2D>& vertices = getComponent<Render2DComponent>()->getVertices();
		for (Vertex2D& vertex : vertices) {
			vertex.color = FILL_COLOR;
		}
	}

	void Shape::updateOutlinePositions()
	{
		const std::vector<Vertex2D>& vertices = getComponent<Render2DComponent>()->getVertices();
		if (vertices.empty()) {
			return;
		}

		const size_t VERTEX_COUNT = vertices.size() - 1;
		const bool MUST_UPDATE_OUTLINECOLORS = VERTEX_COUNT != mOutlineVertices.size() * 2;

		// Update the outline vertices
		mOutlineVertices.resize(VERTEX_COUNT * 2);
		for (size_t i = 0; i < VERTEX_COUNT; ++i) {
			size_t index = i + 1;
			size_t prev = (i == 0) ? VERTEX_COUNT : i;
			size_t next = (index == VERTEX_COUNT) ? 1 : index + 1;

			// Get the two segments shared by the current point
			Vector2f p0 = vertices[prev].position.xy;
			Vector2f p1 = vertices[index].position.xy;
			Vector2f p2 = vertices[next].position.xy;

			// Compute the segments' normals
			Vector2f n1 = normal(p0, p1), n2 = normal(p1, p2);

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
			const float Z_POS = getComponent<Transform2DComponent>()->getPosition().z;
			mOutlineVertices[i * 2 + 0].position = Vector3f(p1, Z_POS);
			mOutlineVertices[i * 2 + 1].position = Vector3f(p1 + normal * mOutlineThickness, Z_POS);
		}

		// Update the model bounding box by taking into account the outline
		Vector2f minPos = mOutlineVertices[0].position.xy, maxPos = mOutlineVertices[0].position.xy;
		for (const Vertex2D& vertex : mOutlineVertices) {
			minPos = min(minPos, vertex.position.xy);
			maxPos = max(maxPos, vertex.position.xy);
		}
		getComponent<Collider2DComponent>()->setModelBounds(Box2f(minPos, maxPos - minPos));

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
		if (MUST_UPDATE_OUTLINECOLORS) {
			updateOutlineColors();
		}
	}

	void Shape::updateOutlineColors()
	{
		const Vector4f OUTLINE_COLOR = mOutlineColor.normalize();
		for (Vertex2D& vertex : mOutlineVertices) {
			vertex.color = OUTLINE_COLOR;
		}
	}
}