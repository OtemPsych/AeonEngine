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

#pragma once

#include <AEON/Graphics/Actor.h>
#include <AEON/Graphics/Color.h>
#include <AEON/Graphics/Texture2D.h>
#include <AEON/Math/AABoxCollider.h>

namespace ae
{
	/*!
	 \brief The class representing the simplest form of a textured 2D object.
	*/
	class AEON_API Sprite : public Actor
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Creates an ae::Sprite without a texture and a white color.

		 \since v0.6.0
		*/
		Sprite();
		/*!
		 \brief Constructs the ae::Sprite by providing a \a texture and an optional texture \a rect.

		 \param[in] texture The ae::Texture2D to assign to the ae::Sprite
		 \param[in] rect An ae::Box2f containing the texture coordinates of the \a texture to be used, uses the entire \a texture by default

		 \par Example:
		 \code
		 ae::Texture2D texture;
		 texture.loadFromFile("Textures/texture.png");

		 auto sprite1 = std::make_unique<ae::Sprite>(texture);                                   // uses the entire texture
		 auto sprite2 = std::make_unique<ae::Sprite>(texture, ae::Box2f(0.f, 0.f, 120.f, 30.f)); // uses part of the texture
		 \endcode

		 \since v0.4.0
		*/
		explicit Sprite(const Texture2D& texture, const Box2f& rect = Box2f());
		/*!
		 \brief Copy constructor.

		 \param[in] copy The ae::Sprite that will be copied

		 \since v0.6.0
		*/
		Sprite(const Sprite& copy);
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Sprite that will be moved

		 \since v0.7.0
		*/
		Sprite(Sprite&& rvalue) noexcept = default;
		/*!
		 \brief Virtual destructor.
		 \details A virtual destructor is needed as this class may be inherited by user-created classes.

		 \since v0.7.0
		*/
		virtual ~Sprite() = default;

		// Public operator(s)
		/*!
		 \brief Assignment operator.

		 \param[in] other The ae::Sprite that will be copied

		 \return The caller ae::Sprite

		 \since v0.6.0
		*/
		Sprite& operator=(const Sprite& other) = default;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Sprite that will be moved

		 \return The caller ae::Sprite

		 \since v0.7.0
		*/
		Sprite& operator=(Sprite&& rvalue) noexcept = default;

