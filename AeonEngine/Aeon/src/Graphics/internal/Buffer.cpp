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

#include <AEON/Graphics/internal/Buffer.h>

#include <string>

#include <GL/glew.h>

#include <AEON/Graphics/internal/GLCommon.h>

namespace ae
{
	// Public constructor(s)
	Buffer::Buffer(uint32_t target)
		: GLResource()
		, mBindingTarget(target)
	{
		// Create the OpenGL buffer
		GLCall(glCreateBuffers(1, &mHandle));
	}

	Buffer::Buffer(Buffer&& rvalue) noexcept
		: GLResource(std::move(rvalue))
		, mBindingTarget(rvalue.mBindingTarget)
	{
	}

	Buffer::~Buffer()
	{
	}

	// Public operator(s)
	Buffer& Buffer::operator=(Buffer&& rvalue) noexcept
	{
		// Copy the rvalue's trivial data and move the rest
		GLResource::operator=(std::move(rvalue));
		mBindingTarget = rvalue.mBindingTarget;

		return *this;
	}

	// Public method(s)
	void* Buffer::map(uint32_t access) const
	{
		return GLCall(glMapNamedBuffer(mHandle, access));
	}

	void* Buffer::mapRange(int offset, int length, uint32_t access) const
	{
		return GLCall(glMapNamedBufferRange(mHandle, offset, length, access));
	}

	void Buffer::unmap() const
	{
		GLCall(glUnmapNamedBuffer(mHandle));
	}

	// Public virtual method(s)
	void Buffer::destroy() const
	{
		// Check if the OpenGL handle is valid before attempting to delete it (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (!mHandle) {
				AEON_LOG_ERROR("Invalid OpenGL handle", "The buffer's OpenGL handle \"" + std::to_string(mHandle) + "\" is invalid.\nAborting destruction.");
				return;
			}
		}

		GLCall(glDeleteBuffers(1, &mHandle));
	}

	void Buffer::bind() const
	{
		// Check if the OpenGL handle is valid before attempting to bind it (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {	
			if (!mHandle) {
				AEON_LOG_ERROR("Invalid OpenGL handle", "The buffer's OpenGL handle \"" + std::to_string(mHandle) + "\" is invalid.\nAborting binding.");
				return;
			}
		}

		GLCall(glBindBuffer(mBindingTarget, mHandle));
	}

	void Buffer::unbind() const
	{
		GLCall(glBindBuffer(mBindingTarget, 0));
	}
}