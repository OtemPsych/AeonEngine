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

#include <AEON/Graphics/RectangleShape.h>

namespace ae
{
	// Public constructor(s)
	RectangleShape::RectangleShape(const Vector2f& size)
		: Shape()
		, mSize(size)
	{
		// Create 4 vertices
		mVertices.resize(4);
	}

	// Public method(s)
	void RectangleShape::setSize(const Vector2f& size) noexcept
	{
		mSize = size;
		mUpdatePosition = true;
	}

	void RectangleShape::setSize(float sizeX, float sizeY) noexcept
	{
		mSize.x = sizeX;
		mSize.y = sizeY;
		mUpdatePosition = true;
	}

	const Vector2f& RectangleShape::getSize() const noexcept
	{
		return mSize;
	}

	// Private virtual method(s)
	void RectangleShape::updatePosition()
	{
		// Check if there are sufficient vertices (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (mVertices.empty()) {
				AEON_LOG_ERROR("Insufficient vertex count", "Unable to update positions due to empty vertex array.");
				return;
			}
		}

		// Update the positions
		const Vector2f& origin = getOrigin();
		mVertices[0].position = Vector2f(0.f,     0.f)     - origin;
		mVertices[1].position = Vector2f(0.f,     mSize.y) - origin;
		mVertices[2].position = Vector2f(mSize.x, mSize.y) - origin;
		mVertices[3].position = Vector2f(mSize.x, 0.f)     - origin;

		// Update the model bounding box's position and size based on the minimum and maximum vertex positions
		mModelBounds = Box2f(mVertices[0].position, mVertices[2].position - mVertices[0].position);
	}

	void RectangleShape::updateUV()
	{
		// Check if there are sufficient vertices (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (mVertices.empty()) {
				AEON_LOG_ERROR("Insufficient vertex count", "Unable to update uv coordinates due to empty vertex array.");
				return;
			}
		}

		// Update the UV coordinates
		const Texture2D* const texture = getTexture();
		const Box2f& rect = getTextureRect();

		const Vector2f TEXTURE_SIZE = (texture) ? Vector2f(texture->getSize()) : Vector2f(1.f, 1.f);
		mVertices[0].uv = Vector2f(rect.min.x,              rect.min.y + rect.max.y) / TEXTURE_SIZE;
		mVertices[1].uv = Vector2f(rect.min.x,              rect.min.y)              / TEXTURE_SIZE;
		mVertices[2].uv = Vector2f(rect.min.x + rect.max.x, rect.min.y)              / TEXTURE_SIZE;
		mVertices[3].uv = Vector2f(rect.min.x + rect.max.x, rect.min.y + rect.max.y) / TEXTURE_SIZE;
	}

	void RectangleShape::updateIndices()
	{
		mIndices = {
			0, 1, 2,
			0, 2, 3
		};
	}
}