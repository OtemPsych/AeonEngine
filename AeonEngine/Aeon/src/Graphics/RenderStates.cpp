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

#include <AEON/Graphics/RenderStates.h>

namespace ae
{
	// Public constructor(s)
	RenderStates::RenderStates() noexcept
		: blendMode(BlendMode::BlendAlpha)
		, transform(Matrix4f::identity())
		, texture(nullptr)
		, shader(nullptr)
		, dirty(false)
	{
	}

	RenderStates::RenderStates(const BlendMode& blendMode) noexcept
		: blendMode(blendMode)
		, transform(Matrix4f::identity())
		, texture(nullptr)
		, shader(nullptr)
		, dirty(false)
	{
	}

	RenderStates::RenderStates(const Matrix4f& transform) noexcept
		: blendMode(BlendMode::BlendAlpha)
		, transform(transform)
		, texture(nullptr)
		, shader(nullptr)
		, dirty(false)
	{
	}

	RenderStates::RenderStates(const Texture& texture) noexcept
		: blendMode(BlendMode::BlendAlpha)
		, transform(Matrix4f::identity())
		, texture(&texture)
		, shader(nullptr)
		, dirty(false)
	{
	}

	RenderStates::RenderStates(const Shader& shader) noexcept
		: blendMode(BlendMode::BlendAlpha)
		, transform(Matrix4f::identity())
		, texture(nullptr)
		, shader(&shader)
		, dirty(false)
	{
	}

	RenderStates::RenderStates(const BlendMode& blendMode, const Matrix4f& transform, const Texture& texture, const Shader& shader) noexcept
		: blendMode(blendMode)
		, transform(transform)
		, texture(&texture)
		, shader(&shader)
		, dirty(false)
	{
	}

	RenderStates::RenderStates(RenderStates&& rvalue) noexcept
		: blendMode(std::move(rvalue.blendMode))
		, transform(std::move(rvalue.transform))
		, texture(rvalue.texture)
		, shader(rvalue.shader)
		, dirty(rvalue.dirty)
	{
	}

	// Public operator(s)
	RenderStates& RenderStates::operator=(RenderStates&& rvalue) noexcept
	{
		// Copy the rvalue's trivial data and move the rest
		blendMode = std::move(rvalue.blendMode);
		transform = std::move(rvalue.transform);
		texture = rvalue.texture;
		shader = rvalue.shader;
		dirty = rvalue.dirty;

		return *this;
	}
}