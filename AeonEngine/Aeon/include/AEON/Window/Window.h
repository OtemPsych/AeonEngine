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

#include <AEON/Window/ContextSettings.h>
#include <AEON/Window/Cursor.h>
#include <AEON/Graphics/internal/RenderTarget.h>

// Forward declaration(s)
struct GLFWwindow;

namespace ae
{
	/*!
	 \brief The class representing the application's window.
	*/
	class AEON_API Window : public RenderTarget
	{
	public:
		/*!
		 \brief The style flags defining the window's appearance.
		 \details The flags Resizable and Decorated can be combined together.
		*/
		enum Style {
			Fullscreen         = 0,
			WindowedFullscreen = 1,
			Resizable          = 2 << 0,
			Decorated          = 2 << 1,

			Default = Resizable | Decorated
		};

	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::Window by providing at least a video mode and a title.
		 \details Attaches a camera suited for a 2D scene by default.

		 \param[in] vidMode The ae::VideoMode containing the properties of the video mode to use
		 \param[in] title The string indicating the name of the window
		 \param[in] style The optional ae::Window::Style flags that will define the window's appearance, resizable and decorated by default
		 \param[in] settings The optional ae::ContextSettings containing the settings of the OpenGL context

		 \par Example:
		 \code
		 ae::Application& app = ae::Application::getInstance();
		 app.createWindow(ae::VideoMode(1280, 720), "My Application");
		 \endcode

