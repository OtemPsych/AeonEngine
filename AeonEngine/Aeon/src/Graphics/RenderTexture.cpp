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

#include <AEON/Graphics/RenderTexture.h>

#include <AEON/Window/internal/EventQueue.h>
#include <AEON/Window/ContextSettings.h>
#include <AEON/Graphics/Camera2D.h>

namespace ae
{
	// Public constructor(s)
	RenderTexture::RenderTexture()
		: RenderTarget()
		, mFramebuffer(nullptr)
		, mTexture(nullptr)
		, mDepthTexture(nullptr)
		, mStencilTexture(nullptr)
		, mColorProperties({ Texture2D::Filter::Nearest, Texture2D::Wrap::None, Texture2D::InternalFormat::RGBA8 })
		, mDepthProperties({ Texture2D::Filter::Nearest, Texture2D::Wrap::None, Texture2D::InternalFormat::Native })
		, mStencilProperties({ Texture2D::Filter::Nearest, Texture2D::Wrap::None, Texture2D::InternalFormat::Native })
	{
		// Assign a 2D camera to the render target
		setCamera(Camera2D(true));
	}

	// Public method(s)
	void RenderTexture::create(unsigned int width, unsigned int height)
	{
		// Delete previous framebuffer and textures
		if (mFramebuffer) {
			mFramebuffer->destroy();
			mFramebuffer.reset();
		}
		if (mTexture) {
			mTexture->destroy();
			mTexture.reset();
		}
		if (mDepthTexture) {
			mDepthTexture->destroy();
			mDepthTexture.reset();
		}
		if (mStencilTexture) {
			mStencilTexture->destroy();
			mStencilTexture.reset();
		}

		// Create the framebuffer object
		mFramebuffer = std::make_unique<Framebuffer>();
		
		// Create the color buffer texture
		mTexture = std::make_unique<Texture2D>(mColorProperties.filter, mColorProperties.wrap, mColorProperties.format);
		if (!mTexture->create(width, height)) {
			AEON_LOG_ERROR("Invalid dimensions", "The dimensions " + std::to_string(width) + "x" + std::to_string(height) + " provided for the color buffer are invalid.");
			return;
		}

		// Create the depth buffer and stencil buffer texture (if requested)
		if (mDepthProperties.format != Texture2D::InternalFormat::Native) {
			mDepthTexture = std::make_unique<Texture2D>(mDepthProperties.filter, mDepthProperties.wrap, mDepthProperties.format);
			if (!mDepthTexture->create(width, height)) {
				AEON_LOG_ERROR("Invalid dimensions", "The dimensions " + std::to_string(width) + "x" + std::to_string(height) + " provided for the depth buffer are invalid.");
				return;
			}
		}
		if (mStencilProperties.format != Texture2D::InternalFormat::Native) {
			mStencilTexture = std::make_unique<Texture2D>(mStencilProperties.filter, mStencilProperties.wrap, mStencilProperties.format);
			if (!mStencilTexture->create(width, height)) {
				AEON_LOG_ERROR("Invalid dimensions", "The dimensions " + std::to_string(width) + "x" + std::to_string(height) + " provided for the stencil buffer are invalid.");
				return;
			}
		}

		// Attach the textures created to the framebuffer
		mFramebuffer->attachTexture(mTexture.get());
		if (mDepthTexture) mFramebuffer->attachTexture(mDepthTexture.get());
		if (mStencilTexture) mFramebuffer->attachTexture(mStencilTexture.get());

		// Set the framebuffer size
		mFramebufferSize.x = width;
		mFramebufferSize.y = height;

		// Enqueue framebuffer resize event
		auto event = std::make_unique<FramebufferResizeEvent>(mFramebufferSize.x, mFramebufferSize.y, getFramebufferHandle());
		EventQueue::getInstance().enqueueEvent(std::move(event));
	}

	void RenderTexture::setColorProperties(Texture2D::Filter filter, Texture2D::Wrap wrap, Texture2D::InternalFormat format) noexcept
	{
		if (filter != Texture2D::Filter::None && filter != Texture2D::Filter::Nearest && filter != Texture2D::Filter::Linear) {
			AEON_LOG_WARNING("Invalid filter", "The filter provided to the color buffer is invalid.\nNo effect.");
		}
		else {
			mColorProperties.filter = filter;
		}

		mColorProperties.wrap = wrap;

		if (format != Texture2D::InternalFormat::R8 && format != Texture2D::InternalFormat::R16 && format != Texture2D::InternalFormat::RG8
			&& format != Texture2D::InternalFormat::RG16 && format != Texture2D::InternalFormat::RGB8 && format != Texture2D::InternalFormat::RGBA8
			&& format != Texture2D::InternalFormat::RGBA16) {
			AEON_LOG_WARNING("Invalid internal format", "The internal format provided to the color buffer is invalid.\nNo effect.");
		}
		else {
			mColorProperties.format = format;
		}
	}

	void RenderTexture::setDepthProperties(Texture2D::Filter filter, Texture2D::Wrap wrap, Texture2D::InternalFormat format) noexcept
	{
		if (filter == Texture2D::Filter::None || filter == Texture2D::Filter::Nearest || filter == Texture2D::Filter::Linear) {
			mDepthProperties.filter = filter;
		}
		else {
			AEON_LOG_WARNING("Invalid filter", "The filter provided to the depth buffer is invalid.\nNo effect.");
		}

		mDepthProperties.wrap = wrap;

		if (format == Texture2D::InternalFormat::Native || format == Texture2D::InternalFormat::DEPTH32 || format == Texture2D::InternalFormat::DEPTH24
			|| format == Texture2D::InternalFormat::DEPTH16 || format == Texture2D::InternalFormat::DEPTH32STENCIL || format == Texture2D::InternalFormat::DEPTH24STENCIL) {
			mDepthProperties.format = format;
		}
		else {
			AEON_LOG_WARNING("Invalid internal format", "The internal format provided to the depth buffer is invalid.\nNo effect.");
		}
	}

	void RenderTexture::setStencilProperties(Texture2D::Filter filter, Texture2D::Wrap wrap, Texture2D::InternalFormat format) noexcept
	{
		if (filter == Texture2D::Filter::None || filter == Texture2D::Filter::Nearest || filter == Texture2D::Filter::Linear) {
			mStencilProperties.filter = filter;
		}
		else {
			AEON_LOG_WARNING("Invalid filter", "The filter provided to the stencil buffer is invalid.\nNo effect.");
		}

		mStencilProperties.wrap = wrap;

		if (format == Texture2D::InternalFormat::Native || format == Texture2D::InternalFormat::STENCIL) {
			mStencilProperties.format = format;
		}
		else {
			AEON_LOG_WARNING("Invalid internal format", "The internal format provided to the stencil buffer is invalid.\nNo effect.");
		}
	}

	const Texture2D* const RenderTexture::getTexture() const noexcept
	{
		return mTexture.get();
	}

	// Public virtual method(s)
	unsigned int RenderTexture::getFramebufferHandle() const noexcept
	{
		return mFramebuffer->getHandle();
	}
}