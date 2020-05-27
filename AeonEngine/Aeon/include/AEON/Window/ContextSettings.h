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

#ifndef Aeon_Window_ContextSettings_H_
#define Aeon_Window_ContextSettings_H_

#include <yvals_core.h>

#include <AEON/Config.h>

namespace ae
{
	/*!
	 \brief The class that represents the OpenGL context settings of a window.
	*/
	class _NODISCARD AEON_API ContextSettings
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::ContextSettings by providing several optional parameters.
		 \details The apply() method must be called in order for the parameters provided to take effect at the creation of the next window.
		 \note The OpenGL version must be equal or greater than 4.5.

		 \param[in] msaa The number of multisampling samples to use for the anti-aliasing (x8, x16, x24), 0 by default
		 \param[in] major The major number of the OpenGL version with which the context has to be compatible, 4 by default
		 \param[in] minor The minor number of the OpenGL version with which the context has to be compatible, 5 by default
		 \param[in] depth The number of bits for the depth buffer, 24 by default
		 \param[in] stencil The number of bits for the stencil buffer, 8 by default
		 \param[in] sRgb If the framebuffer should be sRGB-compatible, false by default

		 \par Example:
		 \code
		 // Anti-aliasing level: x4, OpenGL version: 4.6
		 ae::ContextSettings settings(4, 4, 6);
		 \endcode

		 \since v0.3.0
		*/
		explicit ContextSettings(int msaa = 0, int major = 4, int minor = 5, int depth = 24, int stencil = 8, bool sRgb = false);
	public:
		// Public method(s)
		/*!
		 \brief Applies the window hints of the ae::ContextSettings that will take effect at the creation of the next window.
		 \details This method is called automatically once an ae::ContextSettings object is passed to the ae::Window constructor.

		 \par Example:
		 \code
		 // Anti-aliasing level: x4, OpenGL version: 4.5, Depth buffer bits: 32
		 ae::ContextSettings settings(4, 4, 5, 32);
		 settings.apply();
		 \endcode

		 \since v0.3.0
		*/
		void apply() const;
		/*!
		 \brief Sets the anti-aliasing level to use (the number of multisampling samples).
		 \note The anti-aliasing level must be equal to the result of 2 to the power of n (x2, x4, x8, x16, x32).

		 \param[in] msaa The anti-aliasing level to use that must be equal to the result of 2 to the power of n or 0 to disable anti-aliasing, 0 by default

		 \par Example:
		 \code
		 // Anti-aliasing level: x4
		 ae::ContextSettings settings(4);
		 ...
		 settings.setAntialiasingLevel(0); // disable anti-aliasing
		 settings.setAntialiasingLevel(2); // x2
		 settings.setAntialiasingLevel(16); // x16
		 settings.setAntialiasingLevel(15); // error: no effect
		 \endcode

		 \sa getAntialiasingLevel()

		 \since v0.3.0
		 */
		void setAntialiasingLevel(int msaa);
		/*!
		 \brief Retrieves the anti-aliasing level that is to be used.
		 \details The anti-aliasing level is the number of multisampling samples to use.

		 \return The anti-aliasing level that is to be used by the next window or the current window

		 \par Example:
		 \code
		 // Anti-aliasing level: x4
		 ae::ContextSettings settings(4);
		 ...
		 int antialiasing = settings.getAntialiasingLevel();
		 \endcode

		 \sa setAntialiasingLevel()

		 \since v0.3.0
		*/
		_NODISCARD int getAntialiasingLevel() const noexcept;
		/*!
		 \brief Sets the OpenGL version with which the OpenGL context must be compatible.
		 \note The OpenGL version must be equal or greater than 4.5.

		 \param[in] major The major number of the OpenGL version with which the context has to be compatible, 4 by default
		 \param[in] minor The minor number of the OpenGL version with which the context has to be compatible, 5 by default

		 \par Example:
		 \code
		 // OpenGL version: 4.5
		 ae::ContextSettings settings;
		 ...
		 settings.setContextVersion(4, 6);
		 \endcode

		 \sa getContextVersion()

		 \since v0.3.0
		*/
		void setContextVersion(int major, int minor);
		/*!
		 \brief Modifies the references' values by assigning to them the context's major and minor numbers.

		 \param[in,out] major A reference to the value that'll be assigned to the context's major number
		 \param[in,out] minor A reference to the value that'll be assigned to the context's minor number

		 \par Example:
		 \code
		 // OpenGL version: 4.5
		 ae::ContextSettings settings;
		 ...
		 int major, minor;
		 settings.getContextVersion(major, minor);
		 \endcode

		 \sa setContextVersion()

		 \since v0.3.0
		*/
		void getContextVersion(int& major, int& minor) const noexcept;
		/*!
		 \brief Sets the number of bits to use for the depth buffer.
		 \details The standard number of bits for the depth buffer is 24-bit or 32-bit for a high-precision depth buffer.
		 \note The \a depth bits provided must be a multiple of 8 and equal or greater to 16.

		 \param[in] depth The number of bits for the depth buffer, 24 by default

		 \par Example:
		 \code
		 // Depth buffer bits: 24
		 ae::ContextSettings settings;
		 ...
		 settings.setDepthBits(32);
		 \endcode

		 \sa getDepthBits()

		 \since v0.3.0
		*/
		void setDepthBits(int depth);
		/*!
		 \brief Retrieves the number of bits used for the depth buffer.

		 \return The number of bits for the depth buffer

		 \par Example:
		 \code
		 // Depth buffer bits: 24
		 ae::ContextSettings settings;
		 ...
		 int depthBits = settings.getDepthBits();
		 \endcode

		 \sa setDepthBits()

		 \since v0.3.0
		*/
		_NODISCARD int getDepthBits() const noexcept;
		/*!
		 \brief Sets the number of bits to use for the stencil buffer.
		 \details The usual number of bits for the stencil buffer is 8-bit.
		 \note The \a stencil bits provided must be greater or equal to 0.

		 \param[in] stencil The number of bits for the stencil buffer, 8 by default

		 \par Example:
		 \code
		 // Stencil buffer bits: 8
		 ae::ContextSettings settings;
		 ...
		 settings.setStencilBits(16);
		 \endcode

		 \sa getStencilBits()

		 \since v0.3.0
		*/
		void setStencilBits(int stencil);
		/*!
		 \brief Retrieves the number of bits used for the stencil buffer.

		 \return The number of bits for the stencil buffer

		 \par Example:
		 \code
		 // Stencil buffer bits: 8
		 ae::ContextSetings settings;
		 ...
		 int stencilBits = settings.getStencilBits();
		 \endcode

		 \sa setStencilBits()

		 \since v0.3.0
		*/
		_NODISCARD int getStencilBits() const noexcept;
		/*!
		 \brief Enables or disables sRGB compatibility for the framebuffer.

		 \param[in] flag Whether sRGB rendering will enabled or not, disabled by default

		 \par Example:
		 \code
		 // sRGB-capable: false
		 ae::ContextSettings settings;
		 ...
		 settings.setSrgbEnabled(true);
		 \endcode

		 \sa isSrgbEnabled()

		 \since v0.3.0
		*/
		void setSrgbEnabled(bool flag) noexcept;
		/*!
		 \brief Checks whether sRGB compatibility for the framebuffer is enabled.

		 \return True if sRGB rendering is enabled, false if it's disabled

		 \par Example:
		 \code
		 // sRGB-capable: false
		 ae::ContextSettings settings;
		 ...
		 bool sRGBEnabled = settings.isSrgbEnabled();
		 \endcode

		 \sa setSrgbEnabled()

		 \since v0.3.0
		*/
		_NODISCARD bool isSrgbEnabled() const noexcept;

