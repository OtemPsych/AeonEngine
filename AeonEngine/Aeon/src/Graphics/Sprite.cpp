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

#include <AEON/Graphics/Sprite.h>

#include <AEON/Graphics/internal/Renderer2D.h>
#include <AEON/Graphics/GLResourceFactory.h>

namespace ae
{
	// Public constructor(s)
	Sprite::Sprite()
		: Actor()
		, mTextureRect(0.f, 0.f, 0.f, 0.f)
		, mTexture(nullptr)
		, mColor(Color::White)
	{
		init();
	}

	Sprite::Sprite(const Texture2D& texture, const Box2f& rect)
		: Actor()
		, mTextureRect()
		, mTexture(&texture)
		, mColor(Color::White)
	{
		init();

		// Setup the appropriate texture rect
		setTextureRect((rect == Box2f()) ? Box2f(Vector2f(0.f, 0.f), Vector2f(mTexture->getSize())) : rect);
	}

	Sprite::Sprite(const Sprite& copy)
		: Actor(copy)
		, mTextureRect(copy.mTextureRect)
		, mTexture(copy.mTexture)
		, mColor(copy.mColor)
	{
		init();
		updatePosUV();
	}

	// Public method(s)
	void Sprite::setTexture(const Texture2D& texture, bool resetRect)
	{
		mTexture = &texture;
		if (resetRect || mTextureRect == Box2f(0.f, 0.f, 0.f, 0.f)) {
			setTextureRect(Box2f(Vector2f(0.f, 0.f), Vector2f(mTexture->getSize())));
		}
		else {
			updatePosUV();
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
		updatePosUV();
	}

	void Sprite::setColor(const Color& color) noexcept
	{
		mColor = color;
		updateColor();
	}

	// Protected virtual method(s)
	void Sprite::renderSelf(RenderStates states) const
	{
		// Only render the sprite if a texture has been assigned
		if (mTexture) {
			const Render2DComponent* const renderComponent = getComponent<Render2DComponent>();
			const std::vector<Vertex2D>& vertices = renderComponent->getVertices();

			if (!vertices.empty()) {
				// Setup the appropriate render states
				if (!states.shader) {
					states.shader = GLResourceFactory::getInstance().get<Shader>("_AEON_Basic2D").get();
				}
				states.blendMode = BlendMode::BlendAlpha;
				states.texture = mTexture;

				// Send the sprite to the renderer
				Renderer2D::getActiveInstance()->submit(*renderComponent, states);
			}
		}
	}

	// Private method(s)
	void Sprite::updatePosUV()
	{
		// Update the positions
		const float POS_Z = getComponent<Transform2DComponent>()->getPosition().z;
		std::vector<Vertex2D>& vertices = getComponent<Render2DComponent>()->getVertices();
		vertices[0].position = Vector3f(Vector2f(0.f,                0.f)               , POS_Z);
		vertices[1].position = Vector3f(Vector2f(0.f,                mTextureRect.max.y), POS_Z);
		vertices[2].position = Vector3f(Vector2f(mTextureRect.max.x, mTextureRect.max.y), POS_Z);
		vertices[3].position = Vector3f(Vector2f(mTextureRect.max.x, 0.f)               , POS_Z);

		// Update the model bounding box's position and size based on the minimum and maximum vertex positions
		getComponent<Collider2DComponent>()->setModelBounds(Box2f(vertices[0].position.xy, vertices[2].position.xy - vertices[0].position.xy));

		// Update the UV coordinates
		const Vector2f TEXTURE_SIZE = (mTexture) ? Vector2f(mTexture->getSize()) : Vector2f(1.f, 1.f);
		vertices[0].uv = Vector2f(mTextureRect.min.x,                      mTextureRect.min.y)                      / TEXTURE_SIZE;
		vertices[1].uv = Vector2f(mTextureRect.min.x,                      mTextureRect.min.y + mTextureRect.max.y) / TEXTURE_SIZE;
		vertices[2].uv = Vector2f(mTextureRect.min.x + mTextureRect.max.x, mTextureRect.min.y + mTextureRect.max.y) / TEXTURE_SIZE;
		vertices[3].uv = Vector2f(mTextureRect.min.x + mTextureRect.max.x, mTextureRect.min.y)                      / TEXTURE_SIZE;
	}

	void Sprite::updateColor()
	{
		const Vector4f COLOR = mColor.normalize();
		std::vector<Vertex2D>& vertices = getComponent<Render2DComponent>()->getVertices();
		for (Vertex2D& vertex : vertices) {
			vertex.color = COLOR;
		}
	}

	void Sprite::init()
	{
		// Add components
		addComponent<Transform2DComponent>();
		addComponent<Collider2DComponent>();
		addComponent<Render2DComponent>();

		// Add the vertices and indices
		Render2DComponent* const renderComponent = getComponent<Render2DComponent>();
		renderComponent->getVertices().resize(4);
		renderComponent->getIndices() = {
			0, 1, 2,
			0, 2, 3
		};

		// Update the color in case the color isn't changed by the user
		updateColor();
	}
}