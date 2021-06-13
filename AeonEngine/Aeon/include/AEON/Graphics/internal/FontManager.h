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

#ifndef Aeon_Graphics_FontManager_H_
#define Aeon_Graphics_FontManager_H_

#include <yvals_core.h>

#include <AEON/Config.h>

namespace ae
{
	/*!
	 \brief The singleton class responsible for the initialization of the FreeType library.
	*/
	class AEON_API FontManager
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Destructor.
		 \details Releases the FreeType library pointer.

		 \since v0.5.0
		*/
		~FontManager();
		/*!
		 \brief Deleted copy constructor.

		 \since v0.5.0
		*/
		FontManager(const FontManager&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.5.0
		*/
		FontManager(FontManager&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.5.0
		*/
		FontManager& operator=(const FontManager&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.5.0
		*/
		FontManager& operator=(FontManager&&) = delete;
	public:
		// Public method(s)
		/*!
		 \brief Retrieves the FreeType library pointer.
		 \details The pointer must be converted to FT_Library in order to be used.

		 \return The pointer to the FreeType library

		 \par Example:
		 \code
		 ae::FontManager& fontManager = ae::FontManager::getInstance();
		 FT_Library ftLibrary = static_cast<FT_Library>(fontManager.getHandle());
		 \endcode

		 \since v0.5.0
		*/
		_NODISCARD void* getHandle() const noexcept;

		// Public static method(s)
		/*!
		 \brief Retrieves the single instance of the ae::FontManager.

		 \return The single instance of the ae::FontManager

		 \par Example:
		 \code
		 ae::FontManager& fontManager = ae::FontManager::getInstance();
		 \endcode

		 \since v0.5.0
		*/
		_NODISCARD static FontManager& getInstance();
	private:
		// Private constructor(s)
		/*!
		 \brief Default constructor.
		 \details Initializes the FreeType library.

		 \since v0.5.0
		*/
		FontManager();

	private:
		// Private member(s)
		void* mLibrary; //!< The FreeType library pointer
	};
}
#endif // Aeon_Graphics_FontManager_H_

/*!
 \class ae::FontManager
 \ingroup graphics

 The ae::FontManager singleton class is used to initialize the FreeType library
 which will be used to create the font faces.

 \author Filippos Gleglakos
 \version v0.5.0
 \date 2020.06.04
 \copyright MIT License
*/