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

#ifndef Aeon_Graphics_Sprite_H_
#define Aeon_Graphics_Sprite_H_

#include <AEON/Graphics/Actor2D.h>
#include <AEON/Graphics/Color.h>
#include <AEON/Graphics/Texture2D.h>
#include <AEON/Math/AABoxCollider.h>

namespace ae
{
	/*!
	 \brief The class representing a drawable and transformable 2D object.
	*/
	class _NODISCARD AEON_API Sprite : public Actor2D
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Creates an ae::Sprite without a texture and a white color.

		 \since v0.4.0
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
		 \brief Deleted copy constructor.

		 \since v0.5.0
		*/
		Sprite(const Sprite&) = delete;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Sprite that will be moved

		 \since v0.5.0
		*/
		Sprite(Sprite&& rvalue) noexcept;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.5.0
		*/
		Sprite& operator=(const Sprite&) = delete;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Sprite that will be moved

		 \return The caller ae::Sprite

		 \since v0.5.0
		*/
		Sprite& operator=(Sprite&& rvalue) noexcept;
	public:
		// Public method(s)
		/*!
		 \brief Sets the ae::Sprite's \a texture and the option to reset the current texture rect to the provided \a texture's dimensions.
		 \details The current texture rect will be replaced regardless if it's currently empty.

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

		 sprite->setTexture(texture2); // current texture rects keeps its previous coordinates
		 \endcode
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

		 \since v0.4.0
		*/
		_NODISCARD const Texture2D* const getTexture() const noexcept;
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

		 \since v0.4.0
		*/
		_NODISCARD const Box2f& getTextureRect() const noexcept;
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

		 \since v0.4.0
		*/
		_NODISCARD const Color& getColor() const noexcept;

		// Public virtual method(s)
		/*!
		 \brief Retrieves the ae::Sprite's model bounding box.

		 \return An ae::Box2f containing the model bounding box.

		 \since v0.4.0
		*/
		_NODISCARD virtual Box2f getModelBounds() const override final;
	private:
		// Private method(s)
		/*!
		 \brief Updates the stored vertices' positions and texture coordinates.
		 \details Called when the vertices were attempted to be retrieved and the texture or the texture rect had changed.

		 \sa updateColor(), updateVertices()

		 \since v0.5.0
		*/
		void updatePosUV();
		/*!
		 \brief Updates the stored vertices' color.
		 \details Called when the vertices were attempted to be retrieved and the color had changed.

		 \sa updatePosUV(), updateVertices()

		 \since v0.4.0
		*/
		void updateColor();
		/*!
		 \brief Updates the ae::Sprite's stored vertices.
		 \details Calls the specified update methods.

		 \sa updatePosUV(), updateColor(), updateIndices()

		 \since v0.4.0
		*/
		bool updateVertices();

		// Private virtual method(s)
		/*!
		 \brief Updates the ae::Sprite's vertex data.

		 \param[in] dt The time difference between the previous frame and the current frame

		 \sa renderSelf()

		 \since v0.5.0
		*/
		virtual void updateSelf(const Time& dt) override final;
		/*!
		 \brief Sends the vertex data and render states to the renderer.
		 \details Sets the appropriate shader, blend mode and texture.

		 \param[in] states The ae::RenderStates defining the OpenGL state

		 \sa updateSelf()

		 \since v0.5.0
		*/
		virtual void renderSelf(RenderStates states) const override final;

	private:
		// Private member(s)
		Box2f            mModelBounds; //!< The local model bounds of the sprite
		Box2f            mTextureRect; //!< The texture rectangle containing the texture coordinates
		const Texture2D* mTexture;     //!< The texture to assign to the sprite
		Color            mColor;       //!< The color of the sprite

		bool             mUpdatePosUV; //!< Whether the vertices' positions and texture coordinates need to be updated
		bool             mUpdateColor; //!< Whether the vertices' color needs to be updated
	};
}
#endif // Aeon_Graphics_Sprite_H_

/*!
 \class ae::Sprite
 \ingroup graphics

 The ae::Sprite class is used to represent a 2D object to the screen usually
 holding a texture (an image), but not necessarily.

 The ae::Sprite and ae::RectangleShape instances behave very similarly, but the
 main difference is that an ae::Sprite heavily depends on its texture as it
 also acts as the dimensions of the sprite. This results in a texture that
 never appears stretched or in any way deformed.

 \author Filippos Gleglakos
 \version v0.5.0
 \date 2020.08.07
 \copyright MIT License
*/