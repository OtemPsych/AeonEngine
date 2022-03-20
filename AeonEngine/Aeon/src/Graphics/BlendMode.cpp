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

#include <AEON/Graphics/BlendMode.h>

namespace ae
{
	// Public static member(s)
	const BlendMode BlendMode::BlendAlpha(Factor::SrcAlpha, Factor::OneMinusSrcAlpha, Equation::Add,
	                                      Factor::One, Factor::OneMinusSrcAlpha, Equation::Add);
	const BlendMode BlendMode::BlendAdd(Factor::SrcAlpha, Factor::One, Equation::Add,
	                                    Factor::One, Factor::One, Equation::Add);
	const BlendMode BlendMode::BlendMultiply(Factor::DstColor, Factor::Zero);
	const BlendMode BlendMode::BlendNone(Factor::One, Factor::Zero);

	// Public constructor(s)
	BlendMode::BlendMode() noexcept
		: colorSrcFactor(Factor::SrcAlpha)
		, colorDstFactor(Factor::OneMinusSrcAlpha)
		, colorEquation(Equation::Add)
		, alphaSrcFactor(Factor::One)
		, alphaDstFactor(Factor::OneMinusSrcAlpha)
		, alphaEquation(Equation::Add)
	{
	}

	BlendMode::BlendMode(Factor srcFactor, Factor dstFactor, Equation equation) noexcept
		: colorSrcFactor(srcFactor)
		, colorDstFactor(dstFactor)
		, colorEquation(equation)
		, alphaSrcFactor(srcFactor)
		, alphaDstFactor(dstFactor)
		, alphaEquation(equation)
	{
	}

	BlendMode::BlendMode(Factor colorSrcFactor, Factor colorDstFactor, Equation colorEquation,
	                     Factor alphaSrcFactor, Factor alphaDstFactor, Equation alphaEquation) noexcept
		: colorSrcFactor(colorSrcFactor)
		, colorDstFactor(colorDstFactor)
		, colorEquation(colorEquation)
		, alphaSrcFactor(alphaSrcFactor)
		, alphaDstFactor(alphaDstFactor)
		, alphaEquation(alphaEquation)
	{
	}

	// Public operator(s)
	bool BlendMode::operator==(const BlendMode& other) const noexcept
	{
		return (colorSrcFactor == other.colorSrcFactor) &&
		       (colorDstFactor == other.colorDstFactor) &&
		       (colorEquation == other.colorEquation)   &&
		       (alphaSrcFactor == other.alphaSrcFactor) &&
		       (alphaDstFactor == other.alphaDstFactor) &&
		       (alphaEquation == other.alphaEquation);
	}

	bool BlendMode::operator!=(const BlendMode& other) const noexcept
	{
		return !(*this == other);
	}

	bool BlendMode::operator<(const BlendMode& other) const noexcept
	{
		return colorSrcFactor + colorDstFactor < other.colorSrcFactor + other.colorDstFactor;
	}
}