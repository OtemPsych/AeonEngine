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

#include <AEON/Graphics/internal/VertexArray.h>

#include <string>

#include <GL/glew.h>

#include <AEON/Graphics/internal/GLCommon.h>
#include <AEON/Graphics/internal/VertexBuffer.h>
#include <AEON/Graphics/internal/IndexBuffer.h>

namespace ae
{
	// Public constructor(s)
	VertexArray::VertexArray()
		: GLResource()
		, mVBOs()
		, mIBO(nullptr)
		, mAttributeIndex(0)
	{
		// Create the VAO
		GLCall(glCreateVertexArrays(1, &mHandle));
	}

	VertexArray::VertexArray(VertexArray&& rvalue) noexcept
		: GLResource(std::move(rvalue))
		, mVBOs(std::move(rvalue.mVBOs))
		, mIBO(std::move(rvalue.mIBO))
		, mAttributeIndex(rvalue.mAttributeIndex)
	{
	}

	// Public operator(s)
	VertexArray& VertexArray::operator=(VertexArray&& rvalue) noexcept
	{
		// Copy the rvalue's trivial data and move the rest
		GLResource::operator=(std::move(rvalue));
		mVBOs = std::move(rvalue.mVBOs);
		mIBO = std::move(rvalue.mIBO);
		mAttributeIndex = rvalue.mAttributeIndex;

		return *this;
	}

	// Public method(s)
	void VertexArray::addVBO(std::unique_ptr<VertexBuffer> vbo, unsigned int divisor)
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

	void VertexArray::addIBO(std::unique_ptr<IndexBuffer> ibo)
	{
		// Destroy the previous ibo (if there was one)
		if (mIBO) mIBO->destroy();

		// Move the new ibo
		mIBO = std::move(ibo);
	}

	VertexBuffer* const VertexArray::getVBO(size_t index) const noexcept
	{
		// Check if the index is valid
		if (mVBOs.size() <= index) {
			AEON_LOG_ERROR("Invalid index", "The index \"" + std::to_string(index) + "\" isn't associated with any VBOs.");
			return nullptr;
		}

		return mVBOs[index].get();
	}

	IndexBuffer* const VertexArray::getIBO() const noexcept
	{
		// Check if an IBO was added
		if (!mIBO) {
			AEON_LOG_ERROR("Null IBO", "No IBO was associated to the caller VAO.");
			return nullptr;
		}

		return mIBO.get();
	}

	size_t VertexArray::getVBOCount() const noexcept
	{
		return mVBOs.size();
	}

	// Public virtual method(s)
	void VertexArray::destroy() const
	{
		// Destroy the list of VBOs
		for (const auto& vbo : mVBOs) {
			vbo->destroy();
		}

		// Destroy the IBO (if one was added)
		if (mIBO) mIBO->destroy();

		// Destroy the VAO's identifier
		GLCall(glDeleteVertexArrays(1, &mHandle));
	}

	void VertexArray::bind() const
	{
		// Bind the VAO to the context
		GLCall(glBindVertexArray(mHandle));

		// Bind the IBO (if one was added) to the context
		if (mIBO) mIBO->bind();
	}

	void VertexArray::unbind() const
	{
		// Unbind the IBO (if one was added) from the context
		if (mIBO) mIBO->unbind();

		// Unbind the VAO from the context
		GLCall(glBindVertexArray(0));
	}
}