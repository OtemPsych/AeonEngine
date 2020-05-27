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

#ifndef Aeon_Graphics_Framebuffer_H_
#define Aeon_Graphics_Framebuffer_H_

#include <AEON/Graphics/internal/GLResource.h>

namespace ae
{
	// Forward declaration(s)
	class Texture;

	/*!
	 \brief The class representing an OpenGL framebuffer object used for off-screen rendering.
	 \note This class is considered to be internal but may still be used by the API user.
	*/
	class _NODISCARD AEON_API Framebuffer : public GLResource
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Generates a unique OpenGL identifier.

		 \since v0.4.0
		*/
		Framebuffer();
		/*!
		 \brief Deleted copy constructor.

		 \since v0.4.0
		*/
		Framebuffer(const Framebuffer&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.4.0
		*/
		Framebuffer(Framebuffer&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.4.0
		*/
		Framebuffer& operator=(const Framebuffer&) = delete;
		/*!
		 \brief Deleted move assignment operator.
		 
		 \since v0.4.0
		*/
		Framebuffer& operator=(Framebuffer&&) = delete;
	public:
		// Public method(s)
		/*!
		 \brief Attaches a texture that will serve as the storage for the rendering.
		 \details The attachment type is based on the provided texture's internal format.
		 If the texture has a color format, it will be attached to a color buffer (up to 15 max).
		 If the texture has a depth format, it will be attached to the depth buffer, etc.

		 \param[in] texture Base pointer to an ae::Texture

		 \since v0.4.0
		*/
		void attachTexture(const Texture* const texture);

		// Public virtual method(s)
		/*!
		 \brief Deletes the OpenGL handle to the ae::Framebuffer that was created.
		 \details This method is called automatically when the ae::Framebuffer was created by the ae::GLResourceFactory instance.

		 \since v0.4.0
		*/
		virtual void destroy() const override final;
		/*!
		 \brief Binds the ae::Framebuffer to the context indicating to OpenGL that we're about to use it.
		 \details This method will unbind any currently bound framebuffer (including the default one).

		 \since v0.4.0
		*/
		virtual void bind() const override final;
		/*!
		 \brief Unbind the ae::Framebuffer from the context indicating to OpenGL that we've finished using it.
		 \details This method will set the active framebuffer to the default one, which is the window's back buffer.

		 \since v0.4.0
		*/
		virtual void unbind() const override final;

	private:
		// Private member(s)
		size_t mColorAttachmentCount; //!< The number of textures attached to a color buffer
	};
}
#endif // Aeon_Graphics_Framebuffer_H_

/*!
 \class ae::Framebuffer
 \ingroup graphics

 The ae::Framebuffer class is used for off-screen rendering, such as rendering
 to a render texture.

 \author Filippos Gleglakos
 \version v0.4.0
 \date 2020.05.20
 \copyright MIT License
*/