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

#ifndef Aeon_Graphics_VertexBuffer_H_
#define Aeon_Graphics_VertexBuffer_H_

#include <vector>

#include <AEON/Graphics/internal/Buffer.h>

namespace ae
{
	/*!
	 \brief The class representing an OpenGL buffer used to supply vertex data to a vertex shader.
	 \note This class is considered to be internal but may still be used by the API user.
	*/
	class _NODISCARD AEON_API VertexBuffer : public Buffer
	{
	public:
		// Public nested class(es)
		/*!
		 \brief The class representing the data layout of an ae::VertexBuffer.
		 \note This class is considered to be internal but may still be used by the API user.
		*/
		class _NODISCARD AEON_API Layout
		{
		public:
			// Public enumeration(s)
			/*!
			 \brief The struct representing a single format element associated to a vertex attribute.
			*/
			struct Element
			{
				uint32_t     type;       //!< The OpenGL data type
				int          count;      //!< The number of components per element
				unsigned int offset;     //!< The offset in bytes at which the component's data begins
				bool         normalized; //!< Whether the data should be normalized or not (value between 0 and 1)
			};

		public:
			// Public constructor(s)
			/*!
			 \brief Default constructor.
			 \details Set the stride between vertices to 0.

			 \since v0.4.0
			*/
			Layout() noexcept;
		public:
			// Public method(s)
			/*!
			 \brief Adds a format element to be associated to a vertex attribut.

			 \param[in] type The OpenGL data type of the attribut (GL_FLOAT, GL_UNSIGNED_INT, etc.)
			 \param[in] count The number of components per vertex
			 \param[in] normalized Whether the data has to be normalized before passing them to OpenGL

			 \par Example:
			 \code
			 // Add 3 integers that must be normalized (un ae::Vector3i, for example)
			 vbo->getLayout().addElement(GL_INT, 3, GL_TRUE);
			 \endcode

			 \sa getElements()

			 \since v0.4.0
			*/
			void addElement(uint32_t type, int count, bool normalized);
			/*!
			 \brief Retrieves the list of format elements.

			 \return The format elements of the ae::VertexBuffer

			 \par Example:
			 \code
			 vbo->getLayout().addElement(GL_INT, 3, GL_TRUE);
			 ...
			 const std::vector<ae::VertexBuffer::Layout::Element>& layoutElements = vbo->getLayout().getElements();
			 \endcode

			 \sa addElement()

			 \since v0.4.0
			*/
			_NODISCARD const std::vector<Element>& getElements() const noexcept;
			/*!
			 \brief Retrieves the stride in bytes between each vertex attribut's data.

			 \return The stride in bytes between each vertex

			 \par Example:
			 \code
			 vbo->getLayout().addElement(GL_INT, 3, GL_TRUE);
			 ...
			 GLsizei layoutStride = vbo->getLayout().getStride();
			 \endcode

			 \since v0.4.0
			*/
			_NODISCARD int getStride() const noexcept;

		private:
			// Private member(s)
			std::vector<Element> mElements; //!< The format elements of the buffer's data store
			int                  mStride;   //!< The amount of bytes between the beginning of a vertex's data and the beginning of the next vertex's data
		};

	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::VertexBuffer by providing the intended \a usage pattern.
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
		 ae::VertexBuffer vbo(GL_STATIC_DRAW);
		 \endcode

