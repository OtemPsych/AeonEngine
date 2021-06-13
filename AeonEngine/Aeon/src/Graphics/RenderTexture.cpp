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

#include <AEON/Graphics/RenderTexture.h>

#include <AEON/Window/ContextSettings.h>
#include <AEON/Graphics/GLResourceFactory.h>
#include <AEON/Graphics/Camera2D.h>

namespace ae
{
	// Public constructor(s)
	RenderTexture::RenderTexture(Texture2D::InternalFormat colorFormat, Texture2D::InternalFormat depthFormat, Texture2D::InternalFormat stencilFormat)
		: RenderTarget()
		, mFramebuffer(nullptr)
		, mTexture(nullptr)
		, mDepthTexture(nullptr)
		, mStencilTexture(nullptr)
		, mColorFormat((colorFormat == Texture2D::InternalFormat::Native) ? Texture2D::InternalFormat::RGBA8 : colorFormat)
		, mDepthFormat(depthFormat)
		, mStencilFormat(stencilFormat)
	{
		// Assign a 2D camera to the render target
		setCamera(Camera2D(true));
	}

	RenderTexture::RenderTexture(RenderTexture&& rvalue) noexcept
		: RenderTarget(std::move(rvalue))
		, mFramebuffer(std::move(rvalue.mFramebuffer))
		, mTexture(std::move(rvalue.mTexture))
		, mDepthTexture(std::move(rvalue.mDepthTexture))
		, mStencilTexture(std::move(rvalue.mStencilTexture))
		, mColorFormat(rvalue.mColorFormat)
		, mDepthFormat(rvalue.mDepthFormat)
		, mStencilFormat(rvalue.mStencilFormat)
	{
	}

	// Public operator(s)
	RenderTexture& RenderTexture::operator=(RenderTexture&& rvalue) noexcept
	{
		// Copy the rvalue's trivial data and move the rest
		RenderTarget::operator=(std::move(rvalue));
		mFramebuffer = std::move(rvalue.mFramebuffer);
		mTexture = std::move(rvalue.mTexture);
		mDepthTexture = std::move(rvalue.mDepthTexture);
		mStencilTexture = std::move(rvalue.mStencilTexture);
		mColorFormat = rvalue.mColorFormat;
		mDepthFormat = rvalue.mDepthFormat;
		mStencilFormat = rvalue.mStencilFormat;

		return *this;
	}

	// Public method(s)
	void RenderTexture::create(unsigned int width, unsigned int height)
	{
		// Create the framebuffer object
		GLResourceFactory& factory = GLResourceFactory::getInstance();
		mFramebuffer = factory.create<Framebuffer>("");
		
		// Create the color buffer texture
		mTexture = factory.create<Texture2D>("", Texture2D::Filter::Nearest, Texture2D::Wrap::None, mColorFormat);
		if (!mTexture->create(width, height)) {
			AEON_LOG_ERROR("Invalid dimensions", "The dimensions " + std::to_string(width) + "x" + std::to_string(height) + " provided for the color buffer are invalid.");
			return;
		}

		// Create the depth buffer and stencil buffer texture (if requested)
		if (mDepthFormat != Texture2D::InternalFormat::Native) {
			mDepthTexture = factory.create<Texture2D>("", Texture2D::Filter::Nearest, Texture2D::Wrap::None, mDepthFormat);
			if (!mDepthTexture->create(width, height)) {
				AEON_LOG_ERROR("Invalid dimensions", "The dimensions " + std::to_string(width) + "x" + std::to_string(height) + " provided for the depth buffer are invalid.");
				return;
			}
		}
		if (mStencilFormat != Texture2D::InternalFormat::Native) {
			mStencilTexture = factory.create<Texture2D>("", Texture2D::Filter::Nearest, Texture2D::Wrap::None, mStencilFormat);
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