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

#ifndef Aeon_Graphics_VertexArray_H_
#define Aeon_Graphics_VertexArray_H_

#include <memory>
#include <vector>

#include <AEON/Config.h>

#include <AEON/Graphics/internal/GLResource.h>

namespace ae
{
	// Forward declaration(s)
	class VertexBuffer;
	class IndexBuffer;

	/*!
	 \brief The class that represents the vertex fetch stage and is used to supply input to the appropriate vertex shader.
	 \details A vertex array object is commonly referred to as a VAO.
	 \note This class is considered to be internal but may still be used by the API user.
	*/
	class _NODISCARD AEON_API VertexArray : public GLResource
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Creates an OpenGL VAO initialized to the default state.

		 \since v0.4.0
		*/
		VertexArray();
		/*!
		 \brief Deleted copy constructor.

		 \since v0.4.0
		*/
		VertexArray(const VertexArray&) = delete;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::VertexArray that will be moved

		 \since v0.6.0
		*/
		VertexArray(VertexArray&& rvalue) noexcept;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.4.0
		*/
		VertexArray& operator=(const VertexArray&) = delete;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::VertexArray that will be moved

		 \return The caller ae::VertexArray

		 \since v0.6.0
		*/
		VertexArray& operator=(VertexArray&& rvalue) noexcept;
	public:
		// Public method(s)
		/*!
		 \brief Adds and associates an ae::VertexBuffer to the ae::VertexArray, and applies its data layout.

		 \param[in] vbo The ae::VertexBuffer that will be associated to the ae::VertexArray
		 \param[in] divisor The rate at which generic vertex attributes advance during instanced rendering, 0 by default (instancing is disabled)

		 \par Example:
		 \code
		 // Some data
		 const GLfloat vertices[] = { ... };

		 // Setup the VBO (Vertex Buffer Object)
		 auto vbo = std::make_unique<ae::VertexBuffer>(GL_STATIC_DRAW);
		 vbo->setData(sizeof(vertices), vertices);

		 // Setup the VBO's data layout
		 ae::VertexBuffer::Layout& vboLayout = vbo->getLayout();
		 vboLayout.addElement(GL_FLOAT, 3, GL_FALSE); // The vertex's position composed of 3 floats
		 vboLayout.addElement(GL_FLOAT, 4, GL_FALSE); // The vertex's color composed of 4 floats

		 // Create the VAO and add in the VBO that was previously created
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 auto vao = glResourceFactory.createVertexArray();
		 vao->addVBO(std::move(vbo));
		 \endcode

		 \sa getVBOCount(), getVBO(), addIBO()

		 \since v0.5.0
		*/
		void addVBO(std::unique_ptr<VertexBuffer> vbo, unsigned int divisor = 0);
		/*!
		 \brief Adds an ae::IndexBuffer to the ae::VertexArray.
		 \details The attachment of an ae::IndexBuffer to the ae::VertexArray is mostly conceptual as it merely serves to better organize the different buffers and bind the IBO automatically when the VAO is bound.
		 \note Only *one* ae::IndexBuffer can be attached to the ae::VertexArray. If this method is called a second time, the previously-added ae::IndexBuffer will be replaced by the new one.

		 \param[in] ibo The ae::IndexBuffer that will be bound once the ae::VertexArray is bound

		 \par Example:
		 \code
		 // Create the IBO
		 auto ibo = std::make_unique<ae::IndexBuffer>(GL_STATIC_DRAW);

		 // Create the VAO and add in the IBO that was previously created
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 auto vao = glResourceFactory.createVertexArray();
		 vao->addIBO(std::move(ibo));

		 // If the addIBO() method is called again, the previous IBO will be replaced
		 auto secondIBO = std::make_unique<ae::IndexBuffer>(GL_STATIC_DRAW);
		 vao->addIBO(std::move(secondIBO)); // ibo was replaced with secondIBO
		 \endcode

		 \sa getIBO()

		 \since v0.5.0
		*/
		void addIBO(std::unique_ptr<IndexBuffer> ibo);
		/*!
		 \brief Retrieves the previously-added ae::VertexBuffer associated to the index provided.

		 \param[in] index The index associated with the desired ae::VertexBuffer to retrieve

		 \return The ae::VertexBuffer associated to the index provided or nullptr if the index is invalid

		 \par Example:
		 \code
		 // Setup the VBO (Vertex Buffer Object)
		 auto vbo = std::make_unique<ae::VertexBuffer>(GL_STATIC_DRAW);
		 ...

		 // Create the VAO and add in the VBO that was previously created
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 auto vao = glResourceFactory.createVertexArray();
		 vao->addVBO(std::move(vbo));

		 ...

		 // Retrieve the last VBO added
		 ae::VertexBuffer* const lastVBO = vao->getVBO(vao->getVBOCount() - 1);
		 \endcode

		 \sa addVBO()

		 \since v0.5.0
		*/
		_NODISCARD VertexBuffer* const getVBO(size_t index) const noexcept;
		/*!
		 \brief Retrieves the ae::IndexBuffer associated to the ae::VertexArray.

		 \return The ae::IndexBuffer associated to the ae::VertexArray or nullptr if no IBO was added

		 \par Example:
		 \code
		 // Create the IBO
		 auto ibo = std::make_unique<ae::IndexBuffer>(GL_STATIC_DRAW);

		 // Create the VAO and add in the IBO that was previously created
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 auto vao = glResourceFactory.createVertexArray();
		 vao->addIBO(std::move(ibo));

		 ...

		 // Retrieve the IBO added
		 ae::IndexBuffer* const iboPtr = vao->getIBO();
		 \endcode

		 \sa addIBO()

		 \since v0.5.0
		*/
		_NODISCARD IndexBuffer* const getIBO() const noexcept;
		/*!
		 \brief Retrieves the number of ae::VertexBuffer instances that have been added to the ae::VertexArray.

		 \return The number of ae::VertexBuffer instances added

		 \par Example:
		 \code
		 // Setup the VBO (Vertex Buffer Object)
		 auto vbo = std::make_unique<ae::VertexBuffer>(GL_STATIC_DRAW);
		 ...

		 // Create the VAO and add in the VBO that was previously created and set up
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 auto vao = glResourceFactory.createVertexArray();
		 vao->addVBO(std::move(vbo));

		 // Retrieve the number of VBOs attached
		 size_t vboCount = vao->getVBOCount();
		 \endcode

		 \sa addVBO(), getVBO()

		 \since v0.5.0
		*/
		_NODISCARD size_t getVBOCount() const noexcept;

