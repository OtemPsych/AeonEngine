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

#include <AEON/Graphics/internal/Shape.h>

namespace ae
{
	/*!
	 \brief Class representing a 2D ellipse or circle.
	*/
	class AEON_API EllipseShape : public Shape
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::EllipseShape by providing an optional radius along the X and Y axes, and an optional point count.
		 \details Creates an ellipse with no texture and a white fill color.

		 \param[in] radius The horizontal and vertical radii of the ellipse, (0, 0) by default
		 \param[in] pointCount The amount of points to add to form the ellipse, 30 by default

		 \par Example:
		 \code
		 // A circle with a radius of 5
		 auto circle = std::make_unique<ae::EllipseShape>(ae::Vector2f(5.f, 5.f));

		 // An ellipse with a radius of 10 along the X axis and a radius of 5 along the Y axis
		 auto ellipse = std::make_unique<ae::EllipseShape>(ae::Vector2f(10.f, 5.f));
		 \endcode

		 \since v0.7.0
		*/
		explicit EllipseShape(const Vector2f& radius = Vector2f(), size_t pointCount = 30);
		/*!
		 \brief Copy constructor.

		 \since v0.5.0
		*/
		EllipseShape(const EllipseShape&) = default;
		/*!
		 \brief Move constructor.

		 \since v0.7.0
		*/
		EllipseShape(EllipseShape&&) noexcept = default;
		/*!
		 \brief Virtual destructor.
		 \details This is needed as the API user may derive from this class.

		 \since v0.7.0
		*/
		virtual ~EllipseShape() = default;

		// Public operator(s)
		/*!
		 \brief Assignment operator.

		 \since v0.5.0
		*/
		EllipseShape& operator=(const EllipseShape&) = default;
		/*!
		 \brief Move assignment operator.

		 \return The caller ae::EllipseShape

		 \since v0.7.0
		*/
		EllipseShape& operator=(EllipseShape&&) noexcept = default;

		// Public method(s)
		/*!
		 \brief Sets the radii of the ae::EllipseShape.

		 \param[in] radius The horizontal and vertical radii of the ae::EllipseShape

		 \par Example:
		 \code
		 auto ellipse = std::make_unique<ae::EllipseShape>();
		 ellipse->setRadius(ae::Vector2f(50.f, 25.f));
		 \endcode

		 \sa getRadius()

		 \since v0.7.0
		*/
		void setRadius(const Vector2f& radius) noexcept;
		/*!
		 \brief Sets the radii of the ae::EllipseShape.

		 \param[in] radiusX The radius along the X axis
		 \param[in] radiusY The radius along the Y axis

		 \par Example:
		 \code
		 auto ellipse = std::make_unique<ae::EllipseShape>();
		 ellipse->setRadius(50.f, 25.f);
		 \endcode

		 \sa getRadius()

		 \since v0.7.0
		*/
		void setRadius(float radiusX, float radiusY) noexcept;
		/*!
		 \brief Sets the number of points used to form the ae::EllipseShape.

		 \param[in] count The number of points to use

		 \par Example:
		 \code
		 // Circle with 30 points
		 auto circle = std::make_unique<ae::EllipseShape>(ae::Vector2f(50.f, 50.f));

		 // Set the point count to 120
		 circle->setPointCount(120);
		 \endcode

		 \sa getPointCount()

		 \since v0.7.0
		*/
		void setPointCount(size_t count) noexcept;
		/*!
		 \brief Retrieves the radii of the ae::EllipseShape.

		 \return The 2-dimensional ae::Vector containing the ellipse's radii

		 \par Example:
		 \code
		 auto ellipse = std::make_unique<ae::EllipseShape>(ae::Vector2f(50.f, 25.f));
		 ...
		 const ae::Vector2f& radii = ellipse->getRadius();
		 \endcode

		 \sa setRadius()

		 \since v0.7.0
		*/
		[[nodiscard]] const Vector2f& getRadius() const noexcept { return mRadius; }

		// Public virtual method(s)
		/*!
		 \brief Retrieves the total number of points used to form the ae::EllipseShape.

		 \return The number of points used to form the ellipse

		 \sa setPointCount(), getPoint()

		 \since v0.5.0
		*/
		virtual size_t getPointCount() const noexcept override final;
		/*!
		 \brief Retrieves a point of the ae::EllipseShape by providing an index.

		 \param[in] index The index of the point to retrieve, situated within the range [0 ; getPointCount() - 1]

		 \return The 2-dimensional ae::Vector containing the point's position

		 \sa getPointCount()

		 \since v0.5.0
		*/
		virtual Vector2f getPoint(size_t index) const override final;

	private:
		// Private member(s)
		Vector2f mRadius;     //!< The radius of the ellipse along the X and Y axes
		size_t   mPointCount; //!< The number of points used to form the ellipse
	};
}

/*!
 \class ae::EllipseShape
 \ingroup graphics
 
 The ae::EllipseShape class is used to represent a 2D ellipse or circle,
 instances of this class possess a horizontal radius, a vertical radius, a
 color and can hold an optional texture.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2022.01.02
 \copyright MIT License
*/