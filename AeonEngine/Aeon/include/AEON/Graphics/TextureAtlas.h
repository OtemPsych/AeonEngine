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

#ifndef Aeon_Graphics_TextureAtlas_H_
#define Aeon_Graphics_TextureAtlas_H_

#include <map>

#include <AEON/Config.h>
#include <AEON/Math/AABoxCollider.h>
#include <AEON/Graphics/Texture2D.h>

namespace ae
{
	/*!
	 \brief The class representing a dynamic texture atlas.
	 \note Should only be used to create a texture atlas during runtime.
	*/
	class _NODISCARD AEON_API TextureAtlas
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::TextureAtlas by providing an optional internal format for the buffer.
		 \details Available buffer formats:
		 \li Native (RGBA8 will be chosen)
		 \li R8
		 \li R16
		 \li RG8
		 \li RG16
		 \li RGB8
		 \li RGBA8
		 \li RGBA16

		 \note The buffer formats won't be checked, unexpected results may occur if the wrong format is chosen.

		 \param[in] format The ae::Texture2D::InternalFormat of the buffer's image data, ae::Texture2D::InternalFormat::RGBA8 by default

		 \since v0.5.0
		*/
		explicit TextureAtlas(Texture2D::InternalFormat format = Texture2D::InternalFormat::RGBA8);
		/*!
		 \brief Deleted copy constructor.

		 \since v0.5.0
		*/
		TextureAtlas(const TextureAtlas&) = default;
		/*!
		 \brief Move constructor.

		 \since v0.5.0
		*/
		TextureAtlas(TextureAtlas&&) noexcept = default;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.5.0
		*/
		TextureAtlas& operator=(const TextureAtlas&) = delete;
		/*!
		 \brief Move assignment operator.

		 \since v0.5.0
		*/
		TextureAtlas& operator=(TextureAtlas&&) noexcept = default;
	public:
		// Public method(s)
		/*!
		 \brief Stores a pointer to an ae::Texture2D instance to be packed into the texture atlas.

		 \param[in] texture The pointer to the texture to be added

		 \par Example:
		 \code
		 // Create the individual textures via the resource factory
		 ae::GLResourceFactory& factory = ae::GLResourceFactory::getInstance();
		 auto texture1 = factory.create<ae::Texture2D>("");
		 ...

		 // Create the texture atlas and add in the textures (only update the atlas once all textures have been added)
		 ae::TextureAtlas atlas;
		 atlas.addTexture(texture1.get());
		 atlas.addTexture(texture2.get());
		 atlas.addTexture(texture3.get());

		 // Pack the textures inserted into the texture atlas (compute packed texture positions and render them into texture atlas)
		 atlas.packTextures();

		 // Retrieve the texture containing all individual textures and their positions within it
		 const ae::Texture2D* const finalTexture = atlas.getTexture();

		 // Retrieve the computed texture rectangle within the texture atlas of each texture added
		 ae::Box2i texture2Rect = atlas.getTextureRect(texture2.get());
		 \endcode

		 \sa packTextures(), getTexture(), getTextureRect()

		 \since v0.5.0
		*/
		void addTexture(const Texture2D* const texture);
		/*!
		 \brief Packs together the ae::Texture2D instances that have been added thus far into the texture atlas.

		 \par Example:
		 \code
		 // Create the texture atlas and add in the textures
		 ae::TextureAtlas atlas;
		 atlas.addTexture(texture1);
		 atlas.addTexture(texture2);
		 atlas.addTexture(texture3);

		 // Pack the textures inserted into the texture atlas (compute packed texture positions and render them into texture atlas)
		 atlas.packTextures();

		 // Retrieve the texture containing all individual textures and their positions within it
		 const ae::Texture2D* const finalTexture = atlas.getTexture();

		 // Retrieve the computed texture rectangle within the texture atlas of each texture added
		 ae::Box2i texture2Rect = atlas.getTextureRect(texture2);
		 \endcode

		 \sa addTexture(), getTexture(), getTextureRect()

		 \since v0.5.0
		*/
		void packTextures();
		/*!
		 \brief Retrieves the texture atlas containing all packed textures that have been added thus far.
		 \note The textures to be packed have to be added prior to calling this method.

		 \return The final texture atlas containing all the packed textures

		 \par Example:
		 \code
		 // Create the texture atlas and add in the textures (only update the atlas once all textures have been added)
		 ae::TextureAtlas atlas;
		 atlas.addTexture(texture1, false); // texture added, no computation was made
		 atlas.addTexture(texture2, false); // texture added, no computation was made
		 atlas.addTexture(texture3);        // texture added, computed packed texture positions and rendered into texture atlas

		 // Retrieve the texture containing all individual textures
		 const ae::Texture2D* const finalTexture = atlas.getTexture();
		 \endcode

		 \sa addTexture(), getTextureRects()

		 \since v0.5.0
		*/
		_NODISCARD const Texture2D* const getTexture() const noexcept;
		/*!
		 \brief Retrieves the texture rect computed for the packed texture provided.
		 \note The texture requested has to be added prior to calling this method.

		 \param[in] texture The pointer to the texture added

		 \return The texture rect that was computed for the packed texture requested

		 \par Example:
		 \code
		 // Create the texture atlas and add in the textures
		 ae::TextureAtlas atlas;
		 atlas.addTexture(texture1);
		 atlas.addTexture(texture2);
		 atlas.addTexture(texture3);

		 // Pack the textures inserted into the texture atlas (compute packed texture positions and render them into texture atlas)
		 atlas.packTextures();

		 // Retrieve the computed texture rectangle within the texture atlas of each texture added
		 ae::Box2i texture2Rect = atlas.getTextureRect(texture2);
		 \endcode

		 \sa addTexture(), packTextures(), getTexture()

		 \since v0.5.0
		*/
		_NODISCARD Box2i getTextureRect(const Texture2D* const texture) const noexcept;
	private:
		// Private method(s)
		/*!
		 \brief Calculates the optimal positions for the textures added within the texture atlas.

		 \return The dimensions of the texture atlas required to fit all the individual textures

		 \sa renderTextures(), packTextures()

		 \since v0.5.0
		*/
		Vector2i computePacking();

	private:
		// Private member(s)
		std::shared_ptr<Texture2D>        mAtlas;    //!< The texture that will serve as the texture atlas
		std::map<const Texture2D*, Box2i> mTextures; //!< The textures to be packed and associated rectangles to be computed
	};
}
#endif //Aeon_Graphics_TextureAtlas_H_

/*!
 \class ae::TextureAtlas
 \ingroup graphics

 The ae::TextureAtlas is used to create a dynamic texture atlas composed of
 several other textures. This class shouldn't be used to load in a premade
 texture atlas using a third-party software (the API user should opt for a
 normal texture in that case) as the functionalities that it provides are
 purely suited for runtime creation of a texture atlas.

 \author Filippos Gleglakos
 \version v0.5.0
 \date 2020.08.01
 \copyright MIT License
*/