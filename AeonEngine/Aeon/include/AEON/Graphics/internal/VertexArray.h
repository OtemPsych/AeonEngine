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

#ifndef Aeon_Graphics_VertexArray_H_
#define Aeon_Graphics_VertexArray_H_

#include <yvals_core.h>
#include <memory>

#include <AEON/Config.h>

#include <AEON/Graphics/internal/GLResource.h>
#include <AEON/Graphics/internal/VertexBuffer.h>

namespace ae
{
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
		 \brief Deleted move constructor.

		 \since v0.4.0
		*/
		VertexArray(VertexArray&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.4.0
		*/
		VertexArray& operator=(const VertexArray&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.4.0
		*/
		VertexArray& operator=(VertexArray&&) = delete;
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
		 vao->addBuffer(std::move(vbo));
		 \endcode

		 \since v0.4.0
		*/
		void addBuffer(std::unique_ptr<VertexBuffer> vbo, unsigned int divisor = 0);

		// Public virtual method(s)
		/*!
		 \brief Deletes the OpenGL identifier that was created.

		 \since v0.4.0
		*/
		virtual void destroy() const override final;
		/*!
		 \brief Binds the ae::VertexArray to the context indicating to OpenGL that we're about to use it.

		 \sa unbind()

		 \since v0.4.0
		*/
		virtual void bind() const override final;
		/*!
		 \brief Unbinds the ae::VertexArray from the context indicating to OpenGL that we've finished using it.
		 \note Make sure that the currently-bound ae::VertexArray is caller as this method will unbind any VAO.

		 \sa bind()

		 \since v0.4.0
		*/
		virtual void unbind() const override final;

	private:
		// Private member(s)
		std::vector<std::unique_ptr<VertexBuffer>> mVBOs;           //!< The associated VBOs
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
 \version v0.4.0
 \date 2020.01.05
 \copyright MIT License
*/