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

#ifndef Aeon_Graphics_RenderTexture_H_
#define Aeon_Graphics_RenderTexture_H_

#include <AEON/Graphics/internal/RenderTarget.h>
#include <AEON/Graphics/Texture2D.h>

namespace ae
{
	// Forward declaration(s)
	class Framebuffer;

	/*!
	 \brief Class used for off-screen rendering.
	*/
	class _NODISCARD AEON_API RenderTexture : public RenderTarget
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::RenderTexture by providing an optional internal format for the color buffer, the depth buffer and the stencil buffer.
		 \details Available color buffer formats:
		 \li Native (RGBA8 will be chosen)
		 \li R8
		 \li R16
		 \li RG8
		 \li RG16
		 \li RGB8
		 \li RGBA8
		 \li RGBA16

		 Available depth buffer formats:
		 \li Native (no depth buffer will be created)
		 \li DEPTH32
		 \li DEPTH24
		 \li DEPTH16
		 \li DEPTH32STENCIL (depth and stencil buffer combined)
		 \li DEPTH24STENCIL (depth and stencil buffer combined)

		 Available stencil buffer formats:
		 \li Native (no stencil buffer will be created)
		 \li STENCIL

		 \note The buffer formats won't be checked, unexpected results may occur if the wrong format is chosen.

		 \param[in] colorFormat The ae::Texture2D::InternalFormat of the color buffer's image data, ae::Texture2D::InternalFormat::RGBA8 by default
		 \param[in] depthFormat The ae::Texture2D::InternalFormat of the depth buffer's data, ae::Texture2D::InternalFormat::Native by default
		 \param[in] stencilFormat The ae::Texture2D::InternalFormat of the stencil buffer's data, ae::Texture2D::InternalFormat::Native by default

		 \since v0.5.0
		*/
		explicit RenderTexture(Texture2D::InternalFormat colorFormat = Texture2D::InternalFormat::RGBA8, Texture2D::InternalFormat depthFormat = Texture2D::InternalFormat::Native, Texture2D::InternalFormat stencilFormat = Texture2D::InternalFormat::Native);
		/*!
		 \brief Deleted copy constructor.

		 \since v0.5.0
		*/
		RenderTexture(const RenderTexture&) = delete;
		/*!
		 \brief Move constructor.

		 \since v0.5.0
		*/
		RenderTexture(RenderTexture&&) noexcept = default;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.5.0
		*/
		RenderTexture& operator=(const RenderTexture&) = delete;
		/*!
		 \brief Move assignment operator.

		 \since v0.5.0
		*/
		RenderTexture& operator=(RenderTexture&&) noexcept = default;
	public:
		// Public method(s)
		/*!
		 \brief Creates the render texture by providing the dimensions \a width x \a height.
		 \note The dimensions provided should optimally be even numbers for correct results.

		 \param[in] width The render texture's width
		 \param[in] height The render texture's height

		 \par Example:
		 \code
		 // Creates a standard 250x250 render texture with a color buffer
		 ae::RenderTexture rtexture1;
		 rtexture1.create(250, 250);

		 // Creates a 250x250 render texture with a color buffer and depth buffer of 24 bits
		 ae::RenderTexture rtexture2(ae::Texture::InternalFormat::RGBA8, ae::Texture::InternalFormat::DEPTH24);
		 rtexture2.create(250, 250);

		 // Creates a 250x250 render texture with a color buffer, a depth buffer of 24 bits and a stencil buffer of 8 bits
		 ae::RenderTexture rtexture3(ae::Texture::InternalFormat::RGBA8, ae::Texture::InternalFormat::DEPTH24, ae::Texture::InternalFormat::STENCIL);
		 rtexture3.create(250, 250);
		 \endcode

		 \sa getTexture()

		 \since v0.5.0
		*/
		void create(unsigned int width, unsigned int height);
		/*!
		 \brief Retrieves the ae::RenderTexture's target texture.
		 \details The target texture is the off-screen rendering target.

		 \return The ae::Texture2D that's being used for off-screen rendering

		 \sa create()

		 \since v0.5.0
		*/
		_NODISCARD const Texture2D* const getTexture() const noexcept;

		// Public virtual method(s)
		/*!
		 \brief Retrieves the ae::RenderTexture's internal framebuffer handle.
		 \note This shouldn't be needed by the API user.

		 \return The internal OpenGL framebuffer handle

		 \since v0.5.0
		*/
		_NODISCARD virtual unsigned int getFramebufferHandle() const noexcept override final;

	private:
		// Private member(s)
		std::shared_ptr<Framebuffer> mFramebuffer;    //!< The OpenGL framebuffer object
		std::shared_ptr<Texture2D>   mTexture;        //!< The color channel texture
		std::shared_ptr<Texture2D>   mDepthTexture;   //!< The depth/stencil channel texture
		std::shared_ptr<Texture2D>   mStencilTexture; //!< The stencil channel texture

		Texture2D::InternalFormat    mColorFormat;    //!< The color buffer's internal format
		Texture2D::InternalFormat    mDepthFormat;    //!< The depth buffer's internal format
		Texture2D::InternalFormat    mStencilFormat;  //!< The stencil buffer's internal format
	};
}
#endif // Aeon_Graphics_RenderTexture_H_

/*!
 \class ae::RenderTexture
 \ingroup graphics

 The ae::RenderTexture is used as a target for off-screen rendering into a
 texture. The generated texture can then be used as a normal texture and
 assigned to a sprite.

 \author Filippos Gleglakos
 \version v0.5.0
 \date 2020.07.30
 \copyright MIT License
*/