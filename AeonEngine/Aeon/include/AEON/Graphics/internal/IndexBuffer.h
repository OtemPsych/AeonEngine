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

#ifndef Aeon_Graphics_IndexBuffer_H_
#define Aeon_Graphics_IndexBuffer_H_

#include <AEON/Graphics/internal/Buffer.h>

namespace ae
{
	/*!
	 \brief The class representing an OpenGL buffer containing vertex indices.
	 \details Indices allows us to reuse the same vertex several times without duplicating it.
	 \note This class is considered to be internal but may still be used by the API user.
	*/
	class _NODISCARD AEON_API IndexBuffer : public Buffer
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::IndexBuffer by providing the intended \a usage pattern.
		 \details Usage patterns allow OpenGL to allocate memory that will provide faster access based on the intended use.\n
		 Available usage patterns:
		 \li GL_STATIC_DRAW The data store contents will be modified once and used many times, and modified by the application and used as the source for GL drawing and image specification commands
		 \li GL_DYNAMIC_DRAW The data store contents will be modified repeatedly and used many times, and modified by the application and used as the source for GL drawing and image specification commands
		 \li GL_STREAM_DRAW The data store contents will be modified once and used at most a few times, and modified by the application and used as the source for GL drawing and image specification commands
		 \li GL_STATIC_READ The data store contents will be modified once and used many times, and modified by reading data from OpenGL and used to return that data when queried by the application
		 \li GL_DYNAMIC_READ The data store contents will be modified repeatedly and used many times, and modified by reading data from OpenGL and used to return that data when queried by the application
		 \li GL_STREAM_READ The data store contents will be modified once and used at most a few times, and modified by reading data from OpenGL and used to return that data when queried by the application
		 \li GL_STATIC_COPY The data store contents will be modified once and used many times, and modified by reading data from OpenGL and used as the source for GL drawing and image specification commands
		 \li GL_DYNAMIC_COPY The data store contents will be modified repeatedly and used many times, and modified by reading data from OpenGL and used as the source for GL drawing and image specification commands
		 \li GL_STREAM_COPY The data store contents will be modified once and used at most a few times, and modified by reading data from OpenGL and used as the source for GL drawing and image specification commands

		 \param[in] usage The data store's usage pattern (the function glBufferData of the OpenGL documentation may be consulted)

		 \par Example:
		 \code
		 ae::IndexBuffer ibo(GL_STATIC_DRAW);
		 \endcode

		 \since v0.4.0
		*/
		explicit IndexBuffer(uint32_t usage);
		/*!
		 \brief Deleted copy constructor.
		 
		 \since v0.4.0
		*/
		IndexBuffer(const IndexBuffer&) = delete;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::IndexBuffer that will be moved

		 \since v0.6.0
		*/
		IndexBuffer(IndexBuffer&& rvalue) noexcept;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.4.0
		*/
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::IndexBuffer that will be moved

		 \return The caller ae::IndexBuffer

		 \since v0.6.0
		*/
		IndexBuffer& operator=(IndexBuffer&& rvalue) noexcept;
	public:
		// Public method(s)
		/*!
		 \brief (Re)Creates a new data store for the ae::IndexBuffer with the \a size in bytes specified and the \a data itself.
		 
		 \param[in] size The size of the data store, measured in bytes
		 \param[in] data A pointer to the data that will be placed in the data store

		 \par Example:
		 \code
		 // Vertex data
		 const GLfloat vertices[] = {
			 0.25, -0.25, 0.5, 1.0,
			-0.25, -0.25, 0.5, 1.0,
			 0.25,  0.25, 0.5, 1.0
		 };

		 // Instantiate a VBO and create its data store
		 auto vbo = std::make_unique<ae::VertexBuffer>(GL_STATIC_DRAW);
		 vbo->setData(sizeof(vertices), vertices); // Create and initialize the data store

		 // Index data
		 const GLuint indices[] = {
			0, 1, 2, 3,
			1, 1, 2, 3,
			0, 0, 2, 3
		 };

		 // Instantiate an IBO and create its data store
		 auto ibo = std::make_unique<ae::IndexBuffer>(GL_STATIC_DRAW);
		 ibo->setData(sizeof(indices), indices); // Create and initialize the data store
		 \endcode

		 \since v0.4.0
		*/
		void setData(unsigned int size, const unsigned int* data);
		/*!
		 \brief Retrieves the ae::IndexBuffer's total number of indices.

		 \return The total number of indices held

		 \since v0.4.0
		*/
		_NODISCARD unsigned int getCount() const noexcept;

	private:
		// Private member(s)
		unsigned int mCount; //!< The total number of indices
		uint32_t     mUsage; //!< The intended data store usage pattern
	};
}
#endif // Aeon_Graphics_IndexBuffer_H_

/*!
 \class ae::IndexBuffer
 \ingroup graphics

 The ae::IndexBuffer (IBO) class is directly associated to an ae::VertexBuffer
 object (VBO) wherein the IBO's indices refer to specific vertices contained in
 the associated VBO. If any of the VBO's vertices are equal to one another, the
 same index will be used to refer to them thus optimizing rendering.

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2020.08.27
 \copyright MIT License
*/