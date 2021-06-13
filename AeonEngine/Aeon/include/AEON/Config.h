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

#ifndef Aeon_Config_H_
#define Aeon_Config_H_

// Define Aeon's debug macro
#ifdef _DEBUG
	#define AEON_DEBUG 1
#else
	#define AEON_DEBUG 0
#endif // _DEBUG

// Remove the console window in Release mode
#ifndef _DEBUG
	#ifndef AEON_INTERNAL_LIB
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif // AEON_INTERNAL_LIB
#endif // _DEBUG

// Define macros to differentiate between static and dynamic linking (import / export macros)
#ifndef AEON_STATIC
	#ifdef _WIN32
		// Windows compilers need specific keywords for importing and exporting
		#define AEON_API_EXPORT __declspec(dllexport)
		#define AEON_API_IMPORT __declspec(dllimport)

		// For Visual C++ compilers, we need to turn off the C4251 warning
		#ifdef _MSC_VER
			#pragma warning(disable: 4251)
		#endif // _MSC_VER

	#else // Linux, FreeBSD
		#if __GNUC__ >= 4
			#define AEON_API_EXPORT __attribute__ ((__visibility__ ("default")))
			#define AEON_API_IMPORT __attribute__ ((__visibility__ ("default")))
		#endif // __GNUC__ >= 4
	#endif // _WIN32

	// Define the import / export macro
	#ifdef AEON_API_EXPORTING
		#define AEON_API AEON_API_EXPORT
	#else
		#define AEON_API AEON_API_IMPORT
	#endif // AEON_API_EXPORTING

#else
	// Static builds don't need import/export macros
	#define AEON_API_EXPORT
	#define AEON_API_IMPORT
	#define AEON_API
#endif // AEON_STATIC

// Cross-platform warning for deprecated functions and classes
#ifdef AEON_NO_DEPRECATED_WARNINGS
	// The user explicitly requested to disable deprecation warnings
	#define AEON_DEPRECATED

#elif defined(_MSC_VER)
	// Microsoft C++ compiler
	// Note: On newer MSVC versions, using deprecated functions cause a compiler error.
	// In order to trigger a warning instead of an error, the compiler flag /sdl- must be specified.
	#define AEON_DEPRECATED __declspec(deprecated)

#elif defined(__GNUC__)
	// g++ and Clang
	#define AEON_DEPRECATED __attribute__ ((deprecated))

#else
	// No other compilers are supported, leaves class or function as-is.
	#pragma message("AEON_DEPRECATED isn't supported by your compiler")
	#define AEON_DEPRECATED
#endif // AEON_NO_DEPRECATED_WARNINGS
#endif // Aeon_Config_H_