	private:
		// Private member data
		int  mAntialiasingLevel; //!< The anti-aliasing samples to use
		int  mMajorVersion;      //!< The major number of the context version
		int  mMinorVersion;      //!< The minor version of the context version
		int  mDepthBits;         //!< The number of bits of the depth buffer
		int  mStencilBits;       //!< The number of bits of the stencil buffer
		bool mSrgbCapable;       //!< If the the framebuffer is sRGB-compatible
	};
}
#endif // Aeon_Window_ContextSettings_H_

/*!
 \class ae::ContextSettings
 \ingroup window

 The ae::ContextSettings class permits the definition of several advanced
 options concerning the OpenGL context attached to the active window. It's
 primarily used at the creation of a window; after said creation, the API user
 will rarely have need of it again nor have the need to create another.

 The parameters major number and minor number of the version of the OpenGL
 context are two components of a version separated by a dot.
 If, for example, one wishes to use the version 4.5 of OpenGL, the major
 version would be 4 and the minor version would be 5.

 If the API user decides to use the version 4.5, the installed drivers must
 support version 4.5 of OpenGL in order for the application to work.

 Usage example:
 \code
 // Resolution: 1280x720, the remaining options will be set to those of the desktop mode of the primary monitor
 ae::VideoMode vmode(1280, 720);
 // Anti-aliasing samples: x16, OpenGL version: 4.5
 ae::ContextSettings contextSettings(16, 4, 5);

 ae::Application& app = ae::Application::getInstance();
 app.createWindow(vmode, "My Application", ae::Window::Style::Default, contextSettings);
 \endcode

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.29
 \copyright MIT License
*/