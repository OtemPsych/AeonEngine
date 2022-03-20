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

#pragma once

#include <unordered_map>
#include <queue>
#include <string>

#include <AEON/Graphics/internal/Buffer.h>

namespace ae
{
	// Forward declaration(s)
	class Shader;

	/*!
	 \brief The class representing an OpenGL buffer used to supply uniform data to shaders.
	 \note This class is considered to be internal but may still be used by the API user.
	*/
	class AEON_API UniformBuffer : public Buffer
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details The list of uniform data will be initialized as empty.

		 \since v0.4.0
		*/
		UniformBuffer();
		/*!
		 \brief Deleted copy constructor.

		 \since v0.4.0
		*/
		UniformBuffer(const UniformBuffer&) = delete;
		/*!
		 \brief Move constructor.

		 \since v0.7.0
		*/
		UniformBuffer(UniformBuffer&&) noexcept = default;

		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.4.0
		*/
		UniformBuffer& operator=(const UniformBuffer&) = delete;
		/*!
		 \brief Move assignment operator.

		 \return The caller ae::UniformBuffer

		 \since v0.7.0
		*/
		UniformBuffer& operator=(UniformBuffer&&) noexcept = default;

		// Public method(s)
		/*!
		 \brief Queries the \a shader's layout to retrieve information about the uniform block.
		 \details Several ae::Shader objects can share the same ae::UniformBuffer as long as they possess the same uniform block.

		 \param[in] shader The ae::Shader containing the uniform block
		 \param[in] blockName A string containing the name of the uniform block
		 \param[in] uniformNames An initializer_list of strings containing the names of the uniform block's uniforms

		 \par Example:
		 \code
		 // Retrieve the single instance of the GLResourceFactory
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();

		 // Create the shader program and attach the shader stages that will be created within the shader program
		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->loadFromFile(ae::Shader::StageType::Vertex, "Shaders/vertexShader.vs");
		 shader->loadFromFile(ae::Shader::StageType::Fragment, "Shaders/fragmentShader.fs");
		 shader->link();

		 // Create a UBO and query the previously-created shader's layout to retrieve information about the uniform block
		 std::unique_ptr<ae::UniformBuffer> ubo;
		 ubo->queryLayout(*shader, "uTransformBlock", { "model", "view", "projection" });
		 \endcode

		 \since v0.7.0
		*/
		void queryLayout(const Shader& shader, const std::string& blockName, std::initializer_list<std::string> uniformNames);

		/*!
		 \brief Enqueues a uniform upload by providing the uniform's \a name and the new \a data along with its \a size in bytes.
		 \details Queueing the uniform uploads is far more efficient when we need to update several uniforms as the OpenGL function will only be called once and all previously-queued uniform data will be uploaded at the same time.
		 \note The 'uploadQueuedUniforms()' will have to be called in order for the uniforms' new data to be uploaded to the OpenGL buffer.

		 \param[in] name A string containing the name of the uniform
		 \param[in] data The pointer to the uniform's new data
		 \param[in] size The size of the uniform's new \a data in bytes (sizeof() of the data's type)

		 \par Example:
		 \code
		 // Create a shader
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 ...

		 // Create a UBO and query the shader's layout
		 std::unique_ptr<ae::UniformBuffer> ubo;
		 ubo->queryLayout(*shader, "uBlock", { "floatValue", "intValue", "matrix4fValue" });

		 // The new data
		 float floatValue = 5.f;
		 int intValue = 2;
		 ae::Matrix4f matrix4fValue = ae::Matrix4f::identity();

		 // Enqueue the UBO's uniform uploads (the new data won't be sent to OpenGL by calling this method)
		 ubo->queueUniformUpload("floatValue", &floatValue, sizeof(float));
		 ubo->queueUniformUpload("intValue", &intValue, sizeof(int));
		 ubo->queueUniformUpload("matrix4fValue", matrix4fValue.elements.data(), sizeof(ae::Matrix4f));

		 // Upload the UBO's new uniform data to OpenGL once all the uniform uploads have been enqueued
		 ubo->uploadQueuedUniforms();
		 \endcode

		 \sa uploadQueuedUniforms()

		 \since v0.4.0
		*/
		void queueUniformUpload(const std::string& name, const void* data, size_t size);
		/*!
		 \brief Uploads all previously-enqueued uniform uploads to the OpenGL buffer.
		 \details Queueing the uniform uploads is far more efficient when we need to update several uniforms as the OpenGL function will only be called once and all previously-queued uniform data will be uploaded at the same time.

		 \par Example:
		 \code
		 // Create a UBO and query the shader's layout
		 std::unique_ptr<ae::UniformBuffer> ubo;
		 ubo->queryLayout(*shader, "uBlock", { "floatValue", "intValue", "matrix4fValue" });

		 // The new data
		 float floatValue = 5.f;
		 int intValue = 2;
		 ae::Matrix4f matrix4fValue = ae::Matrix4f::identity();

		 // Enqueue the UBO's uniform uploads (the new data won't be sent to OpenGL by calling this method)
		 ubo->queueUniformUpload("floatValue", &floatValue, sizeof(float));
		 ubo->queueUniformUpload("intValue", &intValue, sizeof(int));
		 ubo->queueUniformUpload("matrix4fValue", matrix4fValue.elements.data(), sizeof(ae::Matrix4f));

		 // Upload the UBO's new uniform data to OpenGL once all the uniform uploads have been enqueued
		 ubo->uploadQueuedUniforms();
		 \endcode

		 \sa queueUniformUpload()

		 \since v0.7.0
		*/
		void uploadQueuedUniforms();

