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

#ifndef Aeon_Window_InputManager_H_
#define Aeon_Window_InputManager_H_

#include <AEON/Config.h>

// Forward declaration(s)
struct GLFWmonitor;
struct GLFWwindow;

namespace ae
{
	/*!
	 \brief The namespace containing several callback functions that create and enqueue input events generated.
	 \note This namespace is considered to be internal but may still be used by the API user.
	*/
	namespace InputManager
	{
		/*!
		 \brief Callback function that receives the monitor that was either connected or disconnected.
		 \details Enqueues an ae::Event::Type::MonitorConnected or an ae::Event::Type::MonitorDisconnected event.

		 \param[in] glfwMonitor The GLFW handle to the monitor in question
		 \param[in] connected Whether the monitor was connected or disconnected

		 \since v0.4.0
		*/
		AEON_API void monitor_callback(GLFWmonitor* glfwMonitor, int connected);
		/*!
		 \brief Callback function that receives the notification that the user wishes to close the window.
		 \details Erases the window's close flag and enqueues an ae::Event::Type::WindowClosed event.

		 \param[in] glfwWindow The GLFW handle to the window

		 \since v0.3.0
		*/
		AEON_API void window_close_callback(GLFWwindow* glfwWindow);
		/*!
		 \brief Callback function that receives the window's new size in screen coordinates when it's resized.
		 \details Enqueues an ae::Event::Type::WindowResized event.

		 \param[in] glfwWindow The GLFW handle to the window
		 \param[in] width The new width of the window
		 \param[in] height The new height of the window

		 \since v0.3.0
		*/
		AEON_API void window_size_callback(GLFWwindow* glfwWindow, int width, int height);
		/*!
		 \brief Callback function that receives the framebuffer's new size in pixels when it's resized.
		 \details Enqueues an ae::Event::Type::FramebufferResized event.

		 \param[in] glfwWindow The GLFW handle to the window
		 \param[in] width The new width of the framebuffer
		 \param[in] height The new height of the framebuffer

		 \since v0.3.0
		*/
		AEON_API void framebuffer_size_callback(GLFWwindow* glfwWindow, int width, int height);
		/*!
		 \brief Callback function that receives the window's new content scale when modified.
		 \details Enqueues an ae::Event::Type::WindowContentScaleChanged event.

		 \param[in] glfwWindow The GLFW handle to the window
		 \param[in] xscale The horizontal content scale
		 \param[in] yscale The vertical content scale

		 \since v0.3.0
		*/
		AEON_API void window_content_scale_callback(GLFWwindow* glfwWindow, float xscale, float yscale);
		/*!
		 \brief Callback function that receives the window's new position when it's moved.
		 \details Enqueues an ae::Event::Type::WindowMoved event.

		 \param[in] glfwWindow The GLFW handle to the window
		 \param[in] xpos The horizontal position of the window
		 \param[in] ypos The vertical position of the window

		 \since v0.3.0
		*/
		AEON_API void window_pos_callback(GLFWwindow* glfwWindow, int xpos, int ypos);
		/*!
		 \brief Callback function that receives the notification that the window was either minimized (iconified) or restored from minimization.
		 \details Enqueues an ae::Event::Type::WindowMinimized or an ae::Event::Type::WindowRestored event.

		 \param[in] glfwWindow The GLFW handle to the window
		 \param[in] iconified Whether the window was minimized or restored

		 \since v0.3.0
		*/
		AEON_API void window_iconify_callback(GLFWwindow* glfwWindow, int iconified);
		/*!
		 \brief Callback function that receives the notification that the window was either maximized or restored from maximization.
		 \details Enqueues an ae::Event::Type::WindowMaximized or an ae::Event::Type::WindowRestored event.

		 \param[in] glfwWindow The GLFW handle to the window
		 \param[in] maximized Whether the window was maximized or restored

		 \since v0.4.0
		*/
		AEON_API void window_maximize_callback(GLFWwindow* glfwWindow, int maximized);
		/*!
		 \brief Callback function that receives the notification that the window has gained focus or lost focus.
		 \details Enqueues an ae::Event::Type::WindowFocusGained or ae::Event::Type::WindowFocusLost event.

		 \param[in] glfwWindow The GLFW handle to the window
		 \param[in] focused True to signify that the window gained focus, false to signify that it lost focus

		 \since v0.3.0
		*/
		AEON_API void window_focus_callback(GLFWwindow* glfwWindow, int focused);
		/*!
		 \brief Callback function that receives the notification that the window's contents have been damaged and need to be refreshed.
		 \details Enqueues an ae::Event::Type::WindowDamaged event.

		 \param[in] glfwWindow The GLFW handle to the window

		 \since v0.3.0
		*/
		AEON_API void window_refresh_callback(GLFWwindow* glfwWindow);

