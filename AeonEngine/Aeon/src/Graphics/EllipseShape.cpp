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

#include <AEON/Graphics/EllipseShape.h>

namespace ae
{
	// Public constructor(s)
	EllipseShape::EllipseShape(const Vector2f& radius, size_t pointCount)
		: Shape()
		, mRadius(radius)
		, mPointCount(pointCount)
	{
		updatePositions();
	}

	// Public method(s)
	void EllipseShape::setRadius(const Vector2f& radius) noexcept
	{
		mRadius = radius;
		updatePositions();
	}

	void EllipseShape::setRadius(float radiusX, float radiusY) noexcept
	{
		mRadius.x = radiusX;
		mRadius.y = radiusY;
		updatePositions();
	}

	void EllipseShape::setPointCount(size_t count) noexcept
	{
		mPointCount = count;
		updatePositions();
	}

	// Public virtual method(s)
	size_t EllipseShape::getPointCount() const noexcept
	{
		return mPointCount;
	}

	Vector2f EllipseShape::getPoint(size_t index) const
	{
		assert(index < getPointCount());

		const float ANGLE = index * 2.f * Math::pi() / mPointCount - Math::pi() / 2.f;
		return mRadius + Vector2f(Math::cos(ANGLE) * mRadius.x, Math::sin(ANGLE) * mRadius.y);
	}
}