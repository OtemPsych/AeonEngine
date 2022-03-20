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

#include <AEON/Window/Window.h>

#include <GLFW/glfw3.h>

#include <AEON/Window/internal/InputManager.h>
#include <AEON/Window/MonitorManager.h>
#include <AEON/Graphics/internal/GLCommon.h>

namespace ae
{
	// Public constructor(s)
	Window::Window(const VideoMode& vidMode, const std::string& title, uint32_t style,
	               const ContextSettings& settings)
		: RenderTarget()
		, mTitle(title)
		, mVideoMode(vidMode)
		, mContextSettings(settings)
		, mStandardCursors()
		, mSizeLimits(-1, -1, -1, -1)
		, mAspectRatio(-1, -1)
		, mPosition()
		, mContentScale()
		, mStyle(style)
		, mMonitor(MonitorManager::getInstance().getPrimaryMonitor())
		, mHandle(nullptr)
		, mVerticalSyncEnabled(false)
	{
		// Apply the OpenGL context hints and create the GLFW window
		mContextSettings.apply();
		create();
	}

	// Destructor
	Window::~Window()
	{
		glfwDestroyWindow(mHandle);
	}

	// Public method(s)
	void Window::create()
	{
		// Destroy the current GLFW window (if there is one)
		if (mHandle) {
			glfwDestroyWindow(mHandle);
			mHandle = nullptr;
		}

		// Modify the window's properties based on the style selected
		GLFWmonitor* monitorHandle = nullptr;
		if (mStyle == Style::Fullscreen) {
			monitorHandle = mMonitor->getHandle();
			glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_TRUE);
		}
		else if (mStyle == Style::WindowedFullscreen) {
			// Retrieve the desktop mode of the selected monitor and overwrite the current one
			mVideoMode = mMonitor->getDesktopMode();
			monitorHandle = mMonitor->getHandle();
			glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
		}
		else {
			// Set the windowed mode window hints
			glfwWindowHint(GLFW_RESIZABLE, mStyle & Style::Resizable);
			glfwWindowHint(GLFW_DECORATED, mStyle & Style::Decorated);
		}

		// Set the video mode hints
		const int32_t RED_BITS = mVideoMode.getRedBits();
		const int32_t GREEN_BITS = mVideoMode.getGreenBits();
		const int32_t BLUE_BITS = mVideoMode.getBlueBits();
		const int32_t ALPHA_BITS = Math::min(RED_BITS, Math::min(GREEN_BITS, BLUE_BITS));

		glfwWindowHint(GLFW_RED_BITS, RED_BITS);
		glfwWindowHint(GLFW_GREEN_BITS, GREEN_BITS);
		glfwWindowHint(GLFW_BLUE_BITS, BLUE_BITS);
		glfwWindowHint(GLFW_ALPHA_BITS, ALPHA_BITS);

		// Create the GLFW window based on the selected style
		mHandle = glfwCreateWindow(mVideoMode.getWidth(), mVideoMode.getHeight(), mTitle.c_str(), monitorHandle, nullptr);
		if (!mHandle) {
			AEON_LOG_ERROR("Window creation failed", "Failed to create the GLFW window.\nThe OpenGL context wasn't made current.");
			return;
		}

		// Set the GLFW input event callbacks
		glfwSetMonitorCallback(InputManager::monitor_callback);
		glfwSetWindowCloseCallback(mHandle, InputManager::window_close_callback);
		glfwSetWindowSizeCallback(mHandle, InputManager::window_size_callback);
		glfwSetFramebufferSizeCallback(mHandle, InputManager::framebuffer_size_callback);
		glfwSetWindowContentScaleCallback(mHandle, InputManager::window_content_scale_callback);
		glfwSetWindowPosCallback(mHandle, InputManager::window_pos_callback);
		glfwSetWindowIconifyCallback(mHandle, InputManager::window_iconify_callback);
		glfwSetWindowMaximizeCallback(mHandle, InputManager::window_maximize_callback);
		glfwSetWindowFocusCallback(mHandle, InputManager::window_focus_callback);
		glfwSetWindowRefreshCallback(mHandle, InputManager::window_refresh_callback);

		glfwSetDropCallback(mHandle, InputManager::path_drop_callback);

		glfwSetKeyCallback(mHandle, InputManager::key_callback);
		glfwSetCharCallback(mHandle, InputManager::character_callback);

		glfwSetCursorPosCallback(mHandle, InputManager::cursor_position_callback);
		glfwSetCursorEnterCallback(mHandle, InputManager::cursor_enter_callback);
		glfwSetMouseButtonCallback(mHandle, InputManager::mouse_button_callback);
		glfwSetScrollCallback(mHandle, InputManager::scroll_callback);

		// Make the OpenGL context current
		glfwMakeContextCurrent(mHandle);

