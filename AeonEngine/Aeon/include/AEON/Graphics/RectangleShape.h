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

#ifndef Aeon_Graphics_RectangleShape_H_
#define Aeon_Graphics_RectangleShape_H_

#include <AEON/Graphics/internal/Shape.h>

namespace ae
{
	/*!
	 \brief Class representing a 2D rectangle.
	*/
	class _NODISCARD AEON_API RectangleShape : public Shape
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::RectangleShape by providing an optional size, an optional corner radius and an optional corner point count.
		 \details Creates a rectangle with no texture and a white fill color.

		 \param[in] size The size of the rectangle, 0x0 by default
		 \param[in] cornerRadius The corner radius in degrees, 0 by default
		 \param[in] cornerPointCount The amount of points to add for each corner, 1 by default

		 \par Example:
		 \code
		 // Normal 50x25 rectangle
		 auto rectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 25.f));

		 // A 50x25 rectangle with rounded corners
		 auto roundedRectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 25.f), 20.f, 15);
		 \endcode

		 \since v0.5.0
		*/
		explicit RectangleShape(const Vector2f& size = Vector2f(), float cornerRadius = 0.f, size_t cornerPointCount = 1);
		/*!
		 \brief Deleted copy constructor.

		 \since v0.5.0
		*/
		RectangleShape(const RectangleShape&) = delete;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::RectangleShape that will be moved

		 \since v0.5.0
		*/
		RectangleShape(RectangleShape&& rvalue) noexcept;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.5.0
		*/
		RectangleShape& operator=(const RectangleShape&) = delete;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::RectangleShape that will be moved

		 \return The caller ae::RectangleShape

		 \since v0.5.0
		*/
		RectangleShape& operator=(RectangleShape&& rvalue) noexcept;
	public:
		// Public method(s)
		/*!
		 \brief Sets the size of the ae::RectangleShape.

		 \param[in] size The new size of the rectangle

		 \par Example:
		 \code
		 auto rectangle = std::make_unique<ae::RectangleShape>();
		 rectangle->setSize(ae::Vector2f(50.f, 25.f));
		 \endcode

		 \sa getSize()

		 \since v0.4.0
		*/
		void setSize(const Vector2f& size) noexcept;
		/*!
		 \brief Sets the size of the ae::RectangleShape.

		 \param[in] sizeX The new horizontal size of the rectangle
		 \param[in] sizeY The new vertical size of the rectangle

		 \par Example:
		 \code
		 auto rectangle = std::make_unique<ae::RectangleShape>();
		 rectangle->setSize(50.f, 25.f);
		 \endcode

		 \sa getSize()

		 \since v0.4.0
		*/
		void setSize(float sizeX, float sizeY) noexcept;
		/*!
		 \brief Sets the corner radius of the rounded corners of the ae::RectangleShape.
		 \note Must also modify the corner point count.

		 \param[in] radius The corner radius in degrees

		 \par Example:
		 \code
		 // Normal 50x25 rectangle
		 auto rectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 25.f));

		 // Set the corner radius to 20 degrees, must also set the corner point count
		 rectangle->setCornerRadius(20.f);
		 rectangle->setCornerPointCount(15);
		 \endcode

		 \sa setCornerPointCount(), getCornerRadius()

		 \since v0.5.0
		*/
		void setCornerRadius(float radius) noexcept;
		/*!
		 \brief Sets the number of points for each corner of the rounded ae::RectangleShape.
		 \note Must also modify the corner radius.

		 \param[in] count The number of points for each corner

		 \par Example:
		 \code
		 // Normal 50x25 rectangle
		 auto rectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 25.f));

		 // Set the corner point count to 15, must also set the corner radius
		 rectangle->setCornerPointCount(15);
		 rectangle->setCornerRadius(20.f);
		 \endcode

		 \sa setCornerRadius(), getCornerPointCount()

		 \since v0.5.0
		*/
		void setCornerPointCount(size_t count) noexcept;
		/*!
		 \brief Retrieves the size of the ae::RectangleShape.

		 \return The 2-dimensional ae::Vector containing the rectangle's size

		 \par Example:
		 \code
		 auto rectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 25.f));
		 ...
		 const ae::Vector2f& rectangleSize = rectangle->getSize();
		 \endcode

		 \sa setSize()

		 \since v0.4.0
		*/
		_NODISCARD const Vector2f& getSize() const noexcept;
		/*!
		 \brief Retrieves the corner radius of the rounded corners of the ae::RectangleShape.

		 \return The corner radius in degrees

		 \par Example:
		 \code
		 auto rectangle = std::make_unique<ae::RectangleShape>(ae::Vector2f(50.f, 25.f));
		 rectangle->setCornerRadius(25.f);
		 rectangle->setCornerPointCount(15);
		 ...
		 float cornerRadius = rectangle->getCornerRadius();
		 \endcode

		 \sa setCornerRadius(), setCornerPointCount()

		 \since v0.5.0
		*/
		_NODISCARD float getCornerRadius() const noexcept;

		// Public virtual method(s)
		/*!
		 \brief Retrieves the total number of points of the ae::RectangleShape.

		 \return The number of points of the ae::RectangleShape

		 \sa getPoint()

		 \since v0.5.0
		*/
		virtual size_t getPointCount() const noexcept override final;
		/*!
		 \brief Retrieves a point of the ae::RectangleShape by providing an index.

		 \param[in] index The index of the point to retrieve, situated within the range [0 ; getPointCount() - 1]

		 \return The 2-dimensional ae::Vector containing the point's position

		 \sa getPointCount()

		 \since v0.5.0
		*/
		virtual Vector2f getPoint(size_t index) const override final;

	private:
		// Private member(s)
		Vector2f mSize;             //!< The size of the rectangle
		float    mCornerRadius;     //!< The corner radius
		size_t   mCornerPointCount; //!< The number of corner points
	};
}
#endif // Aeon_Graphics_RectangleShape_H_

/*!
 \class ae::RectangleShape
 \ingroup graphics

 The ae::RectangleShape class is used to represent a 2D rectangle, instances of
 this class possess a size, a color and can hold an optional texture.

 The ae::RectangleShape can also have rounded corners so that it can be more
 visually pleasing.

 The ae::RectangleShape and ae::Sprite instances behave very similarly, but the
 main difference is that an ae::RectangleShape has an explicit size member
 which remains static even when the texture or the texture rect are modified.

 \author Filippos Gleglakos
 \version v0.5.0
 \date 2020.08.05
 \copyright MIT License
*/