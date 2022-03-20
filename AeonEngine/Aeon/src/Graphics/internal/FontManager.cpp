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

#include <AEON/Graphics/internal/FontManager.h>

#include <cassert>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <AEON/System/DebugLogger.h>

namespace ae
{
	// Public constructor(s)
	FontManager::~FontManager()
	{
		// Release the FreeType library's resources
		assert(mLibrary);
		FT_Library ftLibrary = static_cast<FT_Library>(mLibrary);
		FT_Error ftError = FT_Done_FreeType(ftLibrary);
		if (ftError) {
			AEON_LOG_ERROR("Failed to release FreeType library", "Error code: " + std::to_string(ftError) + '.');
		}
	}

	// Public static method(s)
	FontManager& FontManager::getInstance()
	{
		static FontManager instance;
		return instance;
	}

	// Private constructor(s)
	FontManager::FontManager()
		: mLibrary(nullptr)
	{
		// Initialize the FreeType library and make sure that no errors occurred
		FT_Library ftLibrary;
		FT_Error ftError = FT_Init_FreeType(&ftLibrary);
		if (ftError) {
			AEON_LOG_ERROR("Failed to initialize the FreeType library", "Error code: " + std::to_string(ftError) + '.');
			return;
		}

		mLibrary = static_cast<void*>(ftLibrary);
	}
}