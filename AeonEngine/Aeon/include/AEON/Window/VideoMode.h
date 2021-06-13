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

#ifndef Aeon_Window_VideoMode_H_
#define Aeon_Window_VideoMode_H_

#include <AEON/Math/Vector.h>

namespace ae
{
	// Forward declaration(s)
	class Monitor;

	/*!
	 \brief The class that represents a single video mode of a monitor or of the active window.
	*/
	class AEON_API VideoMode
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::VideoMode by providing an optional pointer to a \a monitor that will retrieve its desktop video mode.
		 \details If no pointer to a monitor is provided, the primary monitor will be automatically retrieved.

		 \param[in] monitor The pointer to the associated ae::Monitor, the primary monitor by default

		 \par Example:
		 \code
		 // Automatically select the primary monitor
		 ae::VideoMode primaryDesktopMode;

		 // Provide a specific monitor
		 const ae::MonitorManager& monitorManager = ae::MonitorManager::getInstance();
		 ae::VideoMode secondMonitorDesktopMode(monitorManager.getMonitor(1));
		 \endcode

		 \since v0.6.0
		*/
		explicit VideoMode(const Monitor* const monitor = nullptr);
		/*!
		 \brief Constructs the ae::VideoMode by providing the resolution, the refresh rate, the bits per channel and the pointer to the associated monitor.
		 \details The \a redBits, \a greenBits, \a blueBits and \a monitor parameters are optional.\n
		 The color bit depth is per channel, so a value of 8 bits per channel will be: (8 bits * 4 channels = 32 bits per pixel).

		 \param[in] resolution A 2-dimensional ae::Vector containing the resolution of the fullscreen window or the size of the windowed window
		 \param[in] refreshRate The window's refresh rate to use, 0 to use the highest available refresh rate
		 \param[in] redBits The bit depth of the red channel, 8 by default
		 \param[in] greenBits The bit depth of the green channel, 8 by default
		 \param[in] blueBits The bit depth of the blue channel, 8 by default
		 \param[in] monitor The pointer to the associated ae::Monitor, the primary monitor by default

		 \par Example:
		 \code
		 ae::VideoMode vmode(ae::Vector2i(1280, 720), 60);
		 \endcode

		 \since v0.3.0
		*/
		VideoMode(const Vector2i& resolution, int refreshRate, int redBits = 8, int greenBits = 8, int blueBits = 8, const Monitor* const monitor = nullptr);
		/*!
		 \brief Constructs the ae::VideoMode by providing the width, the height, the refresh rate, the bits per channel and the pointer to the associated window.
		 \details The \a redBits, \a greenBits, \a blueBits and \a monitor parameters are optional.\n
		 The color bit depth is per channel, so a value of 8 bits per channel will be: (8 bits * 4 channels = 32 bits per pixel).

		 \param[in] width The width of a windowed window or the horizontal resolution of a fullscreen window
		 \param[in] height The height of a windowed window or the vertical resolution of a fullscreen window
		 \param[in] refreshRate The window's refresh rate to use, 0 to use the highest available refresh rate
		 \param[in] redBits The bit depth of the red channel, 8 by default
		 \param[in] greenBits The bit depth of the green channel, 8 by default
		 \param[in] blueBits The bit depth of the blue channel, 8 by default
		 \param[in] monitor The pointer to the associated ae::Monitor, the primary monitor by default

		 \par Example:
		 \code
		 ae::VideoMode vmode(1280, 720, 60);
		 \endcode

		 \since v0.3.0
		*/
		VideoMode(int width, int height, int refreshRate, int redBits = 8, int greenBits = 8, int blueBits = 8, const Monitor* const monitor = nullptr);
		/*!
		 \brief Constructs the ae::VideoMode by providing the \a resolution and an optional pointer to a \a monitor that will retrieve the desktop mode of said \a monitor.
		 \details This approach is the simplest way of creating a native video mode if the API user wishes to use the primary monitor of the application user by not providing a specific \a monitor or
		 to use a specific \a monitor.

		 \param[in] resolution A 2-dimensional ae::Vector containing the resolution of the fullscreen window or the size of the windowed window
		 \param[in] monitor The pointer to the ae::Monitor of which its desktop mode will be retrieved that will provide the remaining parameters, the primary monitor by default

		 \par Example:
		 \code
		 const ae::MonitorManager& monitorManager = ae::MonitorManager::getInstance();
		 ae::VideoMode vmode1(ae::Vector2i(1280, 720), monitorManager.getMonitor(1)); // the desktop mode of the second monitor will be chosen
		 ae::VideoMode vmode2(ae::Vector2i(1280, 720));                               // the desktop mode of the primary monitor will be chosen
		 \endcode

		 \since v0.3.0
		*/
		explicit VideoMode(const Vector2i& resolution, const Monitor* const monitor = nullptr);
		/*!
		 \brief Constructs the ae::VideoMode by providing the \a width, the \a height and an optional pointer to a \a monitor that will retrieve the desktop mode of said \a monitor.
		 \details This approach is the simplest way of creating a native video mode if the API user wishes to use the primary monitor of the application user by not providing a specific \a monitor or
		 to use a specific \a monitor.

		 \param[in] width The width of a windowed window or the horizontal resolution of a fullscreen window
		 \param[in] height The height of a windowed window or the vertical resolution of a fullscreen window
		 \param[in] monitor The pointer to the ae::Monitor of which its desktop mode will be retrieved that will provide the remaining parameters, the primary monitor by default

		 \par Example:
		 \code
		 const ae::MonitorManager& monitorManager = ae::MonitorManager::getInstance();
		 ae::VideoMode vmode1(1280, 720, monitorManager.getMonitor(1)); // the desktop mode of the second monitor will be chosen
		 ae::VideoMode vmode2(1280, 720);                               // the desktop mode of the primary monitor will be chosen
		 \endcode

		 \since v0.3.0
		*/
		VideoMode(int width, int height, const Monitor* const monitor = nullptr);
		/*!
		 \brief Constructs the ae::VideoMode by providing the \a resolution and a \a vmode that will provide the remaining parameters.
		 \details This constructor can be used to create a specific native video mode of a specific monitor.

		 \param[in] resolution A 2-dimensional ae::Vector containing the resolution of the fullscreen window or the size of a windowed window
		 \param[in] vmode The ae::VideoMode that will provide the remaining parameters

		 \par Example:
		 \code
		 // The second best video mode of the second monitor
		 const ae::MonitorManager& monitorManager = ae::MonitorManager::getInstance();
		 const ae::Monitor& secondMonitor = monitorManager.getMonitor(1);
		 const ae::VideoMode& secondBestVideoMode = secondMonitor.getFullscreenModes()[1];
		 ae::VideoMode vmode(ae::Vector2i(1280, 720), secondBestVideoMode);
		 \endcode

		 \since v0.3.0
		*/
		VideoMode(const Vector2i& resolution, const VideoMode& vmode) noexcept;
		/*!
		 \brief Constructs the ae::VideoMode by providing the \a width, the \a height and a \a vmode that will provide the remaining parameters.
		 \details This constructor can be used to create a specific native video mode of a specific monitor.

		 \param[in] width The width of a windowed window or the horizontal resolution of a fullscreen window
		 \param[in] height The height of a windowed window or the vertical resolution of a fullscreen window
		 \param[in] vmode The ae::VideoMode that will provide the remaining parameters

		 \par Example:
		 \code
		 // The second best video mode of the second monitor
		 const ae::MonitorManager& monitorManager = ae::MonitorManager::getInstance();
		 const ae::Monitor& secondMonitor = monitorManager.getMonitor(1);
		 const ae::VideoMode& secondBestVideoMode = secondMonitor.getFullscreenModes()[1];
		 ae::VideoMode vmode(1280, 720, secondBestVideoMode);
		 \endcode

		 \since v0.3.0
		*/
		VideoMode(int width, int height, const VideoMode& vmode) noexcept;
		/*!
		 \brief Copy constructor.

		 \since v0.6.0
		*/
		VideoMode(const VideoMode&) = default;
		/*!
		 \brief Move constructor.

		 \since v0.6.0
		*/
		VideoMode(VideoMode&&) noexcept = default;
	public:
		// Public operator(s)
		/*!
		 \brief Assignment operator.

		 \since v0.6.0
		*/
		VideoMode& operator=(const VideoMode&) = default;
		/*!
		 \brief Move assignment operator.

		 \since v0.6.0
		*/
		VideoMode& operator=(VideoMode&&) noexcept = default;
		/*!
		 \brief Equality operator.
		 \details Checks if the respective data of the caller and of the \a other are equal.\n
		 The pointer to the associated monitor won't be taken into account.

		 \param[in] other The ae::VideoMode that will be compared with the caller

		 \return True if all of the caller's and the \a other's data are respectively equal, false otherwise

		 \par Example:
		 \code
		 const ae::MonitorManager& monitorManager = ae::MonitorManager::getInstance();
		 ae::VideoMode vmode1(1280, 720);
		 ae::VideoMode vmode2(1280, 720, monitorManager.getPrimaryMonitor());
		 if (vmode1 == vmode2) {
			...
		 }
		 \endcode

		 \sa operator!=()

		 \since v0.3.0
		*/
		_NODISCARD bool operator==(const VideoMode& other) const noexcept;
		/*!
		 \brief Inequality operator.
		 \details Checks if the respective data of the caller and of the \a other are inequal.\n
		 The pointer to the associated monitor won't be taken into account.

		 \param[in] other The ae::VideoMode that will be compared with the caller

		 \return True if at least one of the caller's and the \a other's data is inequal, false otherwise

		 \par Example:
		 \code
		 const ae::MonitorManager& monitorManager = ae::MonitorManager::getInstance();
		 ae::VideoMode vmode1(1280, 720);
		 ae::VideoMode vmode2(1280, 720, monitorManager.getMonitor(1));
		 if (vmode1 != vmode2) {
			...
		 }
		 \endcode

		 \sa operator==()

		 \since v0.3.0
		*/
		_NODISCARD bool operator!=(const VideoMode& other) const noexcept;
	public:
		// Public method(s)
		/*!
		 \brief Checks if the ae::VideoMode is a valid video mode for the \a monitor provided or the associated monitor if one isn't provided.
		 \details The validity of video modes only concerns their use in fullscreen windows, otherwise all video modes can be used without restriction.

		 \param[in] monitor The ae::Monitor to check if the ae::VideoMode is a valid native video mode or the associated monitor if one isn't provided, nullptr by default

		 \return True if the ae::VideoMode is valid, false otherwise

		 \par Example:
		 \code
		 const ae::MonitorManager& monitorManager = ae::MonitorManager::getInstance();
		 ae::VideoMode vmode(1280, 720);

		 // Check validity with second monitor
		 if (vmode.isValid(monitorManager.getMonitor(1))) {
			...
		 }

		 // Check validity with associated monitor (the primary monitor in this case)
		 if (vmode.isValid()) {
			...
		 }
		 \endcode

		 \since v0.3.0
		*/
		_NODISCARD bool isValid(const Monitor* monitor = nullptr) const;

