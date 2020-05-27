// MIT License
// 
// Copyright(c) 2019-2020 Filippos Gleglakos
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

#include <AEON/Graphics/Renderer2D.h>

namespace ae
{
	// Public constructor(s)
	Sprite::Sprite()
		: Actor2D()
		, mTexture(nullptr)
		, mTextureRect(0.f, 0.f, 0.f, 0.f)
		, mColor(Color::White)
		, mUpdatePosUV(true)
		, mUpdateColor(true)
	{
		// Create 4 vertices
		mVertices.resize(4);
	}

	Sprite::Sprite(const Texture2D& texture, const Box2f& rect)
		: Actor2D()
		, mTexture(&texture)
		, mTextureRect()
		, mColor(Color::White)
	{
		// Create 4 vertices
		mVertices.resize(4);

		// Setup the appropriate texture rect
		setTextureRect((rect == Box2f()) ? Box2f(Vector2f(0.f, 0.f), Vector2f(mTexture->getSize())) : rect);
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
	void Sprite::setOrigin(const Vector2f& origin) noexcept
	{
		Transformable2D::setOrigin(origin);
		mUpdatePosUV = true;
	}

	Box2f Sprite::getModelBounds() const
	{
		return mModelBounds;
	}

	// Private method(s)
	void Sprite::updatePosUV()
	{
		// Check if there are sufficient vertices (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (mVertices.empty()) {
				AEON_LOG_ERROR("Insufficient vertex count", "Unable to update positions due to empty vertex array.");
				return;
			}
		}

		// Update the positions
		const Vector2f& origin = getOrigin();
		mVertices[0].position = Vector2f(0.f,                0.f)                - origin;
		mVertices[1].position = Vector2f(0.f,                mTextureRect.max.y) - origin;
		mVertices[2].position = Vector2f(mTextureRect.max.x, mTextureRect.max.y) - origin;
		mVertices[3].position = Vector2f(mTextureRect.max.x, 0.f)                - origin;

		// Update the model bounding box's position and size based on the minimum and maximum vertex positions
		mModelBounds = Box2f(mVertices[0].position, mVertices[2].position - mVertices[0].position);

		// Update the UV coordinates
		const Vector2f TEXTURE_SIZE = (mTexture) ? Vector2f(mTexture->getSize()) : Vector2f(1.f, 1.f);
		mVertices[0].uv = Vector2f(mTextureRect.min.x,                      mTextureRect.min.y + mTextureRect.max.y) / TEXTURE_SIZE;
		mVertices[1].uv = Vector2f(mTextureRect.min.x,                      mTextureRect.min.y)                      / TEXTURE_SIZE;
		mVertices[2].uv = Vector2f(mTextureRect.min.x + mTextureRect.max.x, mTextureRect.min.y)                      / TEXTURE_SIZE;
		mVertices[3].uv = Vector2f(mTextureRect.min.x + mTextureRect.max.x, mTextureRect.min.y + mTextureRect.max.y) / TEXTURE_SIZE;
	}

	void Sprite::updateColor()
	{
		const Vector4f COLOR = mColor.normalize();
		for (Vertex2D& vertex : mVertices) {
			vertex.color = COLOR;
		}
	}

	// Private virtual method(s)
	bool Sprite::renderSelf(RenderStates states)
	{
		// Setup the appropriate render states
		if (!states.shader) {
			states.shader = GLResourceFactory::getInstance().get<Shader>("Sprite").get();
		}
		if (mColor.a != 255) {
			states.blendMode = BlendMode::BlendAlpha;
		}
		states.texture = mTexture;

		// Send the sprite to the renderer
		Renderer2D::getInstance().submit(*this, states);
		return true;
	}

	void Sprite::updateVertices()
	{
		// Update the vertices' properties
		if (mUpdatePosUV) {
			updatePosUV();
			mUpdatePosUV = false;
		}
		if (mUpdateColor) {
			updateColor();
			mUpdateColor = false;
		}
	}

	void Sprite::updateIndices()
	{
		// Set the indices in a counter-clockwise position
		mIndices = {
			0, 1, 2,
			0, 2, 3
		};
	}
}