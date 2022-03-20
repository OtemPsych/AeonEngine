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

#include <cstdint>

#include <AEON/Config.h>

namespace ae
{
	/*!
	 \brief The abstract base class used to represent an OpenGL resource.
	 \note This class is considered to be internal but may still be used by the API user.
	*/
	class AEON_API GLResource
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Deleted copy constructor.

		 \since v0.6.0
		*/
		GLResource(const GLResource&) = delete;
		/*!
		 \brief Move constructor.

		 \since v0.7.0
		*/
		GLResource(GLResource&&) noexcept = default;
		/*!
		 \brief Virtual destructor.
		 \details It's needed as this class will be inherited by others.

		 \since v0.7.0
		*/
		virtual ~GLResource() = default;

		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.6.0
		*/
		GLResource& operator=(const GLResource&) = delete;
		/*!
		 \brief Move assignment operator.

		 \return The caller ae::GLResource

		 \since v0.7.0
		*/
		GLResource& operator=(GLResource&&) noexcept = default;

		// Public method(s)
		/*!
		 \brief Retrieves the OpenGL identifier of the ae::GLResource.
		 \note This method should primarily be used internally, the API user shouldn't ever have need of this identifier.

		 \return The ae::GLResource's OpenGL identifier

		 \since v0.7.0
		*/
		[[nodiscard]] inline uint32_t getHandle() const noexcept { return mHandle; }

		// Public virtual method(s)
		/*!
		 \brief Deletes the OpenGL identifier that was created.
		 \note This virtual method has to be overloaded by all derived classes.

		 \since v0.4.0
		*/
		virtual void destroy() const = 0;
		/*!
		 \brief Binds the ae::GLResource to the context indicating to OpenGL that we're about to use it.
		 \note This virtual method has to be overloaded by all derived classes.

		 \sa unbind()

		 \since v0.4.0
		*/
		virtual void bind() const = 0;
		/*!
		 \brief Unbinds the ae::GLResource from the context indicating to OpenGL that we've finished using it.
		 \details Make sure that the currently-bound resource is the caller ae::GLResource as this method will unbind any resource of the same type.
		 \note This virtual method has to be overloaded by all derived classes.

		 \sa bind()

		 \since v0.4.0
		*/
		virtual void unbind() const = 0;
	protected:
		// Protected constructor(s)
		/*!
		 \brief Default constructor.
		 \details Set the OpenGL identifier to 0 (an invalid identifier).

		 \since v0.4.0
		*/
		GLResource();

		// Protected member(s)
		uint32_t mHandle; //!< The OpenGL identifier to the resource
	};
}

/*!
 \class ae::GLResource
 \ingroup graphics

 The ae::GLResource abstract base class is meant to be inherited by all classes
 that represent an OpenGL resource, such as: VAOs, VBOs, IBOs, Shaders, etc.
 This class is considered internal and the average API user shouldn't have need
 of it.

 All derived classes are forced to overload certain virtual methods and should
 only be constructed through the ae::GLResourceFactory singleton class that
 also acts as a manager for these OpenGL resources localizing the destruction
 of allocated resources on the GPU.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.12.27
 \copyright MIT License
*/