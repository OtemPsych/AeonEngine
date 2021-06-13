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

#ifndef Aeon_Graphics_Buffer_H_
#define Aeon_Graphics_Buffer_H_

#include <cstdint>

#include <AEON/Graphics/internal/GLResource.h>

namespace ae
{
	/*!
	 \brief The base class representing a generalized OpenGL buffer used to supply data to OpenGL.
	 \note This class is considered to be internal but may still be used by the API user.
	*/
	class _NODISCARD AEON_API Buffer : public GLResource
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::Buffer by providing a bind \a target.
		 \details Available buffer binding targets:
		 \li GL_ARRAY_BUFFER              - Vertex attributes
		 \li GL_ATOMIC_COUNTER_BUFFER     - Atomic counter storage
		 \li GL_COPY_READ_BUFFER          - Buffer copy source
		 \li GL_COPY_WRITE_BUFFER         - Buffer copy destination
		 \li GL_DISPATCH_INDIRECT_BUFFER  - Indirect compute dispatch commands
		 \li GL_DRAW_INDIRECT_BUFFER      - Indirect command arguments
		 \li GL_ELEMENT_ARRAY_BUFFER      - Vertex array indices
		 \li GL_PIXEL_PACK_BUFFER         - Pixel read target
		 \li GL_PIXEL_UNPACK_BUFFER       - Texture data source
		 \li GL_QUERY_BUFFER              - Query result buffer
		 \li GL_SHADER_STORAGE_BUFFER     - Read-write storage for shaders
		 \li GL_TEXTURE_BUFFER            - Texture data buffer
		 \li GL_TRANSFORM_FEEDBACK_BUFFER - Transform feedback buffer
		 \li GL_UNIFORM_BUFFER            - Uniform block storage

		 \param[in] target The binding target of the ae::Buffer

		 \par Example:
		 \code
		 ae::Buffer buffer(GL_ARRAY_BUFFER);
		 \endcode

		 \since v0.4.0
		*/
		explicit Buffer(uint32_t target);
		/*!
		 \brief Deleted copy constructor.

		 \since v0.4.0
		*/
		Buffer(const Buffer&) = delete;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Buffer that will be moved

		 \since v0.6.0
		*/
		Buffer(Buffer&& rvalue) noexcept;
		/*!
		 \brief Virtual destructor.
		 \details It's needed as this class will be inherited by others.

		 \since v0.4.0
		*/
		virtual ~Buffer();
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.4.0
		*/
		Buffer& operator=(const Buffer&) = delete;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Buffer that will be moved

