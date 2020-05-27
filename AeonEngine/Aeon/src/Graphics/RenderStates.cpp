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

#include <AEON/Graphics/RenderStates.h>

namespace ae
{
	// Public constructor(s)
	RenderStates::RenderStates() noexcept
		: blendMode(BlendMode::BlendAlpha)
		, transform(Matrix4f::identity())
		, texture(nullptr)
		, shader(nullptr)
	{
	}

	RenderStates::RenderStates(const BlendMode& blendMode) noexcept
		: blendMode(blendMode)
		, transform(Matrix4f::identity())
		, texture(nullptr)
		, shader(nullptr)
	{
	}

	RenderStates::RenderStates(const Matrix4f& transform) noexcept
		: blendMode(BlendMode::BlendAlpha)
		, transform(transform)
		, texture(nullptr)
		, shader(nullptr)
	{
	}

	RenderStates::RenderStates(const Texture2D& texture) noexcept
		: blendMode(BlendMode::BlendAlpha)
		, transform(Matrix4f::identity())
		, texture(&texture)
		, shader(nullptr)
	{
	}

	RenderStates::RenderStates(const Shader& shader) noexcept
		: blendMode(BlendMode::BlendAlpha)
		, transform(Matrix4f::identity())
		, texture(nullptr)
		, shader(&shader)
	{
	}

	RenderStates::RenderStates(const BlendMode& blendMode, const Matrix4f& transform, const Texture2D& texture, const Shader& shader) noexcept
		: blendMode(blendMode)
		, transform(transform)
		, texture(&texture)
		, shader(&shader)
	{
	}
}