		/*!
		 \brief Retrieves the ae::UniformBuffer's associated uniform block name.
		 \note This block name will be empty if an ae::Shader's uniform block layout hasn't been queried.

		 \return The ae::UniformBuffer's associated uniform block name

		 \par Example:
		 \code
		 // Create a UBO
		 std::unique_ptr<ae::UniformBuffer> ubo;

		 // Retrieve the uniform block name (this will be empty as a shader's layout hasn't been queried)
		 const std::string& uboBlockName = ubo->getBlockName();

		 // Query the shader's layout
		 ubo->queryLayout(*shader, "uBlock", { "floatValue", "intValue", "matrix4fValue" });

		 // Retrieve the uniform block name (this will be equal to "uBlock")
		 const std::string& uboBlockName = ubo->getBlockName();
		 \endcode

		 \since v0.7.0
		*/
		[[nodiscard]] inline const std::string& getBlockName() const noexcept { return mBlockName; }
		/*!
		 \brief Retrieves the ae::UniformBuffer's automatically-generated binding point.
		 \details Each ae::UniformBuffer instance possesses a unique binding point to which they will be bound.

		 \return The ae::UniformBufer's unique binding point

		 \par Example:
		 \code
		 // Create a UBO and query the shader's layout
		 std::unique_ptr<ae::UniformBuffer> ubo;
		 ubo->queryLayout(*shader, "uBlock", { "floatValue", "intValue", "matrix4fValue" });

		 // Retrieve the uniform buffer's unique binding point
		 int uboBindingPoint = ubo->getBindingPoint();
		 \endcode

		 \since v0.7.0
		*/
		[[nodiscard]] inline uint32_t getBindingPoint() const noexcept { return mBindingPoint; }

	private:
		// Private method(s)
		/*!
		 \brief Retrieves all information regarding the uniforms.

		 \param[in] uniformNames The names of the uniform block's uniforms
		 \param[in] blockName The name of the shader's uniform block
		 \param[in] shaderID The ae::Shader's handle containing the uniform block

		 \sa initUniformBlock()

		 \since v0.7.0
		*/
		void initUniforms(const std::initializer_list<std::string>& uniformNames,
		                  const std::string& blockName, uint32_t shaderID);
		/*!
		 \brief Initializes the ae::UniformBuffer's OpenGL buffer based on the uniform data retrieved.

		 \param[in] shaderID The ae::Shader's handle containing the uniform block
		 
		 \sa initUniforms()

		 \since v0.7.0
		*/
		void initUniformBlock(uint32_t shaderID);

		// Private nested struct(s)
		/*!
		 \brief Struct representing the data of a shader uniform.
		*/
		struct Uniform 
		{
			std::unordered_map<uint32_t, int32_t> metadata; //!< The uniform's metadata
			uint32_t                              index;    //!< The uniform's index in the uniform block

			/*!
			 \brief Constructs the ae::UniformBuffer::Uniform by providing the necessary elements.

			 \param[in] uniformMetadata The metadata of all the block's uniforms
			 \param[in] metadataIndex The index associated to the specific uniform's metadata
			 \param[in] uniformIndex The uniform's index in the shader

			 \since v0.4.0
			*/
			Uniform(const std::vector<std::pair<uint32_t, std::vector<int>>>& uniformMetadata,
			        size_t metadataIndex, unsigned int uniformIndex);
		};
		/*!
		 \brief Struct representing an enqueued uniform upload.
		*/
		struct UniformUpload
		{
			const std::unordered_map<std::string, Uniform>::iterator uniform; //!< The uniform in question
			const void*                                              data;    //!< The uniform's new data
			size_t                                                   size;    //!< The size of the uniform's new data in bytes
		};

		// Private member(s)
		std::unordered_map<std::string, Uniform> mUniforms;     //!< The list of all the uniform block's uniforms
		std::queue<UniformUpload>                mUploadQueue;  //!< The enqueued uniform uploads
		std::string                              mBlockName;    //!< The name of the uniform block
		uint32_t                                 mBindingPoint; //!< The uniform buffer's assigned binding point
	};
}

/*!
 \class ae::UniformBuffer
 \ingroup graphics

 The ae::UniformBuffer class represents an OpenGL memory buffer used to store
 uniform data. This uniform data is then supplied to the GLSL shaders that
 contain a uniform block, the uniform block can use the 'shared' layout as
 this class will query said layout.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.12.27
 \copyright MIT License
*/