		// Public virtual method(s)
		/*!
		 \brief Destroys the VBOs that have been added, the IBO (if one was added) and the ae::VertexArray's OpenGL identifier.

		 \since v0.5.0
		*/
		virtual void destroy() const override final;
		/*!
		 \brief Binds the ae::VertexArray and the ae::IndexBuffer (if one was added) to the context indicating to OpenGL that we're about to use them.

		 \sa unbind()

		 \since v0.5.0
		*/
		virtual void bind() const override final;
		/*!
		 \brief Unbinds the ae::VertexArray and the ae::IndexBuffer (if one was added) from the context indicating to OpenGL that we've finished using them.
		 \note Make sure that the currently-bound ae::VertexArray is caller as this method will unbind any VAO.

		 \sa bind()

		 \since v0.5.0
		*/
		virtual void unbind() const override final;

	private:
		// Private member(s)
		std::vector<std::unique_ptr<VertexBuffer>> mVBOs;           //!< The associated VBOs
		std::unique_ptr<IndexBuffer>               mIBO;            //!< The associated IBO
		unsigned int                               mAttributeIndex; //!< The current index of the last-added vertex attribute
	};
}
#endif // Aeon_Graphics_VertexArray_H_

/*!
 \class ae::VertexArray
 \ingroup graphics

 The ae::VertexArray class represents the vertex fetch stage of the OpenGL
 pipeline and is used to supply input to the vertex shader. This class is
 purely intended to be used internally unless the API user wishes to use their
 OpenGL code.

 Typically, each mesh and model possess their own VAO (Vertex Array Object)
 used to supply their inputs to the corresponding vertex shaders.

 All ae::VertexArray instances should be created via the ae::GLResourceFactory
 instance in order for it to be responsible for their lifetime and for the
 proper deallocation of all OpenGL resources. If it isn't created via the
 ae::GLResourceFactory instance, their management must be handled manually,
 meaning that the 'destroy()' method must be called when the resource is no
 longer needed.

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2020.08.27
 \copyright MIT License
*/