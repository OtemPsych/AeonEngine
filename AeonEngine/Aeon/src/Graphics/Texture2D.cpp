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

#include <AEON/Graphics/Texture2D.h>

#include <string>

#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>

#include <AEON/Graphics/internal/GLCommon.h>

namespace ae
{
	// Public constructor(s)
	Texture2D::Texture2D(Filter filter, Wrap wrap, InternalFormat internalFormat)
		: Texture(GL_TEXTURE_2D, filter, wrap, internalFormat)
		, mFilepath("")
		, mSize(0, 0)
	{
	}

	Texture2D::Texture2D(Texture2D&& rvalue) noexcept
		: Texture(std::move(rvalue))
		, mFilepath(std::move(rvalue.mFilepath))
		, mSize(std::move(rvalue.mSize))
	{
	}

	// Public operator(s)
	Texture2D& Texture2D::operator=(Texture2D&& rvalue) noexcept
	{
		// Move the rvalue's data
		Texture::operator=(std::move(rvalue));
		mFilepath = std::move(rvalue.mFilepath);
		mSize = std::move(rvalue.mSize);

		return *this;
	}

	// Public method(s)
	bool Texture2D::create(unsigned int width, unsigned int height, const void* data)
	{
		// Check that the dimensions provided are valid (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG)
		{
			// Emit error if the dimensions are null
			if (width == 0 || height == 0) {
				AEON_LOG_ERROR("Failed to create texture", "The dimensions (" + std::to_string(width) + "x" + std::to_string(height) + ") are invalid.");
				return false;
			}
		}

		// Recreate the texture if necessary
		if (mSize.x != 0) {
			destroy();
			GLCall(glCreateTextures(mBindingTarget, 1, &mHandle));
		}

		// Modify the texture's metadata
		mSize.x = width;
		mSize.y = height;
		mHasMipmap = false;
		if (mFormat.internal == InternalFormat::Native) {
			mFormat = Format(InternalFormat::RGBA8);
		}

		// Deactivate the byte alignment restriction if necessary
		if (mFormat.internal == InternalFormat::R8 || mFormat.internal == InternalFormat::R16) {
			GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		}

		// Create the OpenGL texture with the dimensions width x height and fill it with the image data provided
		GLCall(glTextureStorage2D(mHandle, 1, static_cast<GLenum>(mFormat.internal), width, height));
		if (data) {
			GLCall(glTextureSubImage2D(mHandle, 0, 0, 0, width, height, mFormat.base, GL_UNSIGNED_BYTE, data));
		}

		// Reactivate the byte alignment restriction if necessary
		if (mFormat.internal == InternalFormat::R8 || mFormat.internal == InternalFormat::R16) {
			GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
		}

		return true;
	}

	bool Texture2D::update(unsigned int offsetX, unsigned int offsetY, unsigned int width, unsigned int height, const void* data)
	{
		// Check that the parameters provided are valid (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG)
		{
			// Emit error if the texture has yet to be created
			if (mSize.x == 0 || mSize.y == 0) {
				AEON_LOG_ERROR("Failed to update texture", "The texture has yet to be created.");
				return false;
			}
			// Emit error if the offsets provided are greater than the texture's size
			if (offsetX >= mSize.x || offsetY >= mSize.y) {
				AEON_LOG_ERROR("Failed to update texture", "The offsets (" + std::to_string(offsetX) + ", " + std::to_string(offsetY) + ") are invalid.");
				return false;
			}
			// Emit error if the dimensions are null or greater than the texture's size
			if (width == 0 || height == 0 || width + offsetX > mSize.x || height + offsetY > mSize.y) {
				AEON_LOG_ERROR("Failed to update texture", "The dimensions (" + std::to_string(width) + "x" + std::to_string(height) + ") are invalid.");
				return false;
			}
		}

		// Deactivate the byte alignment restriction if necessary
		if (mFormat.internal == InternalFormat::R8 || mFormat.internal == InternalFormat::R16) {
			GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		}

		// Modify the texture's image data
		GLCall(glTextureSubImage2D(mHandle, 0, offsetX, offsetY, width, height, mFormat.base, GL_UNSIGNED_BYTE, data));

		// Reactivate the byte alignment restriction if necessary
		if (mFormat.internal == InternalFormat::R8 || mFormat.internal == InternalFormat::R16) {
			GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));
		}

		return true;
	}

	bool Texture2D::loadFromFile(const std::string& filename)
	{
		// Store filepath
		mFilepath = filename;

		// Load in the image data
		int width, height, channels;
		stbi_uc* pixels8 = (mFormat.bitCount == 8) ? stbi_load(filename.c_str(), &width, &height, &channels, mFormat.imposedChannels) : nullptr;
		stbi_us* pixels16 = (mFormat.bitCount == 16 || !pixels8) ? stbi_load_16(filename.c_str(), &width, &height, &channels, mFormat.imposedChannels) : nullptr;

		// Log an error message if the image couldn't be loaded in
		if (!pixels8 && !pixels16) {
			AEON_LOG_ERROR("Failed to load texture " + filename + " from file", stbi_failure_reason());
			return false;
		}
		// Log a warning message if the image's dimensions aren't even numbers (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (width % 2 != 0 || height % 2 != 0) {
				AEON_LOG_WARNING("Texture may not display correctly", "The dimensions (" + std::to_string(width) + "x" + std::to_string(height) + ") aren't even numbers");
			}
		}

		// Modify the texture's metadata
		mSize.x = width;
		mSize.y = height;
		mHasMipmap = false;
		if (mFormat.internal == InternalFormat::Native) {
			switch (channels)
			{
			case 4:
				mFormat = Format((pixels8) ? InternalFormat::RGBA8 : InternalFormat::RGBA16);
				break;
			case 1:
				mFormat = Format((pixels8) ? InternalFormat::R8 : InternalFormat::R16);
				break;
			case 3:
				mFormat = Format(InternalFormat::RGB8);
				break;
			case 2:
				mFormat = Format((pixels8) ? InternalFormat::RG8 : InternalFormat::RG16);
			}
		}

		// Create the OpenGL texture with the loaded texture data and release the data
		GLCall(glTextureStorage2D(mHandle, 1, static_cast<GLenum>(mFormat.internal), width, height));
		if (mFormat.bitCount == 8) {
			GLCall(glTextureSubImage2D(mHandle, 0, 0, 0, width, height, mFormat.base, GL_UNSIGNED_BYTE, pixels8));
			stbi_image_free(pixels8);
		}
		else {
			GLCall(glTextureSubImage2D(mHandle, 0, 0, 0, width, height, mFormat.base, GL_UNSIGNED_SHORT, pixels16));
			stbi_image_free(pixels16);
		}

		return true;
	}

	const std::string& Texture2D::getFilepath() const noexcept
	{
		return mFilepath;
	}

	const Vector2u& Texture2D::getSize() const noexcept
	{
		return mSize;
	}

	// Public virtual method(s)
	void Texture2D::setWrap(Wrap wrap)
	{
		if (mWrap != wrap) {
			mWrap = wrap;

			// Apply the wrapping mode chosen to the coordinates U (or S or X) and V (or T or Y)
			GLint GLWrap = static_cast<GLint>(mWrap);
			GLCall(glTextureParameteri(mHandle, GL_TEXTURE_WRAP_S, GLWrap));
			GLCall(glTextureParameteri(mHandle, GL_TEXTURE_WRAP_T, GLWrap));
		}
	}
}