		 \since v0.4.0
		*/
		Window(const VideoMode& vidMode, const std::string& title, uint32_t style = Style::Default,
		       const ContextSettings& settings = ContextSettings());
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		Window(const Window&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.3.0
		*/
		Window(Window&&) = delete;
		/*!
		 \brief Destructor.
		 \details Destroys the associated GLFW window and OpenGL context.

		 \since v0.3.0
		*/
		~Window();

		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.3.0
		*/
		Window& operator=(const Window&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.3.0
		*/
		Window& operator=(Window&&) = delete;

		// Public method(s)
		/*!
		 \brief (Re)Creates the GLFW window using the currently stored properties.
		 \details Sets the appropriate video mode hints, the style flags and creates the GLFW window.

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 // Recreates the active window
		 mWindow.create();
		 \endcode

		 \since v0.7.0
		*/
		void create();
		/*!
		 \brief Indicates to the window to release all allocated resources and destroy it.

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 mWindow.close();
		 \endcode

		 \sa isOpen()

		 \since v0.7.0
		*/
		void close() const;
		/*!
		 \brief Displays onto the screen what has been rendered to the window thus far.
		 \details This method swaps the backbuffer with the frontbuffer currently displayed on the screen.
		 \note This method should primarily be used internally.

		 \since v0.7.0
		*/
		void display();
		/*!
		 \brief Handles the polled input \a event received if it's of concern to the window.
		 \details This method is automatically called internally.

		 \param[in] event A pointer to the polled input ae::Event that was generated

		 \since v0.4.0
		*/
		void handleEvent(Event* const event);
		/*!
		 \brief Requests the application user's attention.
		 \details The system will highlight the specified window, or on platforms where this is not supported, the application as a whole.

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 mWindow.requestAttention();
		 \endcode

		 \since v0.7.0
		*/
		void requestAttention() const;
		/*!
		 \brief Minimizes (i.e. iconifies) or restores the ae::Window based on the \a flag provided.

		 \param[in] flag True to minimize the ae::Window, false to restore it

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 mWindow.minimize(true);  // minimize the window (iconify it)
		 ...
		 mWindow.minimize(false); // restore the minimized window
		 \endcode

		 \sa isMinimized(), maximize()

		 \since v0.7.0
		*/
		void minimize(bool flag) const;
		/*!
		 \brief Maximizes (i.e. zooms) or restores the ae::Window based on the \a flag provided.

		 \param[in] flag True to maximize the ae::Window, false to restore it

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 mWindow.maximize(true);  // maximize the window
		 ...
		 mWindow.maximize(false); // restore the maximized window
		 \endcode

		 \sa isMaximized(), minimize()

		 \since v0.7.0
		*/
		void maximize(bool flag) const;
		/*!
		 \brief Checks if the ae::Window is currently minimized (i.e. iconified).

		 \return True if the ae::Window is minimized, false otherwise

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 if (mWindow.isMinimized()) {
			...
		 }
		 \endcode

		 \sa minimize(), isMaximized()

		 \since v0.7.0
		*/
		[[nodiscard]] bool isMinimized() const;
		/*!
		 \brief Checks if the ae::Window is currently maximized (i.e. zoomed).

		 \return True if the ae::Window is maximized, false otherwise

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 if (mWindow.isMaximized()) {
			...
		 }
		 \endcode

		 \sa maximize(), isMinimized()

		 \since v0.7.0
		*/
		[[nodiscard]] bool isMaximized() const;
		/*!
		 \brief Checks if the ae::Window is still open.
		 \details This method is primarily used internally.

		 \return True if it's still open, false otherwise

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 while (mWindow.isOpen()) {
			...
		 }
		 \endcode

		 \sa close()

		 \since v0.7.0
		*/
		[[nodiscard]] bool isOpen() const;

		/*!
		 \brief (De)activates vertical synchronization, deactivated by default.
		 \details The activation of vertical synchronization will limit the number of frames displayed to the refresh rate of the monitor.\n
		 This can avoid certain visual artifacts and limit the framerate to a stable value.\n
		 One of the disadvantages is that it can introduce input lag.

		 \param[in] flag True to enable vertical synchronization, false to deactivate it

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 mWindow.enableVerticalSync(true);
		 \endcode

		 \sa isVerticalSyncEnabled()

		 \since v0.7.0
		*/
		void enableVerticalSync(bool flag);
		/*!
		 \brief Sets the ae::Window's title displayed on decorated windows and in a task bar.
		 
		 \param[in] title A string containing the ae::Window's new title

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 mWindow.setTitle("My Title");
		 \endcode

		 \sa getTitle()

		 \since v0.7.0
		*/
		void setTitle(const std::string& title);
		/*!
		 \brief Changes the size of the ae::Window's content area if it's in windowed mode or its resolution if it's in fullscreen mode.
		 \details This method sets the size in screen coordinates of the window's content area without taking into account the title bar nor window frame.
		 \note This shouldn't be used to set the size of the framebuffer which is in pixel-based OpenGL coordinates.

		 \param[in] size A 2-dimensional ae::Vector containing the new size/resolution

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 mWindow.setSize(ae::Vector2i(1920, 1080));
		 \endcode

		 \sa getSize()

		 \since v0.7.0
		*/
		void setSize(const Vector2i& size);
		/*!
		 \brief Changes the ae::Window's refresh rate if it's in fullscreen mode.
		 \note This method will have no effect if the window's not in Fullscreen mode or in WindowedFullscreen mode.

		 \param[in] refreshRate The new refresh rate of the ae::Window

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 mWindow.setRefreshRate(59);
		 \endcode

		 \sa getRefreshRate()

		 \since v0.7.0
		*/
		void setRefreshRate(int32_t refreshRate);
		/*!
		 \brief Sets the minimum and maximum size \a limits of the ae::Window's content area.
		 \note The size \a limits are only in effect when the window is in windowed mode.\n
		 A value of -1 means that that limit isn't in effect.

		 \param[in] limits A 2-dimensional ae::Box containing the minimum and maximum size limits of the ae::Window, -1 to disable the limit
		 
		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member

		 // Minimum size: 200x200, Maximum size: 400x400
		 mWindow.setSizeLimits(ae::Box2i(200, 200, 400, 400));

		 // Minimum size: 640x480, Maximum size: unlimited
		 mWindow.setSizeLimits(ae::Box2i(640, 480, -1, -1));
		 \endcode

		 \sa getSizeLimits();

		 \since v0.7.0
		*/
		void setSizeLimits(const Box2i& limits);
		/*!
		 \brief Sets the aspect ratio of the ae::Window's content area.
		 \details The window's size may be resized freely by the application user but it will be constrained to maintain the aspect ratio.\n
		 In order to maintain the window's current aspect ratio, the current size can be provided as the ratio.

		 \param[in] ratio A 2-dimensional ae::Vector containing the aspect ratio to maintain, -1 to disable the aspect ratio limit

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 
		 // 4:3 aspect ratio
		 mWindow.setAspectRatio(ae::Vector2i(4, 3));

		 // 16:9 aspect ratio
		 mWindow.setAspectRatio(ae::Vector2i(16, 9));

		 // maintain current aspect ratio
		 mWindow.setAspectRatio(mWindow.getSize());

		 // remove the aspect ratio limitation
		 mWindow.setAspectRatio(ae::Vector2i(-1, -1));
		 \endcode

		 \sa getAspectRatio()

		 \since v0.7.0
		*/
		void setAspectRatio(const Vector2i& ratio);
		/*!
		 \brief Sets the position of the ae::Window from the upper-left corner of its content area in screen coordinates.
		 \note The window system may put limitations on window placement.

		 \param[in] position An 2-dimensional ae::Vector containing the new position in screen coordinates

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 mWindow.setPosition(ae::Vector2i(100, 100));
		 \endcode

		 \sa getPosition()

		 \since v0.7.0
		*/
		void setPosition(const Vector2i& position);
		/*!
		 \brief Changes the ae::Window's appearance by providing ae::Window::Style flags.

		 \param[in] style The ae::Window::Style flags defining the ae::Window's appearance

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 mWindow.setStyle(ae::Window::Style::Fullscreen);
		 \endcode

		 \sa getStyle()

		 \since v0.7.0
		*/
		void setStyle(uint32_t style);
		/*!
		 \brief Changes the active monitor of the ae::Window by providing the desired ae::Monitor.
		 \note In order for this method to have any effect, the ae::Window's ae::Window::Style must be either Fullscreen or WindowedFullscreen.\n
		 If the current video mode is incompatible with the new monitor, the video mode will be replaced by the desktop mode of the new ae::Monitor.

		 \param[in] newMonitor The ae::Monitor on which the ae::Window will be placed

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 ae::MonitorManager& monitorManager = ae::MonitorManager::getInstance();
		 const size_t MONITOR_COUNT = monitorManager.getMonitorCount();
		 if (MONITOR_COUNT > 1) {
			const ae::Monitor& secondMonitor = monitorManager.getMonitor(1);
			mWindow.setMonitor(secondMonitor);
		 }
		 \endcode

		 \sa getMonitor(), setStyle()

		 \since v0.7.0
		*/
		void setMonitor(const Monitor& newMonitor);
		/*!
		 \brief Sets a new personalized cursor for the window.
		 \details The ae::Cursor provided needs to be stored locally throughout its lifetime.

		 \param[in] cursor The ae::Cursor containing the custom pixel data

		 \par Example:
		 \code
		 // 16x16 image with 4 color channels (RGBA)
		 std::array<unsigned char, 16 * 16 * 4> pixels;
		 pixels.fill(0xff);

		 // Cursor with white 16x16 image with its position of action at the top left corner
		 ae::Cursor cursor(pixels.data(), ae::Vector2i(16, 16), ae::Vector2i(0, 0));

		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 mWindow.setCursor(cursor);
		 \endcode

		 \sa setCursor(Cursor::Type)

		 \since v0.7.0
		*/
		void setCursor(const Cursor& cursor) const;
		/*!
		 \brief Sets a new standard-themed OS cursor for the window.
		 \details The standard-themed cursors are stored by the ae::Window instance, the API user therefore doesn't need to worry about managing them.

		 \param[in] type The ae::Cursor::Type indicating which standard cursor to apply

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 mWindow.setCursor(ae::Cursor::Type::Hand);
		 \endcode

		 \sa setCursor(const Cursor&)
		 
		 \since v0.7.0
		*/
		void setCursor(Cursor::Type type);
		/*!
		 \brief Checks whether or not VSync is enabled.

		 \return True if it's enabled, false otherwise

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 bool vsyncEnabled = mWindow.isVerticalSyncEnabled();
		 \endcode

		 \sa enableVerticalSync()

		 \since v0.7.0
		*/
		[[nodiscard]] inline bool isVerticalSyncEnabled() const noexcept { return mVerticalSyncEnabled; }
		/*!
		 \brief Retrieves the ae::Window's current title.

		 \return The string containing the window's title

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 const std::string& windowTitle = mWindow.getTitle();
		 \endcode

		 \sa setTitle()

		 \since v0.7.0
		*/
		[[nodiscard]] inline const std::string& getTitle() const noexcept { return mTitle; }
		/*!
		 \brief Retrieves the ae::Window's video mode.
		 \details The video mode contains the size/resolution of the window as well as its size and bits per color channel.

		 \return The ae::VideoMode of the ae::Window

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 const ae::VideoMode& windowVidMode = mWindow.getVideoMode();
		 \endcode

		 \sa getSize(), getRefreshRate()

		 \since v0.7.0
		*/
		[[nodiscard]] inline const VideoMode& getVideoMode() const noexcept { return mVideoMode; }
		/*!
		 \brief Retrieves the size of the ae::Window's content area (if it's in windowed mode) or its resolution (if it's in fullscreen mode).
		 \details This method retrieves the size in screen coordinates of the window's content area without taking into account the title bar nor the window's frame.
		 \note This method doesn't retrieve the size of the framebuffer which is in pixel-based OpenGL coordinates.\n
		 The ae::Window's size/resolution can also be retrieved by calling the 'getVideoMode()' method which contains more properties.

		 \return The 2-dimensional ae::Vector containing the ae::Window's size/resolution

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 const ae::Vector2i& windowSize = mWindow.getSize();
		 \endcode

		 \sa setSize(), getVideoMode(), getRefreshRate()
		 
		 \since v0.7.0
		*/
		[[nodiscard]] inline const Vector2i& getSize() const noexcept { return mVideoMode.getResolution(); }
		/*!
		 \brief Retrieves the ae::Window's refresh rate.
		 \note The ae::Window's refresh rate can also be retrieved by calling the 'getVideoMode()' method which contains more properties.

		 \return The ae::Window's refresh rate

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 int32_t windowRefreshRate = mWindow.getRefreshRate();
		 \endcode

		 \sa getVideoMode(), getSize()

		 \since v0.7.0
		*/
		[[nodiscard]] inline int32_t getRefreshRate() const noexcept { return mVideoMode.getRefreshRate(); }
		/*!
		 \brief Retrieves the ae::Window's OpenGL context settings.

		 \return The ae::ContextSettings containing the applied context settings

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 const ae::ContextSettings& contextSettings = mWindow.getContextSettings();
		 \endcode

		 \since v0.7.0
		*/
		[[nodiscard]] inline const ContextSettings& getContextSettings() const noexcept { return mContextSettings; }
		/*!
		 \brief Retrieves the minimum and maximum size limits of the ae::Window's content area.
		 \note The size limits are only in effect when the window is in windowed mode.\n
		 A value of -1 means that that limit isn't in effect.

		 \return A 2-dimensional ae::Box containing the minimum and maximum size limits of the ae::Window

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 const ae::Box2i& sizeLimits = mWindow.getSizeLimits();
		 \endcode

		 \sa setSizeLimits();

		 \since v0.7.0
		*/
		[[nodiscard]] inline const Box2i& getSizeLimits() const noexcept { return mSizeLimits; }
		/*!
		 \brief Retrieves the aspect ratio of the ae::Window's content area.
		 \details The window's size may be resized freely by the application user but it will be constrained to maintain the aspect ratio.

		 \return The 2-dimensional ae::Vector containing the constrained aspect ratio of the ae::Window

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 const ae::Vector2i& aspectRatio = mWindow.getAspectRatio();
		 \endcode

		 \sa setAspectRatio()

		 \since v0.7.0
		*/
		[[nodiscard]] inline const Vector2i& getAspectRatio() const noexcept { return mAspectRatio; }
		/*!
		 \brief Retrieves the position of the windowed-mode ae::Window from the upper-left corner of its content area in screen coordinates.

		 \return The 2-dimensional ae::Vector containing the position of the ae::Window from its upper-left corner

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 const ae::Vector2i& windowPosition = mWindow.getPosition();
		 \endcode

		 \sa setPosition()

		 \since v0.7.0
		*/
		[[nodiscard]] inline const Vector2i& getPosition() const noexcept { return mPosition; }
		/*!
		 \brief Retrieves the content scale (current DPI / default DPI) of the ae::Window.
		 \details The content scale is the ratio between the current DPI and the platform's default DPI.\n
		 This scale is especially important for text rendering and any UI elements as the UI scaling will appear at a reasonable size for all machines.

		 \return The 2-dimensional ae::Vector containing the content scale of the ae::Monitor

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 const ae::Vector2f& contentScale = mWindow.getContentScale();
		 \endcode

		 \since v0.7.0
		*/
		[[nodiscard]] inline const Vector2f& getContentScale() const noexcept { return mContentScale; }
		/*!
		 \brief Retrieves the ae::Window's ae::Window::Style flags that define the window's appearance.
		 
		 \return The ae::Window::Style flags defining the window's appearance

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 uint32_t windowStyle = mWindow.getStyle();
		 \endcode

		 \sa setStyle()

		 \since v0.7.0
		*/
		[[nodiscard]] inline uint32_t getStyle() const noexcept { return mStyle; }
		/*!
		 \brief Retrieves the pointer to the ae::Monitor that the ae::Window belongs to.
		 \details This pointer is primarily only useful if the ae::Window is in fullscreen mode on a specific monitor.

		 \return The pointer to the ae::Monitor to which the ae::Window belongs to

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 const ae::Monitor* const activeMonitor = mWindow.getMonitor();
		 \endcode

		 \sa setMonitor()

		 \since v0.7.0
		*/
		[[nodiscard]] inline const Monitor* const getMonitor() const noexcept { return mMonitor; }
		/*!
		 \brief Retrieves the internal pointer to the GLFW handle to the window.
		 \note This method doesn't have to be used by the API user.

		 \return The pointer to the GLFW handle to the window

		 \par Example:
		 \code
		 // The protected member 'mWindow' is provided by the ae::State class, all derived classes have access to this member
		 GLFWwindow* winHandle = mWindow.getHandle();
		 \endcode

		 \since v0.7.0
		*/
		[[nodiscard]] inline GLFWwindow* const getHandle() const noexcept { return mHandle; }

	private:
		// Private member(s)
		std::string                              mTitle;               //!< The window's title
		VideoMode                                mVideoMode;           //!< The window's video mode
		ContextSettings                          mContextSettings;     //!< The OpenGL context's settings
		std::unordered_map<Cursor::Type, Cursor> mStandardCursors;     //!< The standard-themed OS cursors
		Box2i                                    mSizeLimits;          //!< The minimum and maximum size of the window's content area
		Vector2i                                 mAspectRatio;         //!< The aspect ratio of the window's content area
		Vector2i                                 mPosition;            //!< The position of the window in screen coordinates
		Vector2f                                 mContentScale;        //!< The content scale (current DPI / default DPI)
		uint32_t                                 mStyle;               //!< The window's appearance
		const Monitor*                           mMonitor;             //!< The pointer to the monitor to which the window belongs
		GLFWwindow*                              mHandle;              //!< The GLFW handle to the window
		bool                                     mVerticalSyncEnabled; //!< Whether VSync is enabled
	};
}

/*!
 \class ae::Window
 \ingroup window

 The ae::Window class is used to create a window on which the API user can draw
 by using the available classes from the Graphics module.

 The application's ae::Window object is accessible from all classes inheriting
 the ae::Layer class via the protected member 'mWindow'.

 All configuration concerning the window are done by using this class's methods.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.12.31
 \copyright MIT License
*/