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

#include <AEON/Graphics/internal/VertexArray.h>

#include <GL/glew.h>

#include <AEON/Graphics/internal/GLCommon.h>

namespace ae
{
	// Public constructor(s)
	VertexArray::VertexArray()
		: GLResource()
		, mVBOs()
		, mAttributeIndex(0)
	{
		// Create the VAO
		GLCall(glCreateVertexArrays(1, &mHandle));
	}

	// Public method(s)
	void VertexArray::addBuffer(std::unique_ptr<VertexBuffer> vbo, unsigned int divisor)
	{
		// Retrieve the VBO's data layout
		const unsigned int BINDING_INDEX = static_cast<unsigned int>(mVBOs.size());
		const VertexBuffer::Layout& layout = vbo->getLayout();
		const std::vector<VertexBuffer::Layout::Element>& layoutElements = layout.getElements();

		// Apply the VBO's data format and link the vertex attributes with the VBO
		for (const VertexBuffer::Layout::Element& element : layoutElements) {
			GLCall(glVertexArrayAttribFormat(mHandle, mAttributeIndex, element.count, element.type, element.normalized, element.offset));
			GLCall(glVertexArrayAttribBinding(mHandle, mAttributeIndex, BINDING_INDEX));
			GLCall(glEnableVertexArrayAttrib(mHandle, mAttributeIndex));
			++mAttributeIndex;
		}

		// Set the divisor for instanced rendering
		if (divisor != 0) {
			GLCall(glVertexArrayBindingDivisor(mHandle, BINDING_INDEX, divisor));
		}

		// Bind the VBO to the VAO and add the VBO to the list
		GLCall(glVertexArrayVertexBuffer(mHandle, BINDING_INDEX, vbo->getHandle(), 0, layout.getStride()));
		mVBOs.push_back(std::move(vbo));
	}

	// Public virtual method(s)
	void VertexArray::destroy() const
	{
		// Destroy the list of VBOs
		for (const auto& vbo : mVBOs) {
			vbo->destroy();
		}

		GLCall(glDeleteVertexArrays(1, &mHandle));
	}

	void VertexArray::bind() const
	{
		GLCall(glBindVertexArray(mHandle));
	}

	void VertexArray::unbind() const
	{
		GLCall(glBindVertexArray(0));
	}
}