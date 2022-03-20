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

#pragma once

#include <AEON/Graphics/TextureAtlas.h>

namespace ae
{
	// Forward declaration(s)
	struct Glyph;
	class Text;

	/*!
	 \brief The class representing a text font.
	*/
	class AEON_API Font
	{
	private:
		// Private nested class(es)
		/*!
		 \brief The structure representing a font page containing glyphs.
		 \details Each font size extracted represents a page.
		*/
		struct Page
		{
			// Public member(s)
			std::map<uint32_t, Glyph> glyphs; //!< The hashmap of glyphs and their codepoint
			void*                     face;   //!< The pointer to the FreeType face

			// Public constructor(s)
			/*!
			 \brief Constructs the ae::Font::Page by providing the filename of the font.

			 \param[in] filename The string containing the location of the font
			 \param[in] success Whether or not the font was successfully loaded in

			 \since v0.7.0
			*/
			Page(const std::string& filename, bool& success);
			/*!
			 \brief Copy constructor.

			 \since v0.7.0
			*/
			Page(const Page&) = default;
			/*!
			 \brief Move constructor.

			 \since v0.7.0
			*/
			Page(Page&&) noexcept = default;
			/*!
			 \brief Destructor.
			 \details Releases all FreeType resources that were allocated.

			 \since v0.5.0
			*/
			~Page();

			// Public operator(s)
			/*!
			 \brief Assignment operator.

			 \return The caller ae::Font::Page

			 \since v0.7.0
			*/
			Page& operator=(const Page&) = default;
			/*!
			 \brief Move assignment operator.

			 \return The caller ae::Font::Page

			 \since v0.7.0
			*/
			Page& operator=(Page&&) noexcept = default;
		};

	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.

		 \since v0.7.0
		*/
		Font() noexcept;
		/*!
		 \brief Deleted copy constructor.

		 \since v0.5.0
		*/
		Font(const Font&) = delete;
		/*!
		 \brief Move constructor.

		 \since v0.7.0
		*/
		Font(Font&&) noexcept = default;

		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.5.0
		*/
		Font& operator=(const Font&) = delete;
		/*!
		 \brief Move assignment operator.

		 \return The caller ae:::Font

		 \since v0.7.0
		*/
		Font& operator=(Font&&) noexcept = default;

		// Public method(s)
		/*!
		 \brief Prepares to load in the font stored at the location provided.
		 \details Supported file formats: .TTF, .TTC, .CFF, .WOFF, .OTF, .OTC, .PFA, .PFB, .PCF, .FNT, .BDF and .PFR.
		 \note Reinitialises the glyph pages in case a different font was previously chosen.
		 
		 \param[in] filename The string containing the filepath of the font (with its extension)

		 \par Example:
		 \code
		 ae::Font font;
		 font.loadFromFile("Assets/Fonts/arial.ttf");
		 \endcode

		 \sa getGlyph()

		 \since v0.7.0
		*/
		void loadFromFile(const std::string& filename);
		/*!
		 \brief Creates a font-text association.
		 \details This method is considered internal, the API user should have no need of this.

		 \param[in] text The text associated to the ae::Font

		 \sa removeText()

		 \since v0.7.0
		*/
		void addText(Text& text);
		/*!
		 \brief Removes the font-text association previously created.
		 \details This method is considered internal, the API user should have no need of this.

		 \param[in] text The text associated to the ae::Font

		 \sa addText()

		 \since v0.7.0
		*/
		void removeText(const Text& text);
		/*!
		 \brief Retrieves the glyph corresponding to the parameters provided.
		 \details If the glyph requested hasn't been loaded in, it will be created.
		 \note A font must be loaded prior to calling this method.

		 \param[in] codepoint The glyph's unicode to retrieve
		 \param[in] characterSize The font size of the glyph to retrieve

		 \return The corresponding ae::Glyph

		 \par Example:
		 \code
		 ae::Font font;
		 font.loadFromFile("Assets/Fonts/arial.ttf");
		 ...
		 const ae::Glyph& glyphF = font.getGlyph((uint32_t)'F', 48);
		 \endcode

		 \sa loadFromFile()

		 \since v0.7.0
		*/
		[[nodiscard]] const Glyph& getGlyph(uint32_t codepoint, uint32_t characterSize);
	private:
		// Private typedef(s)
		using PageItr = std::map<uint32_t, Page>::iterator;
		using GlyphItr = std::map<uint32_t, Glyph>::iterator;

		// Private method(s)
		/*!
		 \brief Creates a new glyph page with the font size provided.

		 \param[in] characterSize The font size of the new glyph page

		 \return The iterator to the newly-created glyph page

		 \sa loadGlyph(), getGlyph()

		 \since v0.7.0
		*/
		[[nodiscard]] PageItr createPage(uint32_t characterSize);
		/*!
		 \brief Loads in the requested glyph to the appropriate page.
		 \details Creates the ae::Glyph, assigns the metadata extracted, creates the individual texture and loads it into the texture atlas.

		 \param[in] page The ae::Font::Page wherein the glyph will be stored
		 \param[in] codepoint The unicode of the glyph to load in

		 \return The iterator to the newly-created glyph

		 \sa createPage(), getGlyph()

		 \since v0.7.0
		*/
		[[nodiscard]] GlyphItr loadGlyph(PageItr& page, uint32_t codepoint);
		/*!
		 \brief Packs all individual textures and assigns the packed positions to the glyphs' texture rects.

		 \sa loadGlyph()

		 \since v0.7.0
		*/
		void updateAtlasTexture();

	private:
		// Private member(s)
		TextureAtlas             mAtlas;           //!< The texture atlas containing all individual glyph textures
		std::map<uint32_t, Page> mPages;           //!< The hashmap of the glyph pages and their character size
		std::vector<Text*>       mAssociatedTexts; //!< The text objects using this font
		std::string              mFilename;        //!< The filepath of the font
	};
}

/*!
 \class ae::Font
 \ingroup graphics

 The ae::Font class is used to load in text fonts which can then be assigned to
 ae::Text instances. The font types supported are as following:
 \li .TTF
 \li .TTC
 \li .CFF
 \li .WOFF
 \li .OTF
 \li .OTC
 \li .PFA
 \li .PFB
 \li .PCF
 \li .FNT
 \li .BDF
 \li .PFR

 It also internally handles font page creation which are the same glyphs
 (characters) in different sizes, and rendering individual glyph textures to an
 atlas texture which is dynamically created in order to reduce texture-swapping,
 therefore improving performance.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2022.02.22
 \copyright MIT License
*/