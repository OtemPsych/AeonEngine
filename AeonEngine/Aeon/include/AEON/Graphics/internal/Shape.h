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

#ifndef Aeon_Graphics_Shape_H_
#define Aeon_Graphics_Shape_H_

#include <AEON/Graphics/Actor2D.h>

namespace ae
{
	/*!
	 \brief Abstract base class representing 2D shapes.
	 \note No direct instance of this class can be created.
	*/
	class AEON_API Shape : public Actor2D
	{
	public:
		// Public method(s)
		/*!
		 \brief Sets the texture to be applied to the ae::Shape.
		 \details The texture will be made to fit within the size constraints assigned to the ae::Shape.
		 \note If no texture rect was assigned beforehand, the entire texture will be used.

		 \param[in] texture The texture to apply to the rectangle
		 \param[in] resetRect True to use the entire texture, false to keep the current texture rect

		 \par Example:
		 \code
		 // Create a 50x50 rectangle
		 auto rectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 50.f));

		 // Assign a 250x250 texture, texture shrinked to fit
		 // Size: 50x50 ; Texture Rect: (0, 0, 250, 250)
		 rectangle->setTexture(someTexture);

		 ...

		 // Assign a 500x500 texture whilst keeping the previous texture rect, texture shrinked to fit
		 // Size: 50x50 ; Texture Rect: (0, 0, 250, 250)
		 rectangle->setTexture(otherTexture);

		 ...

		 // Assign a 32x32 texture and setting the new texture rect, texture stretched to fit
		 // Size: 50x50 ; Texture Rect: (0, 0, 32, 32)
		 rectangle->setTexture(otherOtherTexture, true);
		 \endcode

		 \sa setTextureRect()

		 \since v0.4.0
		*/
		void setTexture(const Texture2D& texture, bool resetRect = false);
		/*!
		 \brief Sets the ae::Shape's texture rect.
		 \details The texture rect represents the area of the assigned texture to display.
		 \note A texture has to be assigned for the texture rect to have any effect.

		 \param[in] rect The ae::Box2f containing the starting position and the size of the area of the texture

		 \par Example:
		 \code
		 // Create a 50x50 rectangle
		 auto rectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 50.f));

		 // Assign a 250x250 texture
		 // Texture Rect: (0, 0, 250, 250)
		 rectangle->setTexture(someTexture);

		 // Set the texture rect starting position at (50, 50) and a size of (100, 100)
		 rectangle->setTextureRect(ae::Box2f(50.f, 50.f, 100.f, 100.f));
		 \endcode

		 \sa setTexture()

		 \since v0.4.0
		*/
		void setTextureRect(const Box2f& rect);
		/*!
		 \brief Sets the fill color of the ae::Shape.
		 \details The fill color refers to the color used to fill the interior of the shape.

		 \param[in] color The new fill ae::Color of the shape

		 \par Example:
		 \code
		 auto rectangle = std::make_unique<ae::RectangleShape>();
		 rectangle->setFillColor(ae::Color::Cyan);
		 \endcode

		 \sa getFillColor()

		 \since v0.4.0
		*/
		void setFillColor(const Color& color) noexcept;
		/*!
		 \brief Retrieves the ae::Shape's assigned texture.
		 \note If no texture was assigned, nullptr will be returned.

		 \return Pointer to the assigned texture or nullptr if no texture was assigned

		 \par Example:
		 \code
		 auto rectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 50.f));
		 rectangle->setTexture(someTexture);

		 ...

		 const Texture2D* const texture = rectangle->getTexture();
		 \endcode

		 \sa setTexture(), getTextureRect()

		 \since v0.4.0
		*/
		_NODISCARD const Texture2D* const getTexture() const noexcept;
		/*!
		 \brief Retrieves the ae::Shape's texture rect.
		 \details The texture rect represents the area of the assigned texture to display.
		 \note A texture has to be assigned for the texture rect to have any effect.

		 \return The ae::Box2f containing the starting position and the size of the area of the texture

		 \par Example:
		 \code
		 auto rectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 50.f));
		 rectangle->setTexture(someTexture);

		 const ae::Box2f& textureRect = rectangle->getTextureRect();
		 \endcode

		 \sa setTextureRect(), getTexture()

		 \since v0.4.0
		*/
		_NODISCARD const Box2f& getTextureRect() const noexcept;
		/*!
		 \brief Retrieves the fill color of the ae::Shape.
		 \details The fill colors refers to the color used to fill the interior of the shape.

		 \return The ae::Color used for the shape's fill color

		 \par Example:
		 \code
		 auto rectangle = std::make_unique<ae::RectangleShape>();
		 rectangle->setFillColor(ae::Color::Cyan);
		 ...
		 const ae::Color& fillColor = rectangle->getFillColor();
		 \endcode

		 \sa setFillColor()

		 \since v0.4.0
		*/
		_NODISCARD const Color& getFillColor() const noexcept;

