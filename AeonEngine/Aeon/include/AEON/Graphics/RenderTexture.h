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

#ifndef Aeon_Graphics_RenderTexture_H_
#define Aeon_Graphics_RenderTexture_H_

#include <AEON/Graphics/internal/Framebuffer.h>
#include <AEON/Graphics/internal/RenderTarget.h>
#include <AEON/Graphics/Texture2D.h>

namespace ae
{
	// Forward declaration(s)
	class Framebuffer;

	/*!
	 \brief Class used for off-screen rendering.
	*/
	class AEON_API RenderTexture : public RenderTarget
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets the the color buffer's properties to nearest filtering, no wrapping mode and RGBA8 format.
		 No depth or stencil buffers are created.

		 \since v0.7.0
		*/
		RenderTexture();
		/*!
		 \brief Deleted copy constructor.

		 \since v0.5.0
		*/
		RenderTexture(const RenderTexture&) = delete;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::RenderTexture that will be moved

		 \since v0.7.0
		*/
		RenderTexture(RenderTexture&& rvalue) noexcept = default;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.5.0
		*/
		RenderTexture& operator=(const RenderTexture&) = delete;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::RenderTexture that will be moved

		 \return The caller ae::RenderTexture

		 \since v0.7.0
		*/
		RenderTexture& operator=(RenderTexture&& rvalue) noexcept = default;
	public:
		// Public method(s)
		/*!
		 \brief Creates the render texture by providing the dimensions \a width x \a height.
		 \note The dimensions provided should optimally be even numbers for correct results.

		 \param[in] width The render texture's width
		 \param[in] height The render texture's height

		 \par Example:
		 \code
		 // Creates a standard 250x250 render texture with a color buffer with nearest filtering, no wrapping mode and RGBA8 format
		 ae::RenderTexture rtexture1;
		 rtexture1.create(250, 250);

		 // Creates a 250x250 render texture with a default color buffer and depth buffer of 24 bits
		 ae::RenderTexture rtexture2;
		 rtexture2.setDepthProperties(ae::Texture::Filter::Nearest, ae::Texture::Wrap::None, ae::Texture::InternalFormat::DEPTH24);
		 rtexture2.create(250, 250);

		 // Creates a 250x250 render texture with a default color buffer, a depth buffer of 24 bits and a stencil buffer of 8 bits
		 ae::RenderTexture rtexture3;
		 rtexture3.setDepthProperties(ae::Texture::Filter::Nearest, ae::Texture::Wrap::None, ae::Texture::InternalFormat::DEPTH24);
		 rtexture3.setStencilProperties(ae::Texture::Filter::Nearest, ae::Texture::Wrap::None, ae::Texture::InternalFormat::STENCIL);
		 rtexture3.create(250, 250);
		 \endcode

		 \sa getTexture(), setColorProperties()

		 \since v0.7.0
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
		/*!
		 \brief Sets the properties of the color buffer.
		 \details Available filters:
		 \li None
		 \li Nearest
		 \li Linear
		 Available internal formats:
		 \li R8
		 \li R16
		 \li RG8
		 \li RG16
		 \li RGB8
		 \li RGBA8
		 \li RGBA16
		 \note The render texture must be recreated after calling this method.

		 \param[in] filter The ae::Texture2D::Filter to apply, ae::Texture2D::Filter::Nearest by default
		 \param[in] wrap The ae::Texture2D::Wrap to apply, ae::Texture2D::Wrap::None by default
		 \param[in] format The ae::Texture2D::InternalFormat to apply, ae::Texture2D::InternalFormat::RGBA8 by default

		 \sa setDepthProperties(), setStencilProperties()

		 \since v0.7.0
		*/
		void setColorProperties(Texture2D::Filter filter = Texture2D::Filter::Nearest, Texture2D::Wrap wrap = Texture2D::Wrap::None, Texture2D::InternalFormat format = Texture2D::InternalFormat::RGBA8) noexcept;
		/*!
		 \brief Sets the properties of the depth buffer.
		 \details Available filters:
		 \li None
		 \li Nearest
		 \li Linear
		 Available internal formats:
		 \li Native (no depth buffer will be created)
		 \li DEPTH32
		 \li DEPTH24
		 \li DEPTH16
		 \li DEPTH32STENCIL (depth and stencil buffer combined)
		 \li DEPTH24STENCIL (depth and stencil buffer combined)
		 \note The render texture must be recreated after calling this method.

		 \param[in] filter The ae::Texture2D::Filter to apply, ae::Texture2D::Filter::Nearest by default
		 \param[in] wrap The ae::Texture2D::Wrap to apply, ae::Texture2D::Wrap::None by default
		 \param[in] format The ae::Texture2D::InternalFormat to apply, ae::Texture2D::InternalFormat::Native by default

		 \sa setColorProperties(), setStencilProperties()

		 \since v0.7.0
		*/
		void setDepthProperties(Texture2D::Filter filter = Texture2D::Filter::Nearest, Texture2D::Wrap wrap = Texture2D::Wrap::None, Texture2D::InternalFormat format = Texture2D::InternalFormat::Native) noexcept;
		/*!
		 \brief Sets the properties of the stencil buffer.
		 \details Available filters:
		 \li None
		 \li Nearest
		 \li Linear
		 Available internal formats:
		 \li Native (no stencil buffer will be created)
		 \li STENCIL
		 \note The render texture must be recreated after calling this method.

		 \param[in] filter The ae::Texture2D::Filter to apply, ae::Texture2D::Filter::Nearest by default
		 \param[in] wrap The ae::Texture2D::Wrap to apply, ae::Texture2D::Wrap::None by default
		 \param[in] format The ae::Texture2D::InternalFormat to apply, ae::Texture2D::InternalFormat::Native by default

		 \sa setColorProperties(), setDepthProperties()

		 \since v0.7.0
		*/
		void setStencilProperties(Texture2D::Filter filter = Texture2D::Filter::Nearest, Texture2D::Wrap wrap = Texture2D::Wrap::None, Texture2D::InternalFormat format = Texture2D::InternalFormat::Native) noexcept;

		// Public virtual method(s)
		/*!
		 \brief Retrieves the ae::RenderTexture's internal framebuffer handle.
		 \note This shouldn't be needed by the API user.

		 \return The internal OpenGL framebuffer handle

		 \since v0.5.0
		*/
		_NODISCARD virtual unsigned int getFramebufferHandle() const noexcept override final;

	private:
		// Private struct(s)
		/*!
		 \brief The properties of the color, depth and stencil textures needed.
		*/
		struct TextureProperties
		{
			Texture2D::Filter         filter; //!< The filtering type to apply
			Texture2D::Wrap           wrap;   //!< The wrapping mode to employ once the normalized coordinates aren't in the range [0,1]
			Texture2D::InternalFormat format; //!< The information regarding the image data's format
		};
	private:
		// Private member(s)
		std::unique_ptr<Framebuffer> mFramebuffer;    //!< The OpenGL framebuffer object
		std::unique_ptr<Texture2D>   mTexture;        //!< The color channel texture
		std::unique_ptr<Texture2D>   mDepthTexture;   //!< The depth/stencil channel texture
		std::unique_ptr<Texture2D>   mStencilTexture; //!< The stencil channel texture
		TextureProperties            mColorProperties;    //!< The color buffer's properties
		TextureProperties            mDepthProperties;    //!< The depth buffer's properties
		TextureProperties            mStencilProperties;  //!< The stencil buffer's properties
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
 \version v0.7.0
 \date 2021.07.15
 \copyright MIT License
*/