		// Retrieve the remaining properties
		glfwGetFramebufferSize(mHandle, &mFramebufferSize.x, &mFramebufferSize.y);
		glfwGetWindowContentScale(mHandle, &mContentScale.x, &mContentScale.y);
		glfwGetWindowPos(mHandle, &mPosition.x, &mPosition.y);
		glfwSetWindowUserPointer(mHandle, this);
	}

	void Window::close() const
	{
		glfwSetWindowShouldClose(mHandle, GLFW_TRUE);
	}

	void Window::display()
	{
		glfwSwapBuffers(mHandle);
		glfwPollEvents();
	}

	void Window::handleEvent(Event* const event)
	{
		if (event->type == Event::Type::WindowResized) {
			WindowResizeEvent* const windowResizeEvent = event->as<WindowResizeEvent>();
			mVideoMode = VideoMode(windowResizeEvent->size.x, windowResizeEvent->size.y, mVideoMode.getRefreshRate(), mVideoMode.getRedBits(), mVideoMode.getGreenBits(), mVideoMode.getBlueBits(), mMonitor);
			windowResizeEvent->handled = true;
		}
		else if (event->type == Event::Type::FramebufferResized) {
			FramebufferResizeEvent* const framebufferResizeEvent = event->as<FramebufferResizeEvent>();
			if (framebufferResizeEvent->handle == 0) {
				mFramebufferSize = framebufferResizeEvent->size;
				GLCall(glViewport(0, 0, mFramebufferSize.x, mFramebufferSize.y));
				framebufferResizeEvent->handled = true;
			}
		}
		else if (event->type == Event::Type::WindowMoved) {
			WindowMoveEvent* const windowMoveEvent = event->as<WindowMoveEvent>();
			mPosition = windowMoveEvent->position;
			windowMoveEvent->handled = true;
		}
		else if (event->type == Event::Type::WindowContentScaleChanged) {
			WindowContentScaleEvent* const windowContentScaleEvent = event->as<WindowContentScaleEvent>();
			mContentScale = windowContentScaleEvent->scale;
			windowContentScaleEvent->handled = true;
		}
		else if (event->type == Event::Type::MonitorDisconnected && !mMonitor) {
			mMonitor = MonitorManager::getInstance().getPrimaryMonitor();
			mVideoMode = VideoMode(mVideoMode.getResolution(), mVideoMode.getRefreshRate(), mVideoMode.getRedBits(), mVideoMode.getGreenBits(), mVideoMode.getBlueBits(), mMonitor);
			if (mStyle == Style::Fullscreen || mStyle == Style::WindowedFullscreen) {
				if (!mVideoMode.isValid()) {
					mVideoMode = mMonitor->getDesktopMode();
				}
				glfwSetWindowMonitor(mHandle, mMonitor->getHandle(), 0, 0, mVideoMode.getWidth(), mVideoMode.getHeight(), mVideoMode.getRefreshRate());
			}
			event->handled = true;
		}
	}

	void Window::requestAttention() const
	{
		glfwRequestWindowAttention(mHandle);
	}

	void Window::minimize(bool flag) const
	{
		(flag) ? glfwIconifyWindow(mHandle) : glfwRestoreWindow(mHandle);
	}

	void Window::maximize(bool flag) const
	{
		(flag) ? glfwMaximizeWindow(mHandle) : glfwRestoreWindow(mHandle);
	}

	bool Window::isMinimized() const
	{
		return glfwGetWindowAttrib(mHandle, GLFW_ICONIFIED);
	}

	bool Window::isMaximized() const
	{
		return glfwGetWindowAttrib(mHandle, GLFW_MAXIMIZED);
	}

	bool Window::isOpen() const
	{
		return !glfwWindowShouldClose(mHandle);
	}

	void Window::enableVerticalSync(bool flag)
	{
		mVerticalSyncEnabled = flag;
		glfwSwapInterval(flag);
	}

	void Window::setTitle(const std::string& title)
	{
		if (mTitle != title) {
			mTitle = title;
			glfwSetWindowTitle(mHandle, title.c_str());
		}
	}

	void Window::setSize(const Vector2i& size)
	{
		assert(size.x > 0 && size.y > 0);

		if (mVideoMode.getResolution() != size) {
			mVideoMode = VideoMode(size.x, size.y, mVideoMode.getRefreshRate(), mVideoMode.getRedBits(), mVideoMode.getGreenBits(), mVideoMode.getBlueBits(), mMonitor);
			glfwSetWindowSize(mHandle, size.x, size.y);
		}
	}

	void Window::setRefreshRate(int32_t refreshRate)
	{
		assert(refreshRate > 0);

		if (getRefreshRate() != refreshRate && (mStyle == Style::Fullscreen || mStyle == Style::WindowedFullscreen)) {
			const int32_t WIDTH = mVideoMode.getWidth();
			const int32_t HEIGHT = mVideoMode.getHeight();
			mVideoMode = VideoMode(WIDTH, HEIGHT, refreshRate, mVideoMode.getRedBits(), mVideoMode.getGreenBits(), mVideoMode.getBlueBits(), mMonitor);
			assert(mVideoMode.isValid());

			glfwSetWindowMonitor(mHandle, mMonitor->getHandle(), 0, 0, WIDTH, HEIGHT, refreshRate);
		}
	}

	void Window::setSizeLimits(const Box2i& limits)
	{
		assert(limits.min.x >= -1 && limits.min.y >= -1 && limits.max.x >= -1 && limits.max.y >= -1);

		mSizeLimits = limits;
		glfwSetWindowSizeLimits(mHandle, limits.min.x, limits.min.y, limits.max.x, limits.max.y);
	}

	void Window::setAspectRatio(const Vector2i& ratio)
	{
		assert(ratio.x >= -1 && ratio.y >= -1);

		mAspectRatio = ratio;
		glfwSetWindowAspectRatio(mHandle, ratio.x, ratio.y);
	}

	void Window::setPosition(const Vector2i& position)
	{
		if (mPosition != position) {
			mPosition = position;
			glfwSetWindowPos(mHandle, position.x, position.y);
		}
	}

	void Window::setStyle(uint32_t style)
	{
		if (mStyle != style) {
			mStyle = style;

			if (mStyle == Style::Fullscreen) {
				// Check if the current video mode is compatible for the monitor and replace it with the desktop mode if it isn't
				if (!mVideoMode.isValid()) {
					mVideoMode = mMonitor->getDesktopMode();
				}

				// Make the window fullscreen
				glfwSetWindowAttrib(mHandle, GLFW_AUTO_ICONIFY, GLFW_TRUE);
				glfwSetWindowMonitor(mHandle, mMonitor->getHandle(), 0, 0, mVideoMode.getWidth(), mVideoMode.getHeight(), mVideoMode.getRefreshRate());
			}
			else if (mStyle == Style::WindowedFullscreen) {
				// Set the monitor's desktop mode and make the window windowed fullscreen
				mVideoMode = mMonitor->getDesktopMode();
				glfwSetWindowAttrib(mHandle, GLFW_AUTO_ICONIFY, GLFW_FALSE);
				glfwSetWindowMonitor(mHandle, mMonitor->getHandle(), 0, 0, mVideoMode.getWidth(), mVideoMode.getHeight(), mVideoMode.getRefreshRate());
			}
			else {
				// Make the window windowed and center it on the monitor
				const Box2i& workarea = mMonitor->getWorkarea();
				const Vector2i& windowSize = mVideoMode.getResolution();
				const Vector2i CENTER = workarea.position + (workarea.size - windowSize) / 2;
				glfwSetWindowMonitor(mHandle, nullptr, CENTER.x, CENTER.y, windowSize.x, windowSize.y, 0);

				// Remove the undesired window attributes and enable the desired ones
				glfwSetWindowAttrib(mHandle, GLFW_DECORATED, mStyle & Style::Decorated);
				glfwSetWindowAttrib(mHandle, GLFW_RESIZABLE, mStyle & Style::Resizable);
			}
		}
	}

	void Window::setMonitor(const Monitor& newMonitor)
	{
		// Verify that the window's style is either Fullscreen or WindowedFullscreen
		if (mMonitor->getHandle() != newMonitor.getHandle() && (mStyle == Style::Fullscreen || mStyle == Style::WindowedFullscreen)) {
			mMonitor = &newMonitor;

			// Modify the associated monitor of the video mode
			const Vector2i RESOLUTION = mVideoMode.getResolution();
			const int32_t REFRESH_RATE = mVideoMode.getRefreshRate();
			mVideoMode = VideoMode(RESOLUTION.x, RESOLUTION.y, REFRESH_RATE, mVideoMode.getRedBits(), mVideoMode.getGreenBits(), mVideoMode.getBlueBits(), mMonitor);

			// Check that the new monitor is compatible with the current video mode and replace it with the desktop mode if it isn't
			if (!mVideoMode.isValid()) {
				mVideoMode = mMonitor->getDesktopMode();
			}

			// Set the new monitor
			glfwSetWindowMonitor(mHandle, mMonitor->getHandle(), 0, 0, RESOLUTION.x, RESOLUTION.y, REFRESH_RATE);
		}
	}

	void Window::setCursor(const Cursor& cursor) const
	{
		GLFWcursor* const cursorHandle = cursor.getHandle();
		assert(cursorHandle);

		glfwSetCursor(mHandle, cursorHandle);
	}

	void Window::setCursor(Cursor::Type type)
	{
		auto cursorEmplacement = mStandardCursors.try_emplace(type, type);
		assert(cursorEmplacement.second);

		GLFWcursor* const cursorHandle = cursorEmplacement.first->second.getHandle();
		assert(cursorHandle);

		glfwSetCursor(mHandle, cursorHandle);
	}
}