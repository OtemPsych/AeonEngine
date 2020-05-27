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

#include <AEON/Graphics/internal/Shape.h>

#include <AEON/Graphics/GLResourceFactory.h>
#include <AEON/Graphics/Renderer2D.h>

namespace ae
{
	// Public method(s)
	void Shape::setTexture(const Texture2D& texture, bool resetRect)
	{
		mTexture = &texture;
		if (resetRect || mTextureRect == Box2f(0.f, 0.f, 0.f, 0.f)) {
			setTextureRect(Box2f(Vector2f(0.f, 0.f), Vector2f(texture.getSize())));
		}
		else {
			mUpdateUV = true;
		}
	}

	void Shape::setTextureRect(const Box2f& rect)
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
		mUpdateUV = true;
	}

	void Shape::setFillColor(const Color& color) noexcept
	{
		mFillColor = color;
		mUpdateFillColor = true;
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

	// Public virtual method(s)
	void Shape::setOrigin(const Vector2f& origin) noexcept
	{
		Transformable2D::setOrigin(origin);
		mUpdatePosition = true;
	}

	Box2f Shape::getModelBounds() const
	{
		return mModelBounds;
	}

	// Protected constructor(s)
	Shape::Shape()
		: Actor2D()
		, mModelBounds(0.f, 0.f, 0.f, 0.f)
		, mUpdatePosition(true)
		, mTextureRect(0.f, 0.f, 0.f, 0.f)
		, mFillColor(Color::White)
		, mTexture(nullptr)
		, mUpdateUV(true)
		, mUpdateFillColor(true)
	{
	}

	// Private method(s)
	void Shape::updateFillColor()
	{
		const Vector4f FILL_COLOR = mFillColor.normalize();
		for (Vertex2D& vertex : mVertices) {
			vertex.color = FILL_COLOR;
		}
	}

	// Private virtual method(s)
	bool Shape::renderSelf(RenderStates states)
	{
		// Setup the appropriate render states
		if (!states.shader) {
			states.shader = GLResourceFactory::getInstance().get<Shader>("Sprite").get();
		}
		if (mFillColor.a != 255) {
			states.blendMode = BlendMode::BlendAlpha;
		}
		states.texture = mTexture;

		// Send the shape to the renderer
		Renderer2D::getInstance().submit(*this, states);
		return true;
	}

	void Shape::updateVertices()
	{
		// Update the vertices' properties
		if (mUpdatePosition) {
			updatePosition();
			mUpdatePosition = false;
		}
		if (mUpdateUV) {
			updateUV();
			mUpdateUV = false;
		}
		if (mUpdateFillColor) {
			updateFillColor();
			mUpdateFillColor = false;
		}
	}
}