		/*!
		 \brief Callback function that receives the recently changed state of a key.
		 \details Enqueues an ae::Event::Type::KeyPressed or an ae::Event::Type::KeyReleased event.
		 
		 \param[in] glfwWindow The GLFW handle to the window
		 \param[in] key The key whose state was changed
		 \param[in] scancode The unique platform-specific scancode for the key
		 \param[in] action Whether the key was pressed down or released
		 \param[in] mods The modifier key flags that were set when the event was generated (Shift, Control, Alt, etc.)

		 \since v0.3.0
		*/
		AEON_API void key_callback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);
		/*!
		 \brief Callback function that receives the unicode character input.
		 \details Enqueues an ae::Event::Type::TextEntered event.

		 \param[in] glfwWindow The GLFW handle to the window
		 \param[in] codepoint The unicode codepoint of the character

		 \since v0.3.0
		*/
		AEON_API void character_callback(GLFWwindow* glfwWindow, unsigned int codepoint);

		/*!
		 \brief Callback function that receives the mouse cursor's new position.
		 \details Enqueues an ae::Event::Type::MouseMoved event.

		 \param[in] glfwWindow The GLFW handle to the window
		 \param[in] xpos The horizontal position of the mouse cursor
		 \param[in] ypos The vertical position of the mouse cursor

		 \since v0.3.0
		*/
		AEON_API void cursor_position_callback(GLFWwindow* glfwWindow, double xpos, double ypos);
		/*!
		 \brief Callback function that receives the notification that mouse cursor either entered or left the window's content area.
		 \details Enqueues an ae::Event::Type::MouseEntered or an ae::Event::Type::MouseLeft event.

		 \param[in] glfwWindow The GLFW handle to the window
		 \param[in] entered True to signify that mouse cursor entered the content area, false to signify that it left the content area

		 \since v0.3.0
		*/
		AEON_API void cursor_enter_callback(GLFWwindow* glfwWindow, int entered);
		/*!
		 \brief Callback function that receives the recently changed state of a mouse button.
		 \details Enqueues an ae::Event::Type::MouseButtonPressed or an ae::Event::Type::MouseButtonReleased event.

		 \param[in] glfwWindow The GLFW handle to the window
		 \param[in] button The button whose state was changed
		 \param[in] action Whether the button was pressed down or released
		 \param[in] mods The modifier key flags that were set when the event was generated (Shift, Control, Alt, etc.)

		 \since v0.3.0
		*/
		AEON_API void mouse_button_callback(GLFWwindow* glfwWindow, int button, int action, int mods);
		/*!
		 \brief Callback function that receives the recently changed state of a mouse wheel.
		 \details Enqueues an ae::Event::Type::MouseWheelScrolled event.

		 \param[in] glfwWindow The GLFW handle to the window
		 \param[in] xoffset The offset of a horizontal mouse wheel
		 \param[in] yoffset The offset of a vertical mouse wheel

		 \since v0.3.0
		*/
		AEON_API void scroll_callback(GLFWwindow* glfwWindow, double xoffset, double yoffset);
	}
}
#endif // Aeon_Window_InputManager_H_

/*!
 \namespace ae::InputManager
 \ingroup window

 The ae::InputManager namespace contains several callback functions used by
 GLFW once an event has been generated. These callback functions are
 responsible for creating the appropriate event and enqueuing it in the
 ae::EventQueue instance's queue.

 This namespace is considered an internal namespace meaning that the API user
 doesn't need to by concerned with it .

 \author Filippos Gleglakos
 \version v0.4.0
 \date 2020.05.08
 \copyright MIT License
*/