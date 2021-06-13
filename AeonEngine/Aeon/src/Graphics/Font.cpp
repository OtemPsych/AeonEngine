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

#include <AEON/Graphics/Font.h>

#include <GL/glew.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <AEON/System/DebugLogger.h>
#include <AEON/Window/internal/EventQueue.h>
#include <AEON/Window/Event.h>
#include <AEON/Graphics/internal/FontManager.h>
#include <AEON/Graphics/internal/Glyph.h>
#include <AEON/Graphics/GLResourceFactory.h>

namespace ae
{
	// Font::Page
		// Public constructor(s)
	Font::Page::Page(const std::string& filename, bool& success)
		: glyphs()
		, face(nullptr)
	{
		// Retrieve the pointer to the FreeType library
		FT_Library ftLib = static_cast<FT_Library>(FontManager::getInstance().getHandle());

		// Load in the new font face and check for eventual errors
		FT_Face ftFace;
		FT_Error ftError = FT_New_Face(ftLib, filename.c_str(), 0, &ftFace);
		if (ftError == FT_Err_Unknown_File_Format) {
			AEON_LOG_ERROR("Failed to load font from file", "The font file was read, but its format is unsupported.");
			success = false;
			return;
		}
		else if (ftError) {
			AEON_LOG_ERROR("Failed to load font from file", "The filepath \"" + filename + "\" may be incorrect.\nError code: " + std::to_string(ftError) + '.');
			success = false;
			return;
		}

		// Store the pointer to the font face
		face = static_cast<void*>(ftFace);
		success = true;
	}

	Font::Page::Page(Page&& rvalue) noexcept
		: glyphs(std::move(rvalue.glyphs))
		, face(rvalue.face)
	{
	}

	Font::Page::~Page()
	{
		// Release the FreeType resources allocated
		if (face) {
			FT_Face ftFace = static_cast<FT_Face>(face);
			FT_Error ftError = FT_Done_Face(ftFace);
			if (ftError) {
				AEON_LOG_WARNING("Failed to terminate FreeType face", "Unable to properly release internal resources.\nError code: " + std::to_string(ftError) + '.');
			}
		}
	}

		// Public operator(s)
	Font::Page& Font::Page::operator=(Page&& rvalue) noexcept
	{
		// Copy the rvalue's trivial data and move the rest
		glyphs = std::move(rvalue.glyphs);
		face = rvalue.face;

		return *this;
	}

	// Font
		// Public constructor(s)
	Font::Font() noexcept
		: mPages()
		, mAtlas(Texture2D::InternalFormat::R8)
		, mFilename("")
	{
	}

		// Public method(s)
	void Font::loadFromFile(const std::string& filename)
	{
		// Check if the same filename is being requested
		if (mFilename == filename || filename.empty()) {
			return;
		}

		// Reinitialize the glyph pages and the atlas if necessary
		if (!mFilename.empty()) {
			std::map<unsigned int, Page>().swap(mPages);
			mAtlas = TextureAtlas(Texture2D::InternalFormat::R8);
		}

		// Set the new filename
		mFilename = filename;
	}

	const Glyph& Font::getGlyph(uint32_t codepoint, unsigned int characterSize)
	{
		// Check if the corresponding glyph page exists, create it otherwise
		PageItr pageItr = mPages.find(characterSize);
		if (pageItr == mPages.end()) {
			pageItr = createPage(characterSize);
		}

		// Check if the glyph requested exists, create it otherwise
		const std::map<uint32_t, Glyph>& glyphMap = pageItr->second.glyphs;
		auto glyphItr = glyphMap.find(codepoint);
		if (glyphItr == glyphMap.end()) {
			glyphItr = loadGlyph(pageItr, codepoint);
		}

		return glyphItr->second;
	}

		// Private method(s)
	Font::PageItr Font::createPage(unsigned int characterSize)
	{
		// Create the glyph page
		bool success;
		PageItr pageItr = mPages.try_emplace(characterSize, mFilename, success).first;

		// Delete the glyph page if it wasn't successfully created
		if (!success) {
			if (pageItr != mPages.end()) {
				mPages.erase(pageItr);
			}
			return mPages.end();
		}

		// Set the font size that the user wishes to extract from the FreeType face
		FT_Face ftFace = static_cast<FT_Face>(pageItr->second.face);
		FT_Error ftError = FT_Set_Pixel_Sizes(ftFace, 0, characterSize);
		if (ftError) {
			AEON_LOG_ERROR("Failed to set font size", "The character size '" + std::to_string(characterSize) + "' couldn't be extracted.");
			return mPages.end();
		}

		// Return the created page's iterator
		return pageItr;
	}

	Font::GlyphItr Font::loadGlyph(PageItr& page, uint32_t codepoint)
	{
		// Load in the FreeType glyph
		FT_Face ftFace = static_cast<FT_Face>(page->second.face);
		FT_Error ftError = FT_Load_Char(ftFace, codepoint, FT_LOAD_RENDER);
		if (ftError) {
			AEON_LOG_ERROR("Failed to load glyph", "Unable to load the glyph '" + std::to_string(static_cast<char>(codepoint)) + "'.");
			return page->second.glyphs.end();
		}

		// Create the glyph
			// Create and store the glyph's texture within the texture atlas
		FT_GlyphSlot ftGlyph = ftFace->glyph;
		std::shared_ptr<Texture2D> glyphTexture = nullptr;
		if (ftGlyph->bitmap.buffer) {
			glyphTexture = GLResourceFactory::getInstance().create<Texture2D>("", Texture2D::Filter::Linear, Texture2D::Wrap::ClampToEdge, Texture2D::InternalFormat::R8);
			if (glyphTexture->create(ftGlyph->bitmap.width, ftGlyph->bitmap.rows, ftGlyph->bitmap.buffer)) {
				mAtlas.add(*glyphTexture);
			}
		}

			// Set the glyph's metadata
		GlyphItr glyphItr = page->second.glyphs.emplace(codepoint, Glyph()).first;
		Glyph& glyph = glyphItr->second;
		glyph.textureRect.size.x = ftGlyph->bitmap.width;
		glyph.textureRect.size.y = ftGlyph->bitmap.rows;
		glyph.bearing.x = ftGlyph->bitmap_left;
		glyph.bearing.y = ftGlyph->bitmap_top;
		glyph.individualTexture = (ftGlyph->bitmap.buffer) ? glyphTexture : nullptr;
		glyph.advance = ftGlyph->advance.x;

		// Pack together all individual textures and assign the packed positions to the glyphs
		updateAtlasTexture();

		// Return the loaded glyph's iterator
		return glyphItr;
	}

	void Font::updateAtlasTexture()
	{
		// Pack together all individual textures amassed thus far
		mAtlas.pack();

		// Assign the packed position and texture atlas to all glyphs inside the page
		const Texture2D& atlasTexture = mAtlas.getTexture();
		for (auto& pageItr : mPages)
		{
			for (auto& glyphItr : pageItr.second.glyphs)
			{
				Glyph& glyph = glyphItr.second;
				if (glyph.individualTexture) {
					glyph.textureRect.position = mAtlas.getTextureRect(*glyph.individualTexture).position;
				}
				glyph.texture = &atlasTexture;
			}
		}

		// Enqueue an event indicating that corresponding texts should update their uv coordinates
		auto event = std::make_unique<FontEvent>(this);
		EventQueue::getInstance().enqueueEvent(std::move(event));
	}
}