		/*!
		 \brief Retrieves the ae::VideoMode's resolution / size.
		 \details This member is considered as the resolution if the associated window is in fullscreen and as the size if it's in windowed mode.

		 \return The 2-dimensional ae::Vector containing the resolution/size of the ae::VideoMode

		 \par Example:
		 \code
		 ae::VideoMode vmode(1280, 720);
		 ...
		 const ae::Vector2i& resolution = vmode.getResolution();
		 \endcode

		 \sa getWidth(), getHeight()

		 \since v0.6.0
		*/
		_NODISCARD inline const Vector2i& getResolution() const noexcept { return mResolution; }
		/*!
		 \brief Retrieves the ae::VideoMode's width / horizontal resolution.
		 \details This member is considered as the horizontal resolution if the associated window is in fullscreen and as the width if it's in windowed mode.

		 \return The width / horizontal resolution of the ae::VideoMode

		 \par Example:
		 \code
		 ae::VideoMode vmode(1280, 720);
		 ...
		 int width = vmode.getWidth();
		 \endcode

		 \sa getHeight(), getResolution()

		 \since v0.6.0
		*/
		_NODISCARD inline int getWidth() const noexcept { return mResolution.x; }
		/*!
		 \brief Retrieves the ae::VideoMode's height / vertical resolution.
		 \details This member is considered as the vertical resolution if the associated window is in fullscreen and as the height if it's in windowed mode.

		 \return The height / vertical resolution of the ae::VideoMode

		 \par Example:
		 \code
		 ae::VideoMode vmode(1280, 720);
		 ...
		 int height = vmode.getHeight();
		 \endcode

		 \sa getWidth(), getResolution()

		 \since v0.6.0
		*/
		_NODISCARD inline int getHeight() const noexcept { return mResolution.y; }
		/*!
		 \brief Retrieves the ae::VideoMode's refresh rate.

		 \return The refresh rate of the ae::VideoMode

		 \par Example:
		 \code
		 ae::VideoMode vmode(1280, 720);
		 ...
		 int refreshRate = vmode.getRefreshRate();
		 \endcode

		 \since v0.6.0
		*/
		_NODISCARD inline int getRefreshRate() const noexcept { return mRefreshRate; }
		/*!
		 \brief Retrieves the ae::VideoMode's bit depth of the red channel.
		 \details The color bit depth is per channel, so a value of 8 bits per channel will be: (8 bits * 4 channels = 32 bits per pixel).

		 \return The bit depth of the red channel

		 \par Example:
		 \code
		 ae::VideoMode vmode(1280, 720, 60, 8, 6, 6);
		 ...
		 int redBits = vmode.getRedBits();
		 \endcode

		 \sa getGreenBits(), getBlueBits()

		 \since v0.6.0
		*/
		_NODISCARD inline int getRedBits() const noexcept { return mRedBits; }
		/*!
		 \brief Retrieves the ae::VideoMode's bit depth of the green channel.
		 \details The color bit depth is per channel, so a value of 8 bits per channel will be: (8 bits * 4 channels = 32 bits per pixel).

		 \return The bit depth of the green channel

		 \par Example:
		 \code
		 ae::VideoMode vmode(1280, 720, 60, 6, 8, 6);
		 ...
		 int greenBits = vmode.getGreenBits();
		 \endcode

		 \sa getRedBits(), getBlueBits()

		 \since v0.6.0
		*/
		_NODISCARD inline int getGreenBits() const noexcept { return mGreenBits; }
		/*!
		 \brief Retrieves the ae::VideoMode's bit depth of the blue channel.
		 \details The color bit depth is per channel, so a value of 8 bits per channel will be: (8 bits * 4 channels = 32 bits per pixel).

		 \return The bit depth of the blue channel

		 \par Example:
		 \code
		 ae::VideoMode vmode(1280, 720, 60, 6, 8, 6);
		 ...
		 int blueBits = vmode.getBlueBits();
		 \endcode

		 \sa getRedBits(), getGreenBits()

		 \since v0.6.0
		*/
		_NODISCARD inline int getBlueBits() const noexcept { return mBlueBits; }
		/*!
		 \brief Retrieves the pointer to the ae::VideoMode's associated monitor.
		 
		 \return The pointer to the ae::VideoMode's associated ae::Monitor

		 \par Example:
		 \code
		 const ae::MonitorManager& monitorManager = ae::MonitorManager::getInstance();

		 // Associated monitor: Primary monitor
		 ae::VideoMode vmode1(1280, 720);

		 // Associated monitor: 2nd monitor
		 ae::VideoMode vmode2(1280, 720, monitorManager.getMonitor(1));
		 \endcode

		 \since v0.6.0
		*/
		_NODISCARD inline const Monitor* const getAssociatedMonitor() const noexcept { return mAssociatedMonitor; }

	private:
		// Private member data
		Vector2i       mResolution;        //!< The video mode's resolution
		int            mRefreshRate;       //!< The video mode's refresh rate
		int            mRedBits;           //!< The bit depth of the red channel
		int            mGreenBits;         //!< The bit depth of the green channel
		int            mBlueBits;          //!< The bit depth of the blue channel
		const Monitor* mAssociatedMonitor; //!< The pointer to the video mode's associated monitor
	};
}
#endif // Aeon_Window_VideoMode_H_

/*!
 \class ae::VideoMode
 \ingroup window

 A video mode is defined by a resolution (in pixels), and a depth for the 4
 color channels (in bits per channel). Video modes are used to setup a window
 (ae::Window) during the creation of said window.

 The primary use of video modes is for fullscreen windows: the user must use
 one of the valid video modes authorized by the operating system (which are
 defined by what the monitor and the graphics card support), otherwise the
 creation of the window may fail.

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2021.01.23
 \copyright MIT License
*/