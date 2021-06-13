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

#ifndef Aeon_Graphics_Texture2D_H_
#define Aeon_Graphics_Texture2D_H_

#include <AEON/Math/Vector.h>
#include <AEON/Graphics/Texture.h>

namespace ae
{
	/*!
	 \brief The class that represents a 2-dimensional texture used for 2D images.
	*/
	class _NODISCARD AEON_API Texture2D : public Texture
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::Texture2D by providing the optional filter type, the wrapping mode and internal format of the data.
		 \details The filter type and the wrapping mode can be set later, but the internal format can only be set during construction.

		 \param[in] filter The ae::Texture::Filter that'll be applied to the texture, ae::Texture2D::Filter::Linear by default
		 \param[in] wrap The ae::Texture::Wrap mode that will be employed once the normalized coordinates aren't in the range [0,1], ae::Texture2D::Wrap::ClampToEdge by default
		 \param[in] internalFormat The ae::Texture::InternalFormat of the image data, ae::Texture2D::InternalFormat::Native by default

		 \par Example:
		 \code
		 ae::Texture2D texture(ae::Texture2D::Filter::Nearest);
		 \endcode

		 \since v0.4.0
		*/
		explicit Texture2D(Filter filter = Filter::Linear, Wrap wrap = Wrap::ClampToEdge, InternalFormat internalFormat = InternalFormat::Native);
		/*!
		 \brief Deleted copy constructor.

		 \since v0.4.0
		*/
		Texture2D(const Texture2D&) = delete;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Texture2D that will be moved

		 \since v0.6.0
		*/
		Texture2D(Texture2D&& rvalue) noexcept;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.4.0
		*/
		Texture2D& operator=(const Texture2D&) = delete;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Texture2D that will be moved

		 \return The caller ae::Texture2D

		 \since v0.6.0
		*/
		Texture2D& operator=(Texture2D&& rvalue) noexcept;
	public:
		// Public method(s)
		/*!
		 \brief (Re)Creates a texture with the dimensions \a width x \a height.
		 \details The dimensions provided should optimally be even numbers for correct results.

		 \param[in] width The texture's width
		 \param[in] height The texture's height
		 \param[in] data The pixel data that will be used to fill the texture, nullptr by default

		 \return True if the texture was created successfully, false otherwise

		 \par Example:
		 \code
		 ae::Texture2D texture(ae::Texture2D::Filter::Nearest);
		 if (!texture.create(640, 480)) {
			std::cerr << "Error.." << std::endl;
		 }
		 \endcode

		 \sa loadFromFile(), update()

		 \since v0.6.0
		*/
		bool create(unsigned int width, unsigned int height, const void* data = nullptr);
		/*!
		 \brief Updates the ae::Texture's image data.
		 \details This method is used to modify the texture's current data without recreating it.

		 \param[in] offsetX The horizontal texel offset to place the new data
		 \param[in] offsetY The vertical texel offset to place the new data
		 \param[in] width The width of the subimage
		 \param[in] height The height of the subimage
		 \param[in] data The pixel data that will be placed within the constraints provided

		 \return True if the texture was updated successfully, false otherwise

		 \par Example:
		 \code
		 ae::Texture2D texture(ae::Texture2D::Filter::Nearest);
		 ...
		 const void* newData = ...;
		 if (!texture.update(0, 0, 240, 480, newData)) {
			std::cerr << "Error.." << std::endl;
		 }
		 \endcode
		 
		 \sa create()

		 \since v0.6.0
		*/
		bool update(unsigned int offsetX, unsigned int offsetY, unsigned int width, unsigned int height, const void* data);
		/*!
		 \brief Loads in a texture from a file on disk.
		 \details The image types supported:
		 \li .JPEG
		 \li .PNG
		 \li .TGA
		 \li .BMP
		 \li .PSD
		 \li .GIF
		 \li .HDR
		 \li .PIC
		 \li .PNM

		 \param[in] filename The string containing the filepath with the extension
		 
		 \return True is the texture was created successfully, false otherwise

		 \par Example:
		 \code
		 ae::Texture2D texture;
		 if (!texture.loadFromFile("Textures/texture.png")) {
			std::cerr << "Error.." << std::endl;
		 }
		 \endcode

		 \sa create()

		 \since v0.4.0
		*/
		bool loadFromFile(const std::string& filename);
		/*!
		 \brief Retrieves the ae::Texture2D's loaded image's filepath.

		 \return A std::string containing the texture's filepath

		 \par Example:
		 \code
		 ae::Texture2D texture;
		 texture.loadFromFile("Textures/texture.png");
		 ...
		 const std::string& filepath = texture.getFilepath();
		 \endcode

		 \since v0.6.0
		*/
		_NODISCARD const std::string& getFilepath() const noexcept;
		/*!
		 \brief Retrieves the ae::Texture2D's loaded image's size.
		 
		 \return An ae::Vector2u containing the texture's dimensions
		 
		 \par Example:
		 \code
		 ae::Texture2D texture;
		 texture.loadFromFile("Textures/texture.png");

		 const ae::Vector2u& textureSize = texture.getSize();
		 \endcode

		 \since v0.4.0
		*/
		_NODISCARD const Vector2u& getSize() const noexcept;

		// Public virtual method(s)
		/*!
		 \brief Sets the ae::Texture2D's wrapping mode.
		 \details The wrapping mode comes into play once the normalized coordinates aren't situated in the range [0,1].

		 \param[in] wrap The ae::Texture2D::Wrap that will replace the current wrapping mode

		 \par Example:
		 \code
		 ae::Texture2D texture(ae::Texture2D::Filter::Nearest);
		 ...
		 texture.setWrap(ae::Texture2D::Wrap::Repeat);
		 \endcode

		 \sa getWrap(), setFilter()

		 \since v0.4.0
		*/
		virtual void setWrap(Wrap wrap) override final;

	private:
		// Private member(s)
		std::string mFilepath; //!< The texture's filepath
		Vector2u    mSize;     //!< The texture's size
	};
}
#endif // Aeon_Graphics_Texture2D_H_

/*!
 \class ae::Texture2D
 \ingroup graphics

 The ae::Texture2D class is used to represent a 2-dimensional texture (the most
 common image) which is, essentially, a collection of pixels. The image data is
 located in the VRAM so this class is simply an intermediary to that data.

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2020.09.05
 \copyright MIT License
*/