		 \since v0.4.0
		*/
		explicit VertexBuffer(uint32_t usage);
		/*!
		 \brief Deleted copy constructor.

		 \since v0.4.0
		*/
		VertexBuffer(const VertexBuffer&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.4.0
		*/
		VertexBuffer(VertexBuffer&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.4.0
		*/
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.4.0
		*/
		VertexBuffer& operator=(VertexBuffer&&) = delete;
	public:
		// Public method(s)
		/*!
		 \brief (Re)Creates a new data store for the ae::VertexBuffer with the \a size in bytes specified and the \a data itself.
		 \details This method is best suited for creating the data store and resizing it if all the data is available at the call of this method.\n
		 Consider the 'setSubData()' method if the data store's content need only be modified.

		 \param[in] size The size of the data store, measured in bytes
		 \param[in] data A pointer to the data that will be placed in the data store

		 \par Example:
		 \code
		 // Some data
		 const GLfloat data[] = {
			 0.25, -0.25, 0.5, 1.0,
			-0.25, -0.25, 0.5, 1.0,
			 0.25,  0.25, 0.5, 1.0
		 };

		 // Instantiate a VBO and create its data store
		 auto vbo = std::make_unique<ae::VertexBuffer>(GL_STATIC_DRAW);
		 vbo->setData(sizeof(data), data); // Create and initialize the data store
		 \endcode

		 \sa setSubData()

		 \since v0.4.0
		*/
		void setData(int size, const void* data) const;
		/*!
		 \brief Modifies the entire or part of the ae::VertexBuffer's data store.
		 \details The data to be modified start at the \a offset provided in bytes, up to the \a size in bytes; the specified memory range will be replaced by the \a data.
		 \note This method is best suited for modifying the existing data store's content without allocating new memory.\n
		 Consider the 'setData()' method if the data store must be created or resized.

		 \param[in] offset Specifies the offset from the start of the data store's memory in which the \a data will be placed, measured in bytes
		 \param[in] size The memory region of the data store that will be affected, measured in bytes
		 \param[in] data A pointer to the data that will be placed in the data store

		 \par Example:
		 \code
		 // Some data
		 const GLfloat data[] = {
			 0.25, -0.25, 0.5, 1.0,
			-0.25, -0.25, 0.5, 1.0,
			 0.25,  0.25, 0.5, 1.0
		 };

		 // Instantiate a VBO and create its data store
		 auto vbo = std::make_unique<ae::VertexBuffer>(GL_DYNAMIC_DRAW);
		 vbo->setData(sizeof(data), data); // Create and initialize the data store
		 ...
		 // Modify the second line of the existing data store
		 const GLfloat secondLine[] = { 0.25, -0.25, 1.0, 1.0 };
		 vbo->setSubData(sizeof(GLfloat) * 4, sizeof(secondLine), secondLine); // Modifies the second line of the original data
		 \endcode

		 \sa setData()

		 \since v0.4.0
		*/
		void setSubData(int offset, int size, const void* data) const;
		/*!
		 \brief Retrieves the ae::VertexBuffer::Layout that describes the data format of the ae::VertexBuffer.

		 \return An ae::VertexBuffer::Layout describing the data format

		 \par Example:
		 \code
		 // Some data
		 const GLfloat data[] = {
			 0.25, -0.25, 0.5, 1.0,
			-0.25, -0.25, 0.5, 1.0,
			 0.25,  0.25, 0.5, 1.0
		 };

		 // Instantiate a VBO and create its data store
		 auto vbo = std::make_unique<ae::VertexBuffer>(GL_STATIC_DRAW);
		 vbo->setData(sizeof(data), data);
		 vbo->getLayout().addElement(GL_FLOAT, 4, GL_FALSE); // each vertex contains 4 floats
		 \endcode

		 \since v0.4.0
		*/
		_NODISCARD Layout& getLayout() noexcept;

	private:
		// Private member(s)
		Layout   mLayout; //!< The data store's layout
		uint32_t mUsage;  //!< The intended data store usage pattern
	};
}
#endif // Aeon_Graphics_VertexBuffer_H_

/*!
 \class ae::VertexBuffer::Layout
 \ingroup graphics

 The ae::VertexBuffer::Layout class is used to represent an ae::VertexBuffer's
 data layout that contains several data types, for example: a position, a color,
 texture coordinates, etc. However, it can prove tedious to calculate the
 offsets in bytes between each attribut and the stride in bytes between each
 vertex; using this class, we can simply add in all the elements of the layout
 which will, in turn, automatically calculate these offsets and strides.

 \author Filippos Gleglakos
 \version v0.4.0
 \date 2020.01.05
 \copyright MIT License
*/

/*!
 \class ae::VertexBuffer
 \ingroup graphics

 The ae::VertexBuffer class represents an OpenGL buffer used to store vertex
 data that will be passed on to the vertex shader stage of a shader program.

 Usage example:
 \code
 // A struct defining a vertex
 struct Vertex
 {
	ae::Vector3f position; // 3 floats
	ae::Vector3f normal;   // 3 floats
	ae::Vector2f texCoords // 2 floats
 };

 // The vertex data
 const Vertex vertices[] = {
	Vertex(ae::Vector3f(-0.25f, -0.25f, 0.25f), ae::Vector2f(0.2f, 0.35f, 1.f), ae::Vector2f(0.f, 0.f)),
	...
 };

 // Create the vertex buffer and add in the vertices
 auto vbo = std::make_unique<ae::VertexBuffer>(GL_STATIC_DRAW);
 vbo->setData(sizeof(vertices), vertices);

 // Setup the data layout of the vertex buffer. By adding in the elements, the bytes occupied by each vertex are automatically calculated
 ae::VertexBuffer::Layout& vboLayout = vbo->getLayout();
 vboLayout.addElement(GL_FLOAT, 3, GL_FALSE); // The Vector3f containing the position
 vboLayout.addElement(GL_FLOAT, 3, GL_FALSE); // The Vector3f containing the normals
 vboLayout.addElement(GL_FLOAT, 2, GL_FALSE); // The Vector2f containing the texture coordinates

 // Create a new VAO (Vertex Array Object) and attach the VBO (Vertex Buffer Object) to the VAO
 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
 auto vao = glResourceFactory.createVertexArray();
 vao->addBuffer(std::move(vbo));
 \endcode

 \author Filippos Gleglakos
 \version v0.4.0
 \date 2019.10.14
 \copyright MIT License
*/