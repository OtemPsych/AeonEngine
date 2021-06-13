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

#include <AEON/Graphics/Sprite.h>

#include <AEON/Graphics/internal/Renderer2D.h>
#include <AEON/Graphics/GLResourceFactory.h>

namespace ae
{
	// Public constructor(s)
	Sprite::Sprite()
		: Actor2D()
		, mModelBounds()
		, mTextureRect(0.f, 0.f, 0.f, 0.f)
		, mTexture(nullptr)
		, mColor(Color::White)
		, mUpdatePosUV(true)
		, mUpdateColor(true)
	{
	}

	Sprite::Sprite(const Texture2D& texture, const Box2f& rect)
		: Actor2D()
		, mModelBounds()
		, mTextureRect()
		, mTexture(&texture)
		, mColor(Color::White)
		, mUpdatePosUV(true)
		, mUpdateColor(true)
	{
		// Setup the appropriate texture rect
		setTextureRect((rect == Box2f()) ? Box2f(Vector2f(0.f, 0.f), Vector2f(mTexture->getSize())) : rect);
	}

	Sprite::Sprite(Sprite&& rvalue) noexcept
		: Actor2D(std::move(rvalue))
		, mModelBounds(std::move(rvalue.mModelBounds))
		, mTextureRect(std::move(rvalue.mTextureRect))
		, mTexture(rvalue.mTexture)
		, mColor(std::move(rvalue.mColor))
		, mUpdatePosUV(rvalue.mUpdatePosUV)
		, mUpdateColor(rvalue.mUpdateColor)
	{
	}

	// Public operator(s)
	Sprite& Sprite::operator=(Sprite&& rvalue) noexcept
	{
		// Copy the rvalue's trivial data and move the rest
		Actor2D::operator=(std::move(rvalue));
		mModelBounds = std::move(rvalue.mModelBounds);
		mTextureRect = std::move(rvalue.mTextureRect);
		mTexture = rvalue.mTexture;
		mColor = std::move(rvalue.mColor);
		mUpdatePosUV = rvalue.mUpdatePosUV;
		mUpdateColor = rvalue.mUpdateColor;

		return *this;
	}

	// Public method(s)
	void Sprite::setTexture(const Texture2D& texture, bool resetRect)
	{
		mTexture = &texture;
		if (resetRect || mTextureRect == Box2f(0.f, 0.f, 0.f, 0.f)) {
			setTextureRect(Box2f(Vector2f(0.f, 0.f), Vector2f(mTexture->getSize())));
		}
		else {
			mUpdatePosUV = true;
		}
	}

	void Sprite::setTextureRect(const Box2f& rect)
	{
		// Check if rect provided has valid values (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (rect.min.x < 0.f || rect.min.y < 0.f || rect.max.x < 0.f || rect.max.y < 0.f) {
				const std::string RECT_STR = "(" + std::to_string(rect.min.x) + ", " + std::to_string(rect.min.y) + ", " +
				                             std::to_string(rect.max.x) + ", " + std::to_string(rect.max.y) + ")";
				AEON_LOG_ERROR("Texture rect invalid", "The texture rect " + RECT_STR + " possesses negative values.");
			}
		}

		// Set the new texture and indicate that the uv coordinates need to be updated
		mTextureRect = rect;
		mUpdatePosUV = true;
	}

	void Sprite::setColor(const Color& color) noexcept
	{
		mColor = color;
		mUpdateColor = true;
	}

	const Texture2D* const Sprite::getTexture() const noexcept
	{
		return mTexture;
	}

	const Box2f& Sprite::getTextureRect() const noexcept
	{
		return mTextureRect;
	}

	const Color& Sprite::getColor() const noexcept
	{
		return mColor;
	}

	// Public virtual method(s)
	Box2f Sprite::getModelBounds() const
	{
		return mModelBounds;
	}

	// Private method(s)
	void Sprite::updatePosUV()
	{
		std::vector<Vertex2D>& vertices = getVertices();
		if (vertices.empty()) {
			vertices.resize(4);
		}

			// Update the positions
		const float POS_Z = getPosition().z;
		vertices[0].position = Vector3f(Vector2f(0.f,                0.f)               , POS_Z);
		vertices[1].position = Vector3f(Vector2f(0.f,                mTextureRect.max.y), POS_Z);
		vertices[2].position = Vector3f(Vector2f(mTextureRect.max.x, mTextureRect.max.y), POS_Z);
		vertices[3].position = Vector3f(Vector2f(mTextureRect.max.x, 0.f)               , POS_Z);

			// Update the model bounding box's position and size based on the minimum and maximum vertex positions
		mModelBounds = Box2f(vertices[0].position.xy, vertices[2].position.xy - vertices[0].position.xy);

			// Update the UV coordinates
		const Vector2f TEXTURE_SIZE = (mTexture) ? Vector2f(mTexture->getSize()) : Vector2f(1.f, 1.f);
		vertices[0].uv = Vector2f(mTextureRect.min.x,                      mTextureRect.min.y)                      / TEXTURE_SIZE;
		vertices[1].uv = Vector2f(mTextureRect.min.x,                      mTextureRect.min.y + mTextureRect.max.y) / TEXTURE_SIZE;
		vertices[2].uv = Vector2f(mTextureRect.min.x + mTextureRect.max.x, mTextureRect.min.y + mTextureRect.max.y) / TEXTURE_SIZE;
		vertices[3].uv = Vector2f(mTextureRect.min.x + mTextureRect.max.x, mTextureRect.min.y)                      / TEXTURE_SIZE;

		// Update indices (if necessary)
		std::vector<unsigned int>& indices = getIndices();
		if (indices.size() != 6) {
			indices = {
				0, 1, 2,
				0, 2, 3
			};
		}
	}

	void Sprite::updateColor()
	{
		// Normalize the color so that it may be passed on to the shader
		const Vector4f COLOR = mColor.normalize();

		// Assign the normalized color to all the vertices
		std::vector<Vertex2D>& vertices = getVertices();
		for (Vertex2D& vertex : vertices) {
			vertex.color = COLOR;
		}
	}

	// Private virtual method(s)
	void Sprite::updateSelf(const Time& dt)
	{
		// Update the sprite's properties which may raise the dirty render flag
		if (mUpdatePosUV) {
			updatePosUV();
			correctProperties();
			setDirty(std::exchange(mUpdatePosUV, false));
		}
		if (mUpdateColor) {
			updateColor();
			setDirty(std::exchange(mUpdateColor, false));
		}
	}

	void Sprite::renderSelf(RenderStates states) const
	{
		// Only render the sprite if a texture has been assigned
		if (mTexture)
		{
			// Setup the appropriate render states
			if (!states.shader) {
				states.shader = GLResourceFactory::getInstance().get<Shader>("_AEON_Basic2D").get();
			}
			states.blendMode = BlendMode::BlendAlpha;
			states.texture = mTexture;
			states.dirty = isDirty();

			// Send the sprite to the renderer
			Renderer2D::getActiveInstance()->submit(getVertices(), getIndices(), states);

			// Drop the dirty render flag
			setDirty(false);
		}
	}
}