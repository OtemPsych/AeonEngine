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

#ifndef Aeon_Graphics_Glyph_H_
#define Aeon_Graphics_Glyph_H_

#include <yvals_core.h>

#include <AEON/Config.h>
#include <AEON/Math/AABoxCollider.h>
#include <AEON/Math/Vector2.h>

namespace ae
{
	// Forward declaration(s)
	class Texture2D;

	/*!
	 \brief The structure describing a glyph (a text character).
	*/
	struct _NODISCARD AEON_API Glyph
	{
		// Public member(s)
		Box2i                      textureRect;       //!< The position and size of the glyph within the texture
		Vector2i                   bearing;           //!< The glyph's offset in pixels based on its origin
		const Texture2D*           texture;           //!< The texture atlas containing the glyph
		std::shared_ptr<Texture2D> individualTexture; //!< The individual texture containing the glyph
		unsigned int               advance;           //!< The horizontal offset in 1/64 pixels to the next glyph's origin

		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 
		 \since v0.5.0
		*/
		Glyph() noexcept;
		/*!
		 \brief Copy constructor.

		 \since v0.5.0
		*/
		Glyph(const Glyph&) = default;
		/*!
		 \brief Move constructor.

		 \since v0.5.0
		*/
		Glyph(Glyph&&) noexcept = default;

		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.5.0
		*/
		Glyph& operator=(const Glyph&) = delete;
		/*!
		 \brief Move assignment operator.

		 \since v0.5.0
		*/
		Glyph& operator=(Glyph&&) noexcept = default;
	};
}
#endif // Aeon_Graphics_Glyph_H_

/*!
 \struct ae::Glyph
 \ingroup graphics

 A glyph is the visual representation of a text character.

 The ae::Glyph structure possesses the following information in order to handle
 a glyph:
 \li The pointer to the individual texture that contains only the bitmap of the glyph
 \li The pointer to the texture atlas that contains part of the bitmap of the glyph
 \li The texture rect indicating its position within the texture and the its size
 \li The glyph's offset in pixels based on its origin
 \li The horizontal offset in 1/64 pixels by the glyph's origin until the origin of the next glyph

 \author Filippos Gleglakos
 \version v0.5.0
 \date 2020.08.02
 \copyright MIT License
*/