		// Public method(s)
		/*!
		 \brief Sets the ae::Sprite's \a texture and the option to reset the current texture rect to the provided \a texture's dimensions.
		 \details The current texture rect will always be replaced if it's currently empty.

		 \param[in] texture The ae::Texture2D to assign to the ae::Sprite
		 \param[in] resetRect True to overwrite the current texture rect with the provided \a texture's dimensions, false by default

		 \par Example:
		 \code
		 ae::Texture2D texture;
		 texture.loadFromFile("Textures/texture.png");

		 auto sprite = std::make_unique<ae::Sprite>();
		 sprite->setTexture(texture); // current texture rect is empty so it's replaced by the provided texture's coordinates

		 ae::Texture2D texture2;
		 texture2.loadFromFile("Textures/texture2.png");

		 sprite->setTexture(texture2); // current texture rect isn't replaced by the texture's coordinates
		 \endcode

		 \since v0.7.0
		*/
		void setTexture(const Texture2D& texture, bool resetRect = false);
		/*!
		 \brief Sets the ae::Sprite's texture rect.
		 \details The texture rect represents the area of the assigned texture to display.
		 \note A texture has to be assigned for the texture rect to have any effect.

		 \param[in] rect The ae::Box2f containing the starting position and the size of the area of the texture

		 \par Example:
		 \code
		 // Create a sprite assigned to a 250x250 texture
		 auto sprite = std::make_unique<ae::Sprite>(texture);

		 ...

		 // Set the texture rect starting position at (50, 50) and a size of (100, 100)
		 sprite->setTextureRect(ae::Box2f(50.f, 50.f, 100.f, 100.f));
		 \endcode

		 \sa setTexture()

		 \since v0.4.0
		*/
		void setTextureRect(const Box2f& rect);
		/*!
		 \brief Sets the ae::Sprite's \a color which will saturate the assigned texture.
		 \details A color of white will leave the assigned texture unmodified.

		 \param[in] color The ae::Color that will saturate the assigned texture (if there is one)

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>();
		 sprite->setTextureRect(ae::Box2f(0.f, 0.f, 50.f, 50.f)); // an untextured quad with a size of 50x50
		 sprite->setColor(ae::Color::Green);
		 \endcode

		 \sa getColor()

		 \since v0.4.0
		*/
		void setColor(const Color& color) noexcept;
		/*!
		 \brief Retrieves the ae::Sprite's assigned texture.
		 \note If no texture was assigned, nullptr will be returned.

		 \return Pointer to the assigned texture or nullptr if no texture was assigned

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ...
		 const Texture2D* const texture = sprite->getTexture();
		 \endcode

		 \sa setTexture(), getTextureRect()

		 \since v0.7.0
		*/
		[[nodiscard]] inline const Texture2D* const getTexture() const noexcept { return mTexture; }
		/*!
		 \brief Retrieves the ae::Sprite's texture rect.
		 \details The texture rect represents the area of the assigned texture to display.
		 \note A texture has to be assigned for the texture rect to have any effect.

		 \return The ae::Box2f containing the starting position and the size of the area of the texture

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(someTexture);
		 ...
		 const ae::Box2f& textureRect = sprite->getTextureRect();
		 \endcode

		 \sa setTextureRect(), getTexture()

		 \since v0.7.0
		*/
		[[nodiscard]] inline const Box2f& getTextureRect() const noexcept { return mTextureRect; }
		/*!
		 \brief Retrieves the color of the ae::Sprite.
		 \details The color refers to the color used to saturate the assigned texture or make it transparent.

		 \return The ae::Color used for the sprite's saturation color

		 \par Example:
		 \code
		 auto sprite = std::make_unique<ae::Sprite>(texture);
		 sprite->setColor(ae::Color::Cyan);
		 ...
		 const ae::Color& color = sprite->getColor();
		 \endcode

		 \sa setColor()

		 \since v0.7.0
		*/
		[[nodiscard]] inline const Color& getColor() const noexcept { return mColor; }
	protected:
		// Protected virtual method(s)
		/*!
		 \brief Sends the vertex data and render states to the renderer.
		 \details Sets the appropriate shader, blend mode and texture.

		 \param[in] states The ae::RenderStates defining the OpenGL state

		 \sa updateSelf()

		 \since v0.7.0
		*/
		virtual void renderSelf(RenderStates states) const override;
	private:
		// Private method(s)
		/*!
		 \brief Updates the stored vertices' positions and texture coordinates.
		 \details Called when the texture or the texture rect changes.

		 \sa updateColor()

		 \since v0.7.0
		*/
		void updatePosUV();
		/*!
		 \brief Updates the stored vertices' color.
		 \details Called when the color changes.

		 \sa updatePosUV()

		 \since v0.7.0
		*/
		void updateColor();
		/*!
		 \brief Adds the necessary components and creates the vertex data.

		 \since v0.7.0
		*/
		void init();

		// Private member(s)
		Box2f            mTextureRect; //!< The texture rectangle containing the texture coordinates
		const Texture2D* mTexture;     //!< The texture to assign to the sprite
		Color            mColor;       //!< The color of the sprite
	};
}

/*!
 \class ae::Sprite
 \ingroup graphics

 The ae::Sprite and ae::RectangleShape instances behave very similarly, but the
 main difference is that an ae::Sprite heavily depends on its texture as it
 also acts as the dimensions of the sprite. This results in a texture that
 never appears stretched or in any way deformed.

 An ae::Sprite should be used when an entire texture (or part of that texture)
 needs to be rendered in said texture's dimensions.

 A textured ae::RectangleShape or ae::EllipseShape should be used when the
 dimensions need to be a fixed size (or radius when using an ellipse)
 regardless of the texture applied.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.12.27
 \copyright MIT License
*/