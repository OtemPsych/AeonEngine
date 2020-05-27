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

#include <AEON/Graphics/internal/VertexBuffer.h>

#include <GL/glew.h>

#include <AEON/Graphics/internal/GLCommon.h>

namespace ae
{
	// VertexBuffer::Layout
		// Public constructor(s)
	VertexBuffer::Layout::Layout() noexcept
		: mElements()
		, mStride(0)
	{
	}

		// Public method(s)
	void VertexBuffer::Layout::addElement(uint32_t type, int count, bool normalized)
	{
		// Calculate the OpenGL type in bytes
		GLsizei size = 0;
		switch (type)
		{
		case GL_FLOAT:
			size = sizeof(GLfloat);
			break;
		case GL_UNSIGNED_INT:
			size = sizeof(GLuint);
			break;
		case GL_INT:
			size = sizeof(GLint);
			break;
		case GL_UNSIGNED_BYTE:
			size = sizeof(GLubyte);
			break;
		case GL_BYTE:
			size = sizeof(GLbyte);
			break;
		case GL_UNSIGNED_SHORT:
			size = sizeof(GLushort);
			break;
		case GL_SHORT:
			size = sizeof(GLshort);
			break;
		case GL_DOUBLE:
			size = sizeof(GLdouble);
			break;
		default:
			// Generate an error log if the type provided is invalid (ignored in Release mode)
			if _CONSTEXPR_IF (AEON_DEBUG) {
				AEON_LOG_ERROR("Invalid type provided", "The OpenGL type provided is invalid.\nNo effect.");
			}
			return;
		}

		// Add in the format element and update the stride
		mElements.push_back({ type, count, static_cast<GLuint>(mStride), normalized });
		mStride += size * count;
	}

	const std::vector<VertexBuffer::Layout::Element>& VertexBuffer::Layout::getElements() const noexcept
	{
		return mElements;
	}

	int VertexBuffer::Layout::getStride() const noexcept
	{
		return mStride;
	}

	// VertexBuffer
		// Public constructor(s)
	VertexBuffer::VertexBuffer(uint32_t usage)
		: Buffer(GL_ARRAY_BUFFER)
		, mLayout()
		, mUsage(usage)
	{
	}

		// Public method(s)
	void VertexBuffer::setData(int size, const void* data) const
	{
		GLCall(glNamedBufferData(mHandle, size, data, mUsage));
	}

	void VertexBuffer::setSubData(int offset, int size, const void* data) const
	{
		GLCall(glNamedBufferSubData(mHandle, offset, size, data));
	}

	VertexBuffer::Layout& VertexBuffer::getLayout() noexcept
	{
		return mLayout;
	}
}