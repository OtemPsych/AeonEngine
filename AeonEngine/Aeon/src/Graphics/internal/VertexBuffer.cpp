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

#include <AEON/Graphics/internal/VertexBuffer.h>

#include <cassert>

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
	void VertexBuffer::Layout::addElement(uint32_t type, int32_t count, bool normalized)
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
			assert(false);
		}

		// Add in the format element and update the stride
		mElements.push_back({ type, count, static_cast<uint32_t>(mStride), normalized });
		mStride += size * count;
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
	void VertexBuffer::setData(int64_t size, const void* data) const
	{
		GLCall(glNamedBufferData(mHandle, size, data, mUsage));
	}

	void VertexBuffer::setSubData(int64_t offset, int64_t size, const void* data) const
	{
		GLCall(glNamedBufferSubData(mHandle, offset, size, data));
	}
}