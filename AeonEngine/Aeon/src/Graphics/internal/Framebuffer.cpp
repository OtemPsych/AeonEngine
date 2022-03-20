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

#include <AEON/Graphics/internal/Framebuffer.h>

#include <string>
#include <vector>

#include <GL/glew.h>

#include <AEON/Graphics/internal/GLCommon.h>
#include <AEON/Graphics/Texture.h>

namespace ae
{
	// Public constructor(s)
	Framebuffer::Framebuffer()
		: GLResource()
		, mColorAttachmentCount(0)
	{
		GLCall(glCreateFramebuffers(1, &mHandle));
	}

	Framebuffer::Framebuffer(Framebuffer&& rvalue) noexcept
		: GLResource(std::move(rvalue))
		, mColorAttachmentCount(rvalue.mColorAttachmentCount)
	{
	}

	// Public operator(s)
	Framebuffer& Framebuffer::operator=(Framebuffer&& rvalue) noexcept
	{
		// Copy the rvalue's trivial data and move the rest
		GLResource::operator=(std::move(rvalue));
		mColorAttachmentCount = rvalue.mColorAttachmentCount;

		return *this;
	}

	// Public method(s)
	void Framebuffer::attachTexture(const Texture* const texture)
	{
		// Make sure that there aren't more than 15 textures attached to a color buffer (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (mColorAttachmentCount > 15) {
				AEON_LOG_ERROR("Max color attachments reached", "Attempt to attach more than 15 textures to a color buffer.\nAborting operation.");
				return;
			}
		}

		// Find the attachment type of the texture provided
		const Texture::InternalFormat FORMAT = texture->getInternalFormat();
		GLenum attachment;
		switch (FORMAT)
		{
		case Texture::InternalFormat::DEPTH32:
		case Texture::InternalFormat::DEPTH24:
		case Texture::InternalFormat::DEPTH16:
			attachment = GL_DEPTH_ATTACHMENT;
			break;
		case Texture::InternalFormat::DEPTH32STENCIL:
		case Texture::InternalFormat::DEPTH24STENCIL:
			attachment = GL_DEPTH_STENCIL_ATTACHMENT;
			break;
		case Texture::InternalFormat::STENCIL:
			attachment = GL_STENCIL_ATTACHMENT;
			break;
		default:
			attachment = GL_COLOR_ATTACHMENT0 + mColorAttachmentCount++;
		}

		// Attach the texture
		GLCall(glNamedFramebufferTexture(mHandle, attachment, texture->getHandle(), 0));

		// Set the buffers that will be rendered into
		if (mColorAttachmentCount > 0) {
			std::vector<GLenum> drawBuffers;
			drawBuffers.reserve(mColorAttachmentCount);
			for (size_t i = 0; i < mColorAttachmentCount; ++i) {
				drawBuffers.emplace_back(GL_COLOR_ATTACHMENT0 + i);
			}

			GLCall(glNamedFramebufferDrawBuffers(mHandle, static_cast<GLsizei>(drawBuffers.size()), drawBuffers.data()));

			// Check that the framebuffer is complete (ignored in Release mode)
			if _CONSTEXPR_IF (AEON_DEBUG) {
				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
					AEON_LOG_ERROR("Incomplete framebuffer", "The framebuffer wasn't correctly set up.");
				}
			}
		}
	}

	// Public virtual method(s)
	void Framebuffer::destroy() const
	{
		// Check if the OpenGL handle is valid before attempting to delete it (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (!mHandle) {
				AEON_LOG_ERROR("Invalid OpenGL handle", "Attempt to destroy default back buffer.\nAborting destruction.");
				return;
			}
		}

		GLCall(glDeleteFramebuffers(1, &mHandle));
	}

	void Framebuffer::bind() const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, mHandle));
	}

	void Framebuffer::unbind() const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
}