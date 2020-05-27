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

#include <AEON/Graphics/Color.h>

namespace ae
{
	// Public static member(s)
	const Color Color::Black(0, 0, 0);
	const Color Color::White(255, 255, 255);
	const Color Color::Red(255, 0, 0);
	const Color Color::Green(0, 255, 0);
	const Color Color::Blue(0, 0, 255);
	const Color Color::Yellow(255, 255, 0);
	const Color Color::Magenta(255, 0, 255);
	const Color Color::Cyan(0, 255, 255);
	const Color Color::Transparent(0, 0, 0, 0);

	// Public constructor(s)
	Color::Color() noexcept
		: r(0)
		, g(0)
		, b(0)
		, a(255)
	{
	}

	Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) noexcept
		: r(red)
		, g(green)
		, b(blue)
		, a(alpha)
	{
	}

	Color::Color(uint32_t hexcode) noexcept
		: r((hexcode & 0xff000000) >> 24)
		, g((hexcode & 0x00ff0000) >> 16)
		, b((hexcode & 0x0000ff00) >> 8)
		, a((hexcode & 0x000000ff) >> 0)
	{
	}

	// Public operator(s)
	Color Color::operator+(const Color& other) const noexcept
	{
		return Color(
			static_cast<uint8_t>(Math::min(static_cast<int>(r) + static_cast<int>(other.r), 255)),
			static_cast<uint8_t>(Math::min(static_cast<int>(g) + static_cast<int>(other.g), 255)),
			static_cast<uint8_t>(Math::min(static_cast<int>(b) + static_cast<int>(other.b), 255)),
			static_cast<uint8_t>(Math::min(static_cast<int>(a) + static_cast<int>(other.a), 255))
		);
	}

	Color Color::operator-(const Color& other) const noexcept
	{
		return Color(
			static_cast<uint8_t>(Math::max(static_cast<int>(r) - static_cast<int>(other.r), 0)),
			static_cast<uint8_t>(Math::max(static_cast<int>(g) - static_cast<int>(other.g), 0)),
			static_cast<uint8_t>(Math::max(static_cast<int>(b) - static_cast<int>(other.b), 0)),
			static_cast<uint8_t>(Math::max(static_cast<int>(a) - static_cast<int>(other.a), 0))
		);
	}

	Color Color::operator*(const Color& other) const noexcept
	{
		return Color(
			static_cast<uint8_t>(static_cast<int>(r) * static_cast<int>(other.r) / 255),
			static_cast<uint8_t>(static_cast<int>(g) * static_cast<int>(other.g) / 255),
			static_cast<uint8_t>(static_cast<int>(b) * static_cast<int>(other.b) / 255),
			static_cast<uint8_t>(static_cast<int>(a) * static_cast<int>(other.a) / 255)
		);
	}

	Color& Color::operator+=(const Color& other) noexcept
	{
		return (*this = *this + other);
	}

	Color& Color::operator-=(const Color& other) noexcept
	{
		return (*this = *this - other);
	}

	Color& Color::operator*=(const Color& other) noexcept
	{
		return (*this = *this * other);
	}

	bool Color::operator==(const Color& other) const noexcept
	{
		return (r == other.r) &&
		       (g == other.g) &&
		       (b == other.b) &&
		       (a == other.a);
	}

	bool Color::operator!=(const Color& other) const noexcept
	{
		return !(*this == other);
	}

	// Public method(s)
	uint32_t Color::toHexcode() const noexcept
	{
		return ((r << 24) | (g << 16) | (b << 8) | a);
	}

	Vector4f Color::normalize() const noexcept
	{
		return Vector4f(static_cast<float>(r) / 255.f,
		                static_cast<float>(g) / 255.f,
		                static_cast<float>(b) / 255.f,
		                static_cast<float>(a) / 255.f);
	}
}