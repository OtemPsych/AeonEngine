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

#ifndef Aeon_Graphics_ConvexShape_H_
#define Aeon_Graphics_ConvexShape_H_

#include <AEON/Graphics/internal/Shape.h>

namespace ae
{
	/*!
	 \brief Class representing a 2D convex polygon.
	*/
	class _NODISCARD AEON_API ConvexShape : public Shape
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details No points used to display the ae::ConvexShape.

		 \since v0.6.0
		*/
		ConvexShape();
		/*!
		 \brief Deleted copy constructor.
		 
		 \since v0.6.0
		*/
		ConvexShape(const ConvexShape&) = delete;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::ConvexShape that will be moved

		 \since v0.6.0
		*/
		ConvexShape(ConvexShape&& rvalue) noexcept;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.6.0
		*/
		ConvexShape& operator=(const ConvexShape&) = delete;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::ConvexShape that will be moved

		 \return The caller ae::ConvexShape

		 \since v0.6.0
		*/
		ConvexShape& operator=(ConvexShape&& rvalue) noexcept;
	public:
		// Public method(s)
		/*!
		 \brief Adds in a new point to represent the ae::ConvexShape.

		 \param[in] point The position of the new point

		 \par Example:
		 \code
		 auto convex = std::make_unique<ae::ConvexShape>();
		 convex->addPoint(ae::Vector2f(5.f, 0.f));
		 convex->addPoint(ae::Vector2f(0.f, 2.5f));
		 convex->addPoint(ae::Vector2f(5.f, 2.5f));
		 \endcode

		 \sa setPoint(), getPoint()

		 \since v0.6.0
		*/
		void addPoint(const Vector2f& point);
		/*!
		 \brief Modifies the position of an existing point of the ae::ConvexShape.

		 \param[in] index The index of the point to modify (0-based indexing)
		 \param[in] point The new position of the point associated at the index provided

		 \par Example:
		 \code
		 auto convex = std::make_unique<ae::ConvexShape>();
		 convex->addPoint(ae::Vector2f(5.f, 0.f));
		 convex->addPoint(ae::Vector2f(0.f, 2.5f));
		 convex->addPoint(ae::Vector2f(5.f, 2.5f));
		 ...
		 // Modify the first point added at (5, 0)
		 convex->setPoint(0, ae::Vector2f(15.f, 5.f));
		 \endcode

		 \sa addPoint(), getPoint()

		 \since v0.6.0
		*/
		void setPoint(size_t index, const Vector2f& point);

		// Public virtual method(s)
		/*!
		 \brief Retrieves the total number of points of the ae::ConvexShape.

		 \return The number of points representing the ae::ConvexShape

		 \par Example:
		 \code
		 auto convex = std::make_unique<ae::ConvexShape>();
		 ...
		 // Retrieve the number of points added
		 size_t pointCount = convex->getPointCount();
		 \endcode

		 \sa getPoint()

		 \since v0.6.0
		*/
		virtual size_t getPointCount() const noexcept override final;
		/*!
		 \brief Retrieves a point of the ae::ConvexShape by providing an index.

		 \param[in] index The index of the point to retrieve (0-based indexing)

		 \par Example:
		 \code
		 auto convex = std::make_unique<ae::ConvexShape>();
		 ...
		 // Retrieve the second point added
		 Vector2f secondPoint = convex->getPoint(1);
		 \endcode

		 \sa addPoint(), getPointCount()

		 \since v0.6.0
		*/
		virtual Vector2f getPoint(size_t index) const override final;

	private:
		// Private member(s)
		std::vector<Vector2f> mPoints; //!< The points representing the polygon
	};
}
#endif // Aeon_Graphics_ConvexShape_H_

/*!
 \class ae::ConvexShape
 \ingroup graphics

 The ae::ConvexShape class is used to represent a 2D convex shape containing an
 arbitrary number of points.

 The points need to be added in a counter-clockwise order.

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2020.08.15
 \copyright MIT License
*/