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

#include <AEON/Graphics/Texture.h>

#include <string>

#include <GL/glew.h>

#include <AEON/Graphics/internal/GLCommon.h>

namespace ae
{
	// Public constructor(s)
	Texture::~Texture()
	{
	}

	// Public method(s)
	void Texture::bind(int unit) const
	{
		// Check if the OpenGL handle is valid before attempting to bind it (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (!mHandle) {
				AEON_LOG_ERROR("Invalid OpenGL handle", "The texture's OpenGL handle \"" + std::to_string(mHandle) + "\" is invalid.\nAborting binding.");
				return;
			}
		}

		// Select the unit texture index provided and bind it to the context
		GLCall(glBindTextureUnit(unit, mHandle));
	}

	void Texture::generateMipmap(Filter filter)
	{
		// Generate the mipmap
		GLCall(glGenerateTextureMipmap(mHandle));

		// Set the filter to apply to the texture/mip levels
		mHasMipmap = true;
		setFilter(filter);
	}

	void Texture::setFilter(Filter filter)
	{
		// Update the filter subject to the following conditions
		if (mFilter != filter && (mHasMipmap || filter == Filter::None || filter == Filter::Nearest || filter == Filter::Linear)) {
			mFilter = filter;
			
			// Find the appropriate magnification filter as it can only use GL_NEAREST or GL_LINEAR
			GLint GLMagFilter = (mFilter == Filter::Linear
			                  || mFilter == Filter::Linear_MipLinear
			                  || mFilter == Filter::Linear_MipNearest) ? GL_LINEAR : GL_NEAREST;

			// Apply the filter chosen
			GLCall(glTextureParameteri(mHandle, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(mFilter)));
			GLCall(glTextureParameteri(mHandle, GL_TEXTURE_MAG_FILTER, GLMagFilter));
		}
	}

	Texture::Filter Texture::getFilter() const noexcept
	{
		return mFilter;
	}

	Texture::Wrap Texture::getWrap() const noexcept
	{
		return mWrap;
	}

	Texture::InternalFormat Texture::getInternalFormat() const noexcept
	{
		return mFormat.internal;
	}

	bool Texture::hasMipmap() const noexcept
	{
		return mHasMipmap;
	}

	// Public virtual method(s)
	void Texture::setWrap(Wrap wrap)
	{
		if (mWrap != wrap) {
			mWrap = wrap;

			// Apply the wrapping mode chosen to the coordinates U (or S or X)
			GLCall(glTextureParameteri(mHandle, GL_TEXTURE_WRAP_S, static_cast<GLint>(mWrap)));
		}
	}

	void Texture::destroy() const
	{
		// Check if the OpenGL identifier is valid (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (!mHandle) {
				AEON_LOG_ERROR("Attempt to delete invalid texture", "The texture's OpenGL identifier is invalid.\nAborting operation.");
				return;
			}
		}

		GLCall(glDeleteTextures(1, &mHandle));
	}

	void Texture::bind() const
	{
		// Bind the texture to the context using the first unit texture
		bind(0);
	}

	void Texture::unbind() const
	{
		// Unbind the currently-bound texture at the first unit texture
		GLCall(glBindTextureUnit(0, 0));
	}

	// Protected constructor(s)
	Texture::Texture(uint32_t target, Filter filter, Wrap wrap, InternalFormat internalFormat)
		: GLResource()
		, mFormat(internalFormat)
		, mWrap(Wrap::None)
		, mBindingTarget(target)
		, mHasMipmap(false)
		, mFilter(Filter::None)
	{
		// Create the texture object
		GLCall(glCreateTextures(mBindingTarget, 1, &mHandle));

		// Set the filter type and the wrapping mode
		setFilter(filter);
		setWrap(wrap);
	}

	// Texture::Format
		// Public constructor(s)
	Texture::Format::Format(InternalFormat internalFormat) noexcept
		: internal(internalFormat)
		, base(GL_NONE)
		, imposedChannels(0)
		, bitCount(8)
	{
		switch (internal)
		{
		case InternalFormat::R8:
			base = GL_RED;
			imposedChannels = 1;
			break;
		case InternalFormat::R16:
			base = GL_RED;
			imposedChannels = 1;
			bitCount = 16;
			break;
		case InternalFormat::RG8:
			base = GL_RG;
			imposedChannels = 2;
			break;
		case InternalFormat::RG16:
			base = GL_RG;
			imposedChannels = 2;
			bitCount = 16;
			break;
		case InternalFormat::RGB8:
			base = GL_RGB;
			imposedChannels = 3;
			break;
		case InternalFormat::RGBA8:
			base = GL_RGBA;
			imposedChannels = 4;
			break;
		case InternalFormat::RGBA16:
			base = GL_RGBA;
			imposedChannels = 4;
			bitCount = 16;
		}
	}
}