		 \since v0.6.0
		*/
		Buffer& operator=(Buffer&& rvalue) noexcept;
	public:
		// Public method(s)
		/*!
		 \brief Retrieves the internal OpenGL pointer to the ae::Buffer's data store by mapping it.
		 \details Mapping a buffer is useful when one doesn't possess all the necessary data when calling the function.
		 \note This internal pointer should be released once we're done using it.

		 \param[in] access The access policy indicating the intended use (GL_READ_ONLY, GL_WRITE_ONLY, GL_READ_WRITE)

		 \return The internal OpenGL pointer to the ae::Buffer's data store

		 \par Example:
		 \code
		 // Initialize the OpenGL buffer and create its data store (derived classes possess a method for creating a data store)
		 ae::Buffer buffer(GL_ARRAY_BUFFER);
		 ...
		 // Map the OpenGL buffer with the intention to write into it
		 void* bufferData = buffer.map(GL_WRITE_ONLY);
		 ...
		 // Unmap the OpenGL buffer once we're done using it
		 buffer.unmap();
		 \endcode

		 \sa mapRange(), unmap()

		 \since v0.4.0
		*/
		void* map(uint32_t access) const;
		/*!
		 \brief Retrieves the internal OpenGL pointer of the entire or part of the ae::Buffer's data store by mapping a certain range of it.
		 \details Mapping a buffer is useful when one doesn't possess all the necessary data when calling the function.\n
		 Mapping a range instead of the entire buffer is a much more performant choice if we don't wish to modify the entire data store.\n
		 Available buffer-mapping access flags:
		 \li GL_MAP_READ_BIT              - Buffer data store will be mapped for reading
		 \li GL_MAP_WRITE_BIT             - Buffer data store will be mapped for writing
		 \li GL_MAP_PERSISTENT_BIT        - Buffer data store can be mapped persistently
		 \li GL_MAP_COHERENT_BIT          - Buffer maps are to be coherent
		 \li GL_MAP_INVALIDATE_RANGE_BIT  - Tells OpenGL that you no longer care about the data in the specified range
		 \li GL_MAP_INVALIDATE_BUFFER_BIT - Tells OpenGL that you no longer care about any of the data in the whole buffer
		 \li GL_MAP_FLUSH_EXPLICIT_BIT    - You promise to tell OpenGL about any data you modify inside the mapped range
		 \li GL_MAP_UNSYNCHRONIZED_BIT    - Tells OpenGL that you will perform any synchronization yourself
		 \note This internal pointer should be released once we're done using it.

		 \param[in] offset Specifies the offset from the start of the data store's memory to retrieve, measured in bytes
		 \param[in] length Specifies the length of the data store to retrieve, measured in bytes
		 \param[in] access Specifies the combination of access flags indicating the intented use of the mapped range

		 \par Example:
		 \code
		 // Initialize the OpenGL buffer and create its data store (derived classes possess a method for creating a data store)
		 ae::Buffer buffer(GL_ARRAY_BUFFER);
		 ...
		 // Map a specific range of the OpenGL buffer with the intention to write into it and signaling that we don't care about the existing data in that range
		 void* bufferData = buffer.mapRange(0, sizeof(GLfloat) * 4, GL_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
		 ...
		 // Unmap the OpenGL buffer once we're done using it
		 buffer.unmap();
		 \endcode

		 \sa map(), unmap()

		 \since v0.4.0
		*/
		void* mapRange(int offset, int length, uint32_t access) const;
		/*!
		 \brief Releases the internal OpenGL pointer to the ae::Buffer's data store that was previously mapped.
		 \note Make sure that the ae::Buffer's data store was indeed mapped before calling this method.

		 \par Example:
		 \code
		 // Initialize the OpenGL buffer and create its data store (derived classes possess a method for creating a data store)
		 ae::Buffer buffer(GL_ARRAY_BUFFER);
		 ...
		 // Map the OpenGL buffer with the intention to write into it
		 void* bufferData = buffer.map(GL_WRITE_ONLY);
		 ...
		 // Unmap the OpenGL buffer once we're done using it
		 buffer.unmap();
		 \endcode

		 \sa map(), mapRange()
		 
		 \since v0.4.0
		*/
		void unmap() const;
		// Public virtual method(s)
		/*!
		 \brief Deletes the OpenGL handle to the ae::Buffer that was created.
		 
		 \par Example:
		 \code
		 ae::Buffer buffer(GL_ARRAY_BUFFER);
		 ...
		 // Destroy the OpenGL buffer created once we're done with it
		 buffer.destroy();
		 \endcode

		 \since v0.4.0
		*/
		virtual void destroy() const override final;
		/*!
		 \brief Binds the ae::Buffer to the context indicating to OpenGL that we're about to use it.
		 \details This method will unbind any buffer currently bound to the ae::Buffer's binding target.\n
		 The bound ae::Buffer should be unbound at the end of its use.

		 \par Example:
		 \code
		 ae::Buffer buffer(GL_ARRAY_BUFFER);
		 ...
		 buffer.bind();   // Bind the buffer to the context in order to use it
		 ...
		 buffer.unbind(); // Unbind it once we're done with it for this frame
		 \endcode

		 \since v0.4.0
		*/
		virtual void bind() const override final;
		/*!
		 \brief Unbind the ae::Buffer from the context indicating to OpenGL that we've finished using it.
		 \details Make sure that the currently-bound buffer is the caller ae::Buffer as this method will unbind any buffer with the same binding target.

		 \par Example:
		 \code
		 ae::Buffer buffer(GL_ARRAY_BUFFER);
		 ...
		 buffer.bind();   // Bind the buffer to the context in order to use it
		 ...
		 buffer.unbind(); // Unbind it once we're done with it for this frame
		 \endcode

		 \since v0.4.0
		*/
		virtual void unbind() const override final;

	protected:
		// Protected member(s)
		uint32_t mBindingTarget; //!< The binding target of the OpenGL buffer
	};
}
#endif // Aeon_Graphics_Buffer_H_

/*!
 \class ae::Buffer
 \ingroup graphics

 The ae::Buffer base class represents a generalized OpenGL buffer used to
 supply data to OpenGL, whether it be vertex data, uniform data, index data,
 etc.

 The derived classes offer more specific functionalities pertaining to their
 use and should be used instead of the ae::Buffer base class.

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2020.08.27
 \copyright MIT License
*/