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

#include <AEON/Graphics/Renderable2D.h>

namespace ae
{
	// Public constructor(s)
	Renderable2D::~Renderable2D()
	{
	}

	// Public method(s)
	const std::vector<Vertex2D>& Renderable2D::getVertices()
	{
		// Update the list of vertices if necessary before returning it
		updateVertices();
		return mVertices;
	}

	const std::vector<unsigned int>& Renderable2D::getIndices()
	{
		// Update the list of indices if necessary (should only be needed once to create the list)
		if (mUpdateIndices) {
			updateIndices();
			mUpdateIndices = false;
		}

		return mIndices;
	}

	// Protected constructor(s)
	Renderable2D::Renderable2D() noexcept
		: mVertices()
		, mIndices()
		, mUpdateIndices(true)
	{
	}

	// Private virtual method(s)
	void Renderable2D::updateVertices()
	{
	}

	void Renderable2D::updateIndices()
	{
	}
}