		// Public virtual method(s)
		/*!
		 \brief Sets the ae::Shape's local origin (or anchor point).
		 \details An origin of (0, 0) places the origin at the top left part of the shape.

		 \param[in] origin A 2-dimensional ae::Vector containing the local origin, (0, 0) by default

		 \par Example:
		 \code
		 // Place the origin at the center of the rectangle, can also be achieved with origin flags
		 auto rectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 50.f));
		 rectangle->setOrigin(sprite->getModelBounds().max / 2.f);
		 \endcode

		 \since v0.4.0
		*/
		virtual void setOrigin(const Vector2f& origin) noexcept override final;
		/*!
		 \brief Retrieves the ae::Shape's model bounding box.

		 \return An ae::Box2f containing the model bounding box.

		 \since v0.4.0
		*/
		_NODISCARD virtual Box2f getModelBounds() const override final;
	protected:
		// Protected constructor(s)
		/*!
		 \brief Default constructor.
		 \note No direct instance of this class can be created.

		 \since v0.4.0
		*/
		Shape();
		/*!
		 \brief Copy constructor.

		 \param[in] copy The ae::Shape that will be copied

		 \since v0.4.0
		*/
		Shape(const Shape& copy) = default;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Shape that will be moved

		 \since v0.4.0
		*/
		Shape(Shape&& rvalue) = default;
	protected:
		// Protected operator(s)
		/*!
		 \brief Assignment operator.

		 \param[in] other The ae::Shape that will be copied

		 \since v0.4.0
		*/
		Shape& operator=(const Shape& other) = default;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Shape that will be moved

		 \since v0.4.0
		*/
		Shape& operator=(Shape&& rvalue) = default;
	protected:
		// Protected virtual method(s)
		/*!
		 \brief Updates the stored vertices' positions.
		 \details Called when the vertices were attempted to be retrieved and the size/radius had changed.
		 \note Must be defined by derived classes.

		 \sa updateUV(), updateFillColor(), updateVertices()

		 \since v0.4.0
		*/
		virtual void updatePosition() = 0;
		/*!
		 \brief Updates the stored vertices' uv coordinates.
		 \details Called when the vertices were attempted to be retrieved and the texture or the texture rect had changed.
		 \note Must be defined by derived classes.

		 \sa updatePosition(), updateFillColor(), updateVertices()

		 \since v0.4.0
		*/
		virtual void updateUV() = 0;
	private:
		// Private method(s)
		/*!
		 \brief Updates the stored vertices' color.
		 \details Called when the vertices were attempted to be retrieved and the fill color had changed.

		 \sa updateUV(), updatePosition(), updateVertices()

		 \since v0.4.0
		*/
		void updateFillColor();

		// Private virtual method(s)
		/*!
		 \brief Sends the vertex data and render states to the renderer.
		 \details Sets the appropriate shader, blend mode and texture.

		 \param[in] states The ae::RenderStates defining the OpenGL state

		 \return True if any other ae::Actor2D objects should be allowed to render themselves and their children, false otherwise

		 \since v0.4.0
		*/
		_NODISCARD virtual bool renderSelf(RenderStates states) override final;
		/*!
		 \brief Updates the ae::Shape's stored vertices.
		 \details Calls the derived classes' definition of the update methods.

		 \sa updatePosition(), updateUV(), updateFillColor()

		 \since v0.4.0
		*/
		virtual void updateVertices() override final;

	protected:
		// Protected member(s)
		Box2f            mModelBounds;     //!< The local model bounds of the shape
		bool             mUpdatePosition;  //!< Whether the vertices' positions need to be updated
	private:
		// Private member(s)
		Box2f            mTextureRect;     //!< The texture rectangle containing the texture coordinates
		Color            mFillColor;       //!< The fill color of the shape
		const Texture2D* mTexture;         //!< The optional texture to assign to the shape
		bool             mUpdateUV;        //!< Whether the vertices' texture coordinates need to be updated
		bool             mUpdateFillColor; //!< Whether the vertices' fill color needs to be updated
	};
}
#endif // Aeon_Graphics_Shape_H_

/*!
 \class ae::Shape
 \ingroup graphics

 The ae::Shape abstract base class is used to represent a 2-dimensional shape;
 while this class is considered internal and the API user shouldn't have any
 direct need of it, its derived classes will come in very handy to display
 geometrical shapes such as rectangles, circles and general convex shapes.

 \author Filippos Gleglakos
 \version v0.4.0
 \date 2020.05.26
 \copyright MIT License
*/