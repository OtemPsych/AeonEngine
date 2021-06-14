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

#ifndef Aeon_System_Clipboard_H_
#define Aeon_System_Clipboard_H_

#include <string>

#include <AEON/Config.h>

namespace ae
{
	/*!
	 \brief The namespace containing the functions providing access to the clipboard.
	*/
	namespace Clipboard
	{
		// Function(s)
		/*!
		 \brief Places a string into the system clipboard.

		 \param[in] string A std::string containing the new string

		 \par Example:
		 \code
		 ae::Clipboard::setString("A string to place into the clipboard");
		 \endcode

		 \sa getString()

		 \since v0.6.1
		*/
		AEON_API void setString(const std::string& string);
		/*!
		 \brief Retrieves the string stored in the system clipboard.

		 \return A std::string containing the clipboard's contents

		 \par Example:
		 \code
		 std::string clipboardContent = ae::Clipboard::getString();
		 \endcode

		 \sa setString()

		 \since v0.6.1
		*/
		AEON_API std::string getString();
	}
}
#endif // Aeon_System_Clipboard_H_

/*!
 \namespace ae::Clipboard
 \ingroup system

 The ae::Clipboard namespace allows the API user to retrieve the string
 currently in the system clipboard and to set the string directly from the
 application.

 Usage example:
 \code
 // Retrieve the clipboard string
 std::string clipboardString = ae::Clipboard::getString();

 // Set a new clipboard string
 ae::Clipboard::setString("A new string to place into the system clipboard");
 \endcode

 \author Filippos Gleglakos
 \version v0.6.1
 \date 2021.06.13
 \copyright MIT License
*/