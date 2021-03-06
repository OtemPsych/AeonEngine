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

#include <AEON/Graphics/internal/Glyph.h>

namespace ae
{
	// Public constructor(s)
	Glyph::Glyph() noexcept
		: textureRect()
		, bearing()
		, texture(nullptr)
		, advance(0)
	{
	}

	Glyph::Glyph(Glyph&& rvalue) noexcept
		: textureRect(std::move(rvalue.textureRect))
		, bearing(std::move(rvalue.bearing))
		, texture(rvalue.texture)
		, individualTexture(std::move(rvalue.individualTexture))
		, advance(rvalue.advance)
	{
	}

	// Public operator(s)
	Glyph& Glyph::operator=(Glyph&& rvalue) noexcept
	{
		// Copy the rvalue's trivial data and move the rest
		textureRect = std::move(rvalue.textureRect);
		bearing = std::move(rvalue.bearing);
		texture = rvalue.texture;
		individualTexture = std::move(rvalue.individualTexture);
		advance = rvalue.advance;

		return *this;
	}
}