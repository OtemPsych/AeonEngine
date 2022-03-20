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

#ifndef Aeon_Window_Monitor_H_
#define Aeon_Window_Monitor_H_

#include <vector>
#include <memory>
#include <string>

#include <AEON/Math/Vector.h>
#include <AEON/Math/AABoxCollider.h>
#include <AEON/Window/VideoMode.h>

// Forward declaration(s)
struct GLFWmonitor;

namespace ae
{
	/*!
	 \brief The class that represents a monitor in the application user's setup.
	 \note The API user doesn't have to construct a monitor object.
	*/
	class _NODISCARD AEON_API Monitor
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::Monitor by providing the pointer to the GLFW handle of the monitor.
		 \note This constructor doesn't have to be used by the API user.

		 \param[in] handle The pointer to the GLFW handle of the monitor

		 \since v0.3.0
		*/
		explicit Monitor(GLFWmonitor* handle);
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		Monitor(const Monitor&) = delete;
		/*!
		 \brief Move constructor.
		 \details Performs a move operation on the \a rvalue's member data to the lvalue ae::Monitor's member data.

		 \param[in] rvalue The ae::Monitor rvalue that will be moved

		 \since v0.3.0
		*/
		Monitor(Monitor&& rvalue);
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.3.0
		*/
		Monitor& operator=(const Monitor&) = delete;
		/*!
		 \brief Move assignment operator.
		 \details Performs a move assignment from the \a rvalue's member data to the caller ae::Monitor's ones.

		 \param[in] rvalue The ae::Monitor rvalue that will be moved

		 \return The caller ae::Monitor containing the moved member data

		 \since v0.3.0
		*/
		Monitor& operator=(Monitor&& rvalue);
	public:
		// Public method(s)
		/*!
		 \brief Updates the ae::Monitor's properties that are dependant on the application user's monitor setup.
		 \details This method is called automatically once a monitor is connected or disconnected.

		 \par Example:
		 \code
		 const ae::Monitor* const primaryMonitor = ae::MonitorManager::getPrimaryMonitor();
		 primaryMonitor->update();
		 \endcode

		 \since v0.3.0
		*/
		void update();
		/*!
		 \brief Sets the gamma exponent (the contrast factor) of the monitor in fullscreen mode.
		 \details For this method to have any effect, the window associated to this ae::Monitor must be fullscreen.

		 \param[in] gamma The desired gamma exponent, the value provided must be greater than 0.f

		 \par Example:
		 \code
		 const ae::Monitor* const primaryMonitor = ae::MonitorManager::getPrimaryMonitor();
		 primaryMonitor->setGamma(0.8f);
		 \endcode

		 \sa getGamma()

		 \since v0.3.0
		*/
		void setGamma(float gamma);
		/*!
		 \brief Retrieves the list of all video modes supported in fullscreen mode by the ae::Monitor.
		 \details At the creation of a fullscreen window, the video mode options are limited in order to be compatible with what the graphics driver and the monitor support.\n
		 This method retrieves the complete list of all the video modes that can be used in fullscreen mode.\n
		 The array retrieved is sorted from best to worst, so the first element will always be the best video mode available.

		 \return The list containing all the fullscreen video modes that are supported in descending order

		 \par Example:
		 \code
		 const ae::Monitor* const primaryMonitor = ae::MonitorManager::getPrimaryMonitor();
		 const std::vector<ae::VideoMode>& fullscreenModes = primaryMonitor->getFullscreenModes();
		 \endcode

		 \sa getDesktopMode()

		 \since v0.6.0
		*/
		_NODISCARD inline const std::vector<VideoMode>& getFullscreenModes() const noexcept { return mVModes; }
		/*!
		 \brief Retrieves the original desktop video mode of the ae::Monitor.
		 \details The desktop ae::VideoMode is the video mode used by the monitor prior to any modification by the application and is usually the first element of the list of fullscreen modes.\n
		 This ae::VideoMode can be used to create a "windowed fullscreen" or "borderless fullscreen" window.

		 \return The ae::VideoMode that is currently being used by the monitor which is most often the best

		 \par Example:
		 \code
		 const ae::Monitor* const primaryMonitor = ae::MonitorManager::getPrimaryMonitor();
		 const ae::VideoMode& desktopMode = primaryMonitor->getDesktopMode();
		 \endcode

		 \sa getFullscreenModes()

		 \since v0.6.0
		*/
		_NODISCARD inline const VideoMode& getDesktopMode() const { return *mOriginalVMode; }
		/*!
		 \brief Retrieves the human-readable name of the ae::Monitor.

		 \return The string containing the human-readable name of the ae::Monitor

		 \par Example:
		 \code
		 const ae::Monitor* const primaryMonitor = ae::MonitorManager::getPrimaryMonitor();
		 const std::string& name = primaryMonitor->getName();
		 \endcode

		 \since v0.6.0
		*/
		_NODISCARD inline const std::string& getName() const noexcept { return mName; }
		/*!
		 \brief Retrieves the work area of the ae::Monitor of the upper-left corner in screen coordinates.
		 \details The work area is the area of a monitor not occupied by global task bars or menu bars.\n
		 If no task bar exists, the work area is the monitor resolution in screen coordinates.

		 \return A 2-dimensional ae::Box containing the position from the upper-left corner and the size of the work area

		 \par Example:
		 \code
		 const ae::Monitor* const primaryMonitor = ae::MonitorManager::getPrimaryMonitor();
		 const ae::Box2i& workarea = primaryMonitor->getWorkarea();
		 \endcode

		 \since v0.6.0
		*/
		_NODISCARD inline const Box2i& getWorkarea() const noexcept { return mWorkarea; }
		/*!
		 \brief Retrieves the virtual position in screen coordinates of the upper-left corner of the ae::Monitor.
		 \details The virtual position of a monitor is relevant to the virtual screen for multi-monitor setups.

		 \return The 2-dimensional ae::Vector containing the virtual position in screen coordinates

		 \par Example:
		 \code
		 const ae::Monitor* const primaryMonitor = ae::MonitorManager::getPrimaryMonitor();
		 const ae::Vector2i& virtualPosition = primaryMonitor->getVirtualPosition();
		 \endcode

		 \since v0.6.0
		*/
		_NODISCARD inline const Vector2i& getVirtualPosition() const noexcept { return mVirtualPos; }
		/*!
		 \brief Retrieves the approximate physical size in millimetres of the ae::Monitor.

		 \return The 2-dimensional ae::Vector containing the physical size in millimetres

		 \par Example:
		 \code
		 const ae::Monitor* const primaryMonitor = ae::MonitorManager::getPrimaryMonitor();
		 const ae::Vector2i& physicalSize = primaryMonitor->getPhysicalSize();
		 \endcode

		 \since v0.6.0
		*/
		_NODISCARD inline const Vector2i& getPhysicalSize() const noexcept { return mPhysicalSize; }
		/*!
		 \brief Retrieves the content scale (current DPI / default DPI) of the ae::Monitor.
		 \details The content scale is the ratio between the current DPI and the platform's default DPI.\n
		 This scale is especially important for text rendering and any UI elements as the UI scaling will appear at a reasonable size on all other machines.

		 \return The 2-dimensional ae::Vector containing the content scale of the ae::Monitor

		 \par Example:
		 \code
		 const ae::Monitor* const primaryMonitor = ae::MonitorManager::getPrimaryMonitor();
		 const ae::Vector2f& contentScale = primaryMonitor->getContentScale();
		 \endcode

		 \since v0.6.0
		*/
		_NODISCARD inline const Vector2f& getContentScale() const noexcept { return mContentScale; }
		/*!
		 \brief Retrieves the gamma exponent (the contrast factor) of the monitor in fullscreen mode.

		 \return The current gamma exponent of the monitor

		 \par Example:
		 \code
		 const ae::Monitor* const primaryMonitor = ae::MonitorManager::getPrimaryMonitor();
		 float gamma = primaryMonitor->getGamma();
		 \endcode

		 \sa setGamma()

		 \since v0.6.0
		*/
		_NODISCARD inline float getGamma() const noexcept { return mGamma; }
		/*!
		 \brief Retrieves the internal pointer to the GLFW handle to the monitor.
		 \note This method doesn't have to be used by the API user.

		 \return The pointer to the GLFW handle to the monitor

		 \par Example:
		 \code
		 const ae::Monitor* const primaryMonitor = ae::MonitorManager::getPrimaryMonitor();
		 GLFWmonitor* handle = primaryMonitor->getHandle();
		 \endcode

		 \since v0.6.0
		*/
		_NODISCARD inline GLFWmonitor* const getHandle() const noexcept { return mHandle; }

	private:
		// Private member(s)
		std::vector<VideoMode>     mVModes;        //!< The list of supported video modes
		std::unique_ptr<VideoMode> mOriginalVMode; //!< The original video mode of the monitor
		std::string                mName;          //!< The human-readable name of the monitor
		Box2i                      mWorkarea;      //!< The work area of the monitor
		Vector2i                   mVirtualPos;    //!< The monitor's position on the virtual desktop
		Vector2i                   mPhysicalSize;  //!< The physical size of the monitor in millimetres
		Vector2f                   mContentScale;  //!< The content scale (current DPI / default DPI)
		float                      mGamma;         //!< The gamma exponent (contrast factor)
		GLFWmonitor*               mHandle;        //!< The GLFW handle to the monitor
	};
}
#endif // Aeon_Window_Monitor_H_

/*!
 \class ae::Monitor
 \ingroup window

 The ae::Monitor class is used in the representantion of a monitor of the
 application user's setup. It contains several properties concerning the
 monitor, such as: the current video mode, its virtual position, a readable
 name, an estimated physical size and a gamma ramp.

 The ae::Monitor instances (the application user may have multiple monitors)
 are managed by the ae::MonitorManager singleton.

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2021.01.23
 \copyright MIT License
*/