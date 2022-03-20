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

namespace ae
{
	// Forward declaration(s)
	class Texture2D;

	/*!
	 \brief Abstract base class representing 2D shapes.
	 \note No direct instance of this class can be created.
	*/
	class AEON_API Shape : public Actor
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Virtual destructor.
		 \details A virtual destructor is needed as this class will be inherited.

		 \since v0.7.0
		*/
		virtual ~Shape() = default;

		// Public method(s)
		/*!
		 \brief Sets the texture to be applied to the ae::Shape.
		 \details The texture will be made to fit within the size constraints assigned to the ae::Shape.
		 \note If no texture rect was assigned beforehand, the entire texture will be used.

		 \param[in] texture The texture to apply to the rectangle, nullptr to remove the current texture
		 \param[in] resetRect True to use the entire texture, false to keep the current texture rect

		 \par Example:
		 \code
		 // Create a 50x50 rectangle
		 auto rectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 50.f));

		 // Assign a 250x250 texture, texture shrinked to fit
		 // Size: 50x50 ; Texture Rect: (0, 0, 250, 250)
		 rectangle->setTexture(&someTexture);

		 ...

		 // Assign a 500x500 texture whilst keeping the previous texture rect, part of the texture shown
		 // Size: 50x50 ; Texture Rect: (0, 0, 250, 250)
		 rectangle->setTexture(&otherTexture);

		 ...

		 // Assign a 32x32 texture and setting the new texture rect, texture stretched to fit
		 // Size: 50x50 ; Texture Rect: (0, 0, 32, 32)
		 rectangle->setTexture(&otherOtherTexture, true);
		 \endcode

		 \sa setTextureRect()

		 \since v0.7.0
		*/
		void setTexture(const Texture2D* const texture, bool resetRect = false);
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
		 rectangle->setTexture(&someTexture);

		 // Set the texture rect starting position at (50, 50) and a size of (100, 100)
		 rectangle->setTextureRect(ae::Box2f(50.f, 50.f, 100.f, 100.f));
		 \endcode

		 \sa setTexture()

		 \since v0.7.0
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

		 \since v0.7.0
		*/
		void setFillColor(const Color& color) noexcept;
		/*!
		 \brief Sets the outline color of the ae::Shape.
		 \details The outline color refers to the color used by the shape's border.
		 \note The outline will only be rendered if its thickness is not equal to 0.

		 \param[in] color The new outline ae::Color of the shape

		 \par Example:
		 \code
		 auto rectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 50.f));
		 rectangle->setOutlineThickness(2.f);
		 rectangle->setOutlineColor(ae::Color::Black);
		 \endcode

		 \sa getOutlineColor(), setOutlineThickness()

		 \since v0.7.0
		*/
		void setOutlineColor(const Color& color) noexcept;
		/*!
		 \brief Sets the outline thickness of the ae::Shape.
		 \details The outline thickness is used to determine the size of the border surrounding the shape.
		 \note The outline thickness musn't be equal to 0 in order for it to be rendered.

		 \param[in] thickness The distance from the primary shape determining the size of the shape's border

		 \par Example:
		 \code
		 auto rectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 50.f));
		 rectangle->setOutlineThickness(2.f);
		 \endcode

		 \sa getOutlineThickness()

		 \since v0.7.0
		*/
		void setOutlineThickness(float thickness) noexcept;
		/*!
		 \brief Retrieves the assigned texture.
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

		 \since v0.7.0
		*/
		[[nodiscard]] inline const Texture2D* const getTexture() const noexcept { return mTexture; }
		/*!
		 \brief Retrieves the texture rect.
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

		 \since v0.7.0
		*/
		[[nodiscard]] inline const Box2f& getTextureRect() const noexcept { return mTextureRect; }
		/*!
		 \brief Retrieves the fill color.
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

		 \since v0.7.0
		*/
		[[nodiscard]] inline const Color& getFillColor() const noexcept { return mFillColor; }
		/*!
		 \brief Retrieves the outline color.
		 \details The outline color refers to the color used by the shape's border.

		 \return The ae::Color used for the the shape's outline color

		 \par Example:
		 \code
		 auto rectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 50.f));
		 rectangle->setOutlineThickness(2.f);
		 rectangle->setOutlineColor(ae::Color::Black);
		 ...
		 const ae::Color& outlineColor = rectangle->getOutlineColor();
		 \endcode

		 \sa setOutlineColor(), setOutlineThickness()

		 \since v0.7.0
		*/
		[[nodiscard]] inline const Color& getOutlineColor() const noexcept { return mOutlineColor; }
		/*!
		 \brief Retrieves the outline thickness.
		 \details The outline thickness is used to determine the size of the border surrounding the shape.

		 \return The outline thickness of the ae::Shape

		 \par Example:
		 \code
		 auto rectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 50.f));
		 rectangle->setOutlineThickness(2.f);
		 ...
		 float outlineThickness = rectangle->getOutlineThickness();
		 \endcode

		 \sa setOutlineThickness()

		 \since v0.7.0
		*/
		[[nodiscard]] inline float getOutlineThickness() const noexcept { return mOutlineThickness; }

		// Public virtual method(s)
		/*!
		 \brief Retrieves the total number of points of the shape.
		 \note Must be defined by derived classes.

		 \return The number of points of the shape

		 \sa getPoint()

		 \since v0.5.0
		*/
		virtual size_t getPointCount() const noexcept = 0;
		/*!
		 \brief Retrieves a point of the shape by providing an index.
		 \note Must be defined by derived classes.

		 \param[in] index The index of the point to retrieve, situated within the range [0 ; getPointCount() - 1]

		 \return The 2-dimensional ae::Vector containing the point's position

		 \sa getPointCount()

		 \since v0.5.0
		*/
		virtual Vector2f getPoint(size_t index) const = 0;
	protected:
		// Protected constructor(s)
		/*!
		 \brief Default constructor.
		 \note No direct instance of this class can be created.

		 \since v0.7.0
		*/
		Shape();
		/*!
		 \brief Copy constructor.

		 \param[in] copy The ae::Shape that will be copied

		 \since v0.7.0
		*/
		Shape(const Shape& copy);
		/*!
		 \brief Move constructor.

		 \since v0.7.0
		*/
		Shape(Shape&&) noexcept = default;

		// Protected operator(s)
		/*!
		 \brief Assignment operator.

		 \return The caller ae::Shape

		 \since v0.6.0
		*/
		Shape& operator=(const Shape&) = default;
		/*!
		 \brief Move assignment operator.

		 \return The caller ae::Shape

		 \since v0.7.0
		*/
		Shape& operator=(Shape&&) noexcept = default;

		// Protected method(s)
		/*!
		 \brief Updates the stored vertices' positions and the stored indices.
		 \details Called when the size/radius has changed.

		 \sa updateUVs(), updateFillColors(), updateShape()

		 \since v0.7.0
		*/
		void updatePositions();

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
		 \brief Updates the stored vertices' uv coordinates.
		 \details Called when the texture or the texture rect has changed.

		 \sa updatePositions(), updateFillColors(), updateShape()

		 \since v0.5.0
		*/
		void updateUVs();
		/*!
		 \brief Updates the stored vertices' color.
		 \details Called when the fill color has changed.

		 \sa updateUVs(), updatePositions(), updateShape()

		 \since v0.5.0
		*/
		void updateFillColors();
		/*!
		 \brief Updates the stored outline vertices' positions and the stored outline indices.
		 \details Called when the size/radius has changed.
		 \note Only called when the outline thickness is not equal to 0.

		 \sa updateOutlineColors(), updateOutline()

		 \since v0.7.0
		*/
		void updateOutlinePositions();
		/*!
		 \brief Updates the stored outline vertices' colors.
		 \details Called when the outline color has changed.
		 \note Only called when the outline thickness is not equal to 0.

		 \sa updateOutlinePositions(), updateOutline()

		 \since v0.5.0
		*/
		void updateOutlineColors();

	private:
		// Private member(s)
		std::vector<Vertex2D> mOutlineVertices;  //!< The list of outline vertices
		std::vector<uint32_t> mOutlineIndices;   //!< The list of outline indices
		Box2f                 mInnerBounds;      //!< The inner model bounding box (without the outline)
		Box2f                 mTextureRect;      //!< The texture rectangle containing the texture coordinates
		Color                 mFillColor;        //!< The fill color of the shape
		Color                 mOutlineColor;     //!< The outline color of the shape
		const Texture2D*      mTexture;          //!< The optional texture to assign to the shape
		float                 mOutlineThickness; //!< The outline's thickness
	};
}

/*!
 \class ae::Shape
 \ingroup graphics

 The ae::Shape abstract base class is used to represent a 2-dimensional shape;
 while this class is considered internal and the API user shouldn't have any
 direct need of it, its derived classes will come in very handy to display
 geometrical shapes such as rectangles, circles and general convex shapes.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.12.31
 \copyright MIT License
*/