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

#include <AEON/Graphics/internal/UniformBuffer.h>

#include <vector>

#include <GL/glew.h>

#include <AEON/Graphics/internal/GLCommon.h>
#include <AEON/Graphics/Shader.h>

namespace ae
{
	// Global variable(s) (inaccessible from outside the source file)
	int bindingPointCounter = 0; //!< Used to assign an incrementing binding point to each UBO created

	// UniformBuffer
		// Public constructor(s)
	UniformBuffer::UniformBuffer()
		: Buffer(GL_UNIFORM_BUFFER)
		, mUniforms()
		, mUploadQueue()
		, mBlockName("")
		, mBindingPoint(bindingPointCounter++)
	{
		// Bind the UBO to the assigned binding point
		GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, mBindingPoint, mHandle));
	}

	UniformBuffer::UniformBuffer(UniformBuffer&& rvalue) noexcept
		: Buffer(std::move(rvalue))
		, mUniforms(std::move(rvalue.mUniforms))
		, mUploadQueue(std::move(rvalue.mUploadQueue))
		, mBlockName(std::move(rvalue.mBlockName))
		, mBindingPoint(rvalue.mBindingPoint)
	{
	}

		// Public operator(s)
	UniformBuffer& UniformBuffer::operator=(UniformBuffer&& rvalue) noexcept
	{
		// Copy the rvalue's trivial data and move the rest
		Buffer::operator=(std::move(rvalue));
		mUniforms = std::move(rvalue.mUniforms);
		mUploadQueue = std::move(rvalue.mUploadQueue);
		mBlockName = std::move(rvalue.mBlockName);
		mBindingPoint = rvalue.mBindingPoint;

		return *this;
	}

		// Public method(s)
	void UniformBuffer::queryLayout(const Shader& shader, const std::string& blockName, std::initializer_list<std::string> uniformNames)
	{
		// Keep a copy of the uniform block's name
		mBlockName = blockName;

		// Query the shader's uniform block's layout
		const unsigned int SHADER_ID = shader.getHandle();
		initUniforms(uniformNames, blockName + '.', SHADER_ID);
		initUniformBlock(SHADER_ID);
	}

	void UniformBuffer::queueUniformUpload(const std::string& name, const void* data, size_t size)
	{
		// Enqueue the uniform upload if the name provided is associated to a valid uniform
		auto uniformItr = mUniforms.find(mBlockName + '.' + name);
		if (uniformItr != mUniforms.end()) {
			mUploadQueue.push({ uniformItr, data, size });
		}
	}

	void UniformBuffer::uploadQueuedUniforms()
	{
		// Check if any uniform uploads have been enqueued
		if (!mUploadQueue.empty()) {
			void* mapped = map(GL_WRITE_ONLY);
			do {
				const UniformUpload& upload = mUploadQueue.front();
				const int OFFSET = upload.uniform->second.metadata[GL_UNIFORM_OFFSET];
				std::memcpy(static_cast<void*>(static_cast<char*>(mapped) + OFFSET), upload.data, upload.size);
				mUploadQueue.pop();
			} while (!mUploadQueue.empty());
			unmap();
		}
	}

	const std::string& UniformBuffer::getBlockName() const noexcept
	{
		return mBlockName;
	}

	int UniformBuffer::getBindingPoint() const noexcept
	{
		return mBindingPoint;
	}

		// Private method(s)
	void UniformBuffer::initUniforms(const std::initializer_list<std::string>& uniformNames,
	                                 const std::string& blockName, unsigned int shaderID)
	{
		const GLsizei UNIFORM_COUNT = static_cast<GLsizei>(uniformNames.size());

		// Uniforms' primary data (indices)
			// Put together the uniforms' full names (block name + uniform name)
		std::vector<std::string> cppNames;
		cppNames.reserve(UNIFORM_COUNT);
		for (const std::string& name : uniformNames) {
			cppNames.emplace_back(blockName + name);
		}

		std::vector<const GLchar*> cNames;
		cNames.reserve(UNIFORM_COUNT);
		for (const std::string& name : cppNames) {
			cNames.emplace_back(name.c_str());
		}

			// Retrieve the uniforms' indices
		std::vector<GLuint> indices(UNIFORM_COUNT);
		GLCall(glGetUniformIndices(shaderID, UNIFORM_COUNT, cNames.data(), indices.data()));

		// Uniforms' metadata
			// Setup the metadata's containers as every uniform's metadata will be retrieved at once
		std::vector<std::pair<GLenum, std::vector<GLint>>> uniformMetadata;
		uniformMetadata.reserve(8);
		for (GLenum i = GL_UNIFORM_TYPE; i <= GL_UNIFORM_IS_ROW_MAJOR; ++i) {
			uniformMetadata.emplace_back(i, UNIFORM_COUNT);
		}

			// Retrieve each uniform's metadata (offsets, strides, etc.)
		for (auto& metadata : uniformMetadata) {
			GLCall(glGetActiveUniformsiv(shaderID, UNIFORM_COUNT, indices.data(), metadata.first, metadata.second.data()));
		}

		// Keep a copy of each uniform's name, index and metadata
		for (GLsizei i = 0; i < UNIFORM_COUNT; ++i) {
			mUniforms.emplace(cppNames[i], Uniform(uniformMetadata, i, indices[i]));
		}
	}

	void UniformBuffer::initUniformBlock(unsigned int shaderID)
	{
		const GLuint BLOCK_INDEX = mUniforms.begin()->second.metadata[GL_UNIFORM_BLOCK_INDEX];

		// Retrieve the uniform block's buffer size
		GLint blockSize;
		GLCall(glGetActiveUniformBlockiv(shaderID, BLOCK_INDEX, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize));

		// Initialize the uniform block's data store
		GLCall(glNamedBufferData(mHandle, blockSize, nullptr, GL_DYNAMIC_DRAW));
	}

	// UniformBuffer::Uniform		
		// Public constructor(s)
	UniformBuffer::Uniform::Uniform(const std::vector<std::pair<uint32_t, std::vector<int>>>& uniformMetadata,
	                                size_t metadataIndex, unsigned int uniformIndex)
		: metadata()
		, index(uniformIndex)
	{
		for (const auto& uniMeta : uniformMetadata) {
			metadata.emplace(uniMeta.first, uniMeta.second[metadataIndex]);
		}
	}
}