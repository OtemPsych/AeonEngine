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

#ifndef Aeon_Graphics_GLCommon_H_
#define Aeon_Graphics_GLCommon_H_

#include <AEON/System/DebugLogger.h>

#ifdef AEON_DEBUG
	#define GLCall(x) x;\
	                  ae::gl::checkError(#x);
#else
	#define GLCall(x) x;
#endif // AEON_DEBUG

namespace ae
{
	/*!
	 \brief The internal namespace containing OpenGL error-checking and validity-checking functions.
	 \note This namespace is considered to be internal but may still be used by the API user.
	*/
	namespace gl
	{
		/*!
		 \brief Checks if an OpenGL error occurred and logs an error message if one did occur.
		 \details It logs the OpenGL error type, the OpenGL statement that produced it as well as where it happened.

		 \param[in] statement The OpenGL statement that may have produced an error

		 \par Example:
		 \code
		 // All OpenGL statements should be wrapped with the GLCall() macro (the macro is ignored in Release mode)
		 GLCall(glCompileShader(vertexShader));
		 \endcode

		 \since v0.4.0
		*/
		void checkError(const char* statement);
	}
}
#endif // Aeon_Graphics_GLCommon_H_

/*!
 \namespace ae::gl
 \ingroup graphics

 The ae::gl namespace contains several (mostly) internal functions concerning
 OpenGL and GLSL, the API user won't have need of this namespace unless they
 decide to write their own OpenGL code.

 This namespace contains enumerations in order to be able to use OpenGL types
 without including the necessary files so the average API user won't have to
 include OpenGL into his project if they don't have any intention of using it.

 \author Filippos Gleglakos
 \version v0.4.0
 \date 2019.07.30
 \copyright MIT License
*/