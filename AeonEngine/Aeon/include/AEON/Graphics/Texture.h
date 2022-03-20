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

#ifndef Aeon_Graphics_Texture_H_
#define Aeon_Graphics_Texture_H_

#include <cstdint>
#include <vector>

#include <AEON/Graphics/internal/GLResource.h>

namespace ae
{
	/*!
	 \brief The abstract base class that represents a texture storing image data.
	*/
	class AEON_API Texture : public GLResource
	{
	public:
		// Public enumeration(s)
		/*!
		 \brief The enumeration of all available filtering options.
		 \details Nearest neighbour filtering is more performant but it's quality is visibly worse than linear filtering.\n
		 Mip level filtering can only be used if the texture possesses a mipmap (smaller versions of the base texture).\n
		 Using a mipmap improves performance but increases memory usage.
		*/
		enum class Filter
		{
			None               = 0x0000, //!< No filtering will be performed
			Nearest            = 0x2600, //!< Nearest texture texel chosen
			Linear             = 0x2601, //!< Linear filtering of base texture
			Nearest_MipNearest = 0x2700, //!< Nearest texture texel chosen of selected mip level - Nearest mip level chosen
			Linear_MipNearest  = 0x2701, //!< Linear filtering of selected mip level - Nearest mip level chosen
			Nearest_MipLinear  = 0x2702, //!< Nearest texture texel chosen of selected mip level - Linear interpolation between mip levels
			Linear_MipLinear   = 0x2703  //!< Linear filtering of selected mip level - Linear interpolation between mip levels
		};
		/*!
		 \brief The enumeration of all available wrapping modes.
		 \details The texture wrap comes into play once the normalized coordinates of a texture are no longer situated in the range [0,1].
		*/
		enum class Wrap
		{
			None                = 0x0000, //!< No wrapping mode
			Repeat              = 0x2901, //!< The texture repeats itself
			ClampToBorder       = 0x812D, //!< The texture's border color is used to fill the space available
			ClampToEdge         = 0x812F, //!< The texture's pixels at the edges are used to fill the space available
			MirroredRepeat      = 0x8370, //!< The texture repeats itself in a mirrored manner
			MirroredClampToEdge = 0x8743  //!< Acts the same as MirroredRepeat if the coordinate is in the ranges [-1,0] or [1,2], like ClampToEdge otherwise
		};
		/*!
		 \brief The enumeration of all available sized internal formats.
		*/
		enum class InternalFormat
		{
			Native         = 0x0000, //!< No imposed format, the format native to the texture will be chosen
			R8             = 0x8229, //!< One-channel of 8 bits
			R16            = 0x822A, //!< One-channel of 16 bits
			RG8            = 0x822B, //!< Two-channel of 8 bits (16 bits total)
			RG16           = 0x822C, //!< Two-channel of 16 bits (32 bits total)
			RGB8           = 0x8051, //!< Three-channel of 8 bits (24 bits total)
			RGBA8          = 0x8058, //!< Four-channel of 8 bits (32 bits total)
			RGBA16         = 0x805B, //!< Four-channel of 16 bits (64 bits total)
			DEPTH32        = 0x8CAC, //!< Depth channel of 32 bits
			DEPTH24        = 0x81A6, //!< Depth channel of 24 bits
			DEPTH16        = 0x81A5, //!< Depth channel of 16 bits
			DEPTH32STENCIL = 0x8CAD, //!< Depth channel of 32 bits and stencil channel of 8 bits
			DEPTH24STENCIL = 0x88F0, //!< Depth channel of 24 bits and stencil channel of 8 bits
			STENCIL        = 0x8D48  //!< Stencil channel of 8 bits
		};

	public:
		// Public constructor(s)
		/*!
		 \brief Deleted copy constructor.

		 \since v0.4.0
		*/
		Texture(const Texture&) = delete;
		/*!
		 \brief Virtual destructor.
		 \details It's needed as this class will be inherited by others.

		 \since v0.4.0
		*/
		virtual ~Texture();
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.4.0
		*/
		Texture& operator=(const Texture&) = delete;
	public:
		// Public method(s)
		/*!
		 \brief Binds the ae::Texture to the context indicating to OpenGL that we're about to use it.
		 \details The unit texture is necessary if multiple textures are to be used to draw a single entity.\n
		 \note The ae::Texture should be unbound at the end of its usage.

		 \param[in] unit The unit texture index with which the ae::Texture will be bound

		 \sa bind(), unbind()

		 \since v0.4.0
		*/
		void bind(int unit) const;
		/*!
		 \brief Automatically generates a mipmap levels by using the texture's current image data.
		 \details A mipmap is a collection of pre-calculated optimized textures.
		 Every mip level in a mipmap is generated by dividing the previous level's dimensions until the final level's size is 1x1.
		 The textures generated can use more advanced filtering methods thus improving the visual quality as well as performance.

		 \param[in] filter The ae::Texture::Filter to apply to the mipmap, ae::Texture::Filter::Linear_MipLinear by default

		 \sa setFilter(), hasMipmap()

		 \since v0.7.0
		*/
		void generateMipmap(Filter filter = Filter::Linear_MipLinear);
		/*!
		 \brief Sets the ae::Texture's filtering type.
		 \note The filter types containing the mipmap levels can only be used if a mipmap has been generated.

		 \param[in] filter The ae::Texture::Filter that will replace the current filter type

		 \sa getFilter(), generateMipmap(), setWrap()

		 \since v0.5.0
		*/
		void setFilter(Filter filter);

		/*!
		 \brief Retrieves the ae::Texture's filtering type performed on the texture.

		 \return The ae::Texture::Filter used to filter the texture's image data

		 \sa setFilter(), getWrap()

		 \since v0.7.0
		*/
		_NODISCARD inline Filter getFilter() const noexcept { return mFilter; }
		/*!
		 \brief Retrieves the ae::Texture's wrapping mode that comes into effect once the normalized coordinates aren't situated in the range [0,1].

		 \return The ae::Texture::Wrap used to wrap the texture in the available space

		 \sa setWrap(), getFilter()

		 \since v0.7.0
		*/
		_NODISCARD inline Wrap getWrap() const noexcept { return mWrap; }
		/*!
		 \brief Retrieves the ae::Texture's internal format of the image data.
		 \details The number of color channels depends on the format imposed at the texture's instantiation or of the loaded texture's native format.

		 \return The ae::Texture::InternalFormat imposed or the loaded texture's native format

		 \since v0.7.0
		*/
		_NODISCARD inline InternalFormat getInternalFormat() const noexcept { return mFormat.internal; }
		/*!
		 \brief Checks whether a mipmap has been generated for the ae::Texture.

		 \return True if a mipmap's been generated, false otherwise

		 \sa generateMipmap()

		 \since v0.7.0
		*/
		_NODISCARD inline bool hasMipmap() const noexcept { return mHasMipmap; }

		// Public virtual method(s)
		/*!
		 \brief Sets the ae::Texture's wrapping mode.
		 \details The wrapping mode comes into play once the normalized coordinates aren't situated in the range [0,1].

		 \param[in] wrap The ae::Texture::Wrap that will replace the current wrapping mode

		 \sa getWrap(), setFilter()

		 \since v0.4.0
		*/
		virtual void setWrap(Wrap wrap);
		/*!
		 \brief Deletes the OpenGL identifier that was created.

		 \since v0.4.0
		*/
		virtual void destroy() const override final;
		/*!
		 \brief Binds the ae::Texture to the context indicating to OpenGL that we're about to use it.
		 \details This method will assume that the first unit texture is desired so multiple textures can't be bound at the same time.
		 \note The ae::Texture should be unbound at the end of its usage.

		 \sa bind(int), unbind()

		 \since v0.4.0
		*/
		virtual void bind() const override final;
		/*!
		 \brief Unbinds the ae::Texture from the context indicating to OpenGL that we've finished using it.
		 \details This method will reset the active texture to the first unit texture.
		 \note Make sure that the currently-bound texture is the caller ae::Texture as this method will unbind any texture.

		 \sa bind()

		 \since v0.4.0
		*/
		virtual void unbind() const override final;
	protected:
		// Protected constructor(s)
		/*!
		 \brief Constructs the ae::Texture by providing a binding \a target, the filter type, the wrapping mode and the internal format of the data.
		 \details Available binding targets:
		 \li GL_TEXTURE_1D
		 \li GL_TEXTURE_2D
		 \li GL_TEXTURE_3D
		 \li GL_TEXTURE_1D_ARRAY
		 \li GL_TEXTURE_2D_ARRAY
		 \li GL_TEXTURE_RECTANGLE
		 \li GL_TEXTURE_CUBE_MAP
		 \li GL_TEXTURE_CUBE_MAP_ARRAY
		 \li GL_TEXTURE_BUFFER
		 \li GL_TEXTURE_2D_MULTISAMPLE
		 \li GL_TEXTURE_2D_MULTISAMPLE_ARRAY

		 \param[in] target The binding target to which the ae::Texture can be bound
		 \param[in] filter The ae::Texture::Filter that'll be applied to the texture
		 \param[in] wrap The ae::Texture::Wrap mode that will be employed once the normalized coordinates aren't in the range [0,1]
		 \param[in] internalFormat The ae::Texture::InternalFormat of the image data

		 \since v0.7.0
		*/
		Texture(uint32_t target, Filter filter, Wrap wrap, InternalFormat internalFormat);
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Texture that will be moved

		 \since v0.7.0
		*/
		Texture(Texture&& rvalue) noexcept = default;
	protected:
		// Protected operator(s)
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Texture that will be moved

		 \return The caller ae::Texture

		 \since v0.7.0
		*/
		Texture& operator=(Texture&& rvalue) noexcept = default;

	protected:
		// Protected struct(s)
		/*!
		 \brief Struct describing the texture's image data format.
		*/
		struct Format
		{
			// Public member(s)
			InternalFormat internal;        //!< The sized internal format
			uint32_t       base;            //!< The OpenGL base format
			int            imposedChannels; //!< The number of imposed channels
			int            bitCount;        //!< The number of bits per channel

			// Public constructor(s)
			/*!
			 \brief Constructs the ae::Texture::Format by providing the sized internal format.
			 \details The OpenGL base format, number of channels and the bit count will be calculated based on the internal format provided.

			 \param[in] internalFormat The ae::Texture::InternalFormat of the image data

			 \since v0.7.0
			*/
			explicit Format(InternalFormat internalFormat) noexcept;
			/*!
			 \brief Deleted copy constructor.

			 \since v0.6.0
			*/
			Format(const Format&) = delete;
			/*!
			 \brief Move constructor.

			 \param[in] rvalue The ae::Texture::Format that will be moved

			 \since v0.7.0
			*/
			Format(Format&& rvalue) noexcept = default;

			// Public operator(s)
			/*!
			 \brief Deleted assignment operator.

			 \since v0.6.0
			*/
			Format& operator=(const Format&) = delete;
			/*!
			 \brief Move assignment operator.

			 \param[in] rvalue The ae::Texture::Format that will be moved

			 \return The caller ae::Texture::Format

			 \since v0.7.0
			*/
			Format& operator=(Format&& rvalue) noexcept = default;
		};
	protected:
		// Protected member(s)
		Format   mFormat;        //!< The information regarding the image data's format
		Wrap     mWrap;          //!< The wrapping mode to employ once the normalized coordinates aren't in the range [0,1]
		uint32_t mBindingTarget; //!< The binding target of the OpenGL texture
		bool     mHasMipmap;     //!< Whether a mipmap has been generated
	private:
		// Private member(s)
		Filter   mFilter;        //!< The filtering type to apply
	};
}
#endif // Aeon_Graphics_Texture_H_

/*!
 \class ae::Texture
 \ingroup graphics

 The ae::Texture abstract base class is used to represent a texture which is,
 essentially, a collection of pixels. The texture's data are located in the
 VRAM so this class is simply an intermediary to that data.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.07.26
 \copyright MIT License
*/