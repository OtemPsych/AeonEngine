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

#include <AEON/Graphics/RectangleShape.h>

namespace ae
{
	// Public constructor(s)
	RectangleShape::RectangleShape(const Vector2f& size, float cornerRadius, size_t cornerPointCount)
		: Shape()
		, mSize(size)
		, mCornerRadius(cornerRadius)
		, mCornerPointCount(cornerPointCount)
	{
		updatePositions();
	}

	// Public method(s)
	void RectangleShape::setSize(const Vector2f& size) noexcept
	{
		mSize = size;
		updatePositions();
	}

	void RectangleShape::setSize(float sizeX, float sizeY) noexcept
	{
		mSize.x = sizeX;
		mSize.y = sizeY;
		updatePositions();
	}

	void RectangleShape::setCornerRadius(float radius) noexcept
	{
		mCornerRadius = radius;
		updatePositions();
	}

	void RectangleShape::setCornerPointCount(size_t count) noexcept
	{
		mCornerPointCount = count;
		updatePositions();
	}

	// Public virtual method(s)
	size_t RectangleShape::getPointCount() const noexcept
	{
		return mCornerPointCount * 4;
	}

	Vector2f RectangleShape::getPoint(size_t index) const
	{
		assert(index < getPointCount());

		const float DELTA_ANGLE = 90.f / (Math::max(static_cast<int>(mCornerPointCount) - 1, 1));
		const size_t CENTER_INDEX = index / mCornerPointCount;
		const size_t DELTA_INDEX = index - CENTER_INDEX;
		const float FINAL_ANGLE = Math::radians(DELTA_ANGLE * DELTA_INDEX);

		Vector2f center;
		switch (CENTER_INDEX)
		{
		default:
		case 0:
			center = Vector2f(mSize.x - mCornerRadius, mCornerRadius);
			break;
		case 1:
			center = Vector2f(mCornerRadius,           mCornerRadius);
			break;
		case 2:
			center = Vector2f(mCornerRadius,           mSize.y - mCornerRadius);
			break;
		case 3:
			center = Vector2f(mSize.x - mCornerRadius, mSize.y - mCornerRadius);
		}

		return Vector2f( mCornerRadius * Math::cos(FINAL_ANGLE) + center.x,
		                -mCornerRadius * Math::sin(FINAL_ANGLE) + center.y);
	}
}