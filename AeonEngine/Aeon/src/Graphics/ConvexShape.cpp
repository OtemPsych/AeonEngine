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

#include <AEON/Graphics/ConvexShape.h>

namespace ae
{
	// Public constructor(s)
	ConvexShape::ConvexShape()
		: Shape()
		, mPoints()
	{
	}

	ConvexShape::ConvexShape(ConvexShape&& rvalue) noexcept
		: Shape(std::move(rvalue))
		, mPoints(std::move(rvalue.mPoints))
	{
	}

	// Public operator(s)
	ConvexShape& ConvexShape::operator=(ConvexShape&& rvalue) noexcept
	{
		// Move the rvalue's data
		Shape::operator=(std::move(rvalue));
		mPoints = std::move(rvalue.mPoints);

		return *this;
	}

	// Public method(s)
	void ConvexShape::addPoint(const Vector2f& point)
	{
		mPoints.emplace_back(point);
		mUpdatePositions = true;
	}

	void ConvexShape::setPoint(size_t index, const Vector2f& point)
	{
		// Check if the index provided is valid (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (index >= mPoints.size()) {
				AEON_LOG_WARNING("Invalid index", "The index provided is greater than the number of points added thus far.\nAborting operation.");
				return;
			}
		}

		// Modify the point's position
		mPoints[index] = point;
		mUpdatePositions = true;
	}

	// Public virtual method(s)
	size_t ConvexShape::getPointCount() const noexcept
	{
		return mPoints.size();
	}

	Vector2f ConvexShape::getPoint(size_t index) const
	{
		// Check if the index provided is valid (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (index >= mPoints.size()) {
				AEON_LOG_WARNING("Invalid index", "The index provided is greater than the number of points added thus far.\nReturning erroneous data.");
				return Vector2f();
			}
		}

		return mPoints[index];
	}
}