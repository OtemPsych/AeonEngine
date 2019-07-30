// MIT License
// 
// Copyright(c) 2019 Filippos Gleglakos
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

#include <AEON/Window/internal/InputManager.h>

#include <GLFW/glfw3.h>

#include <AEON/Window/internal/EventQueue.h>
#include <AEON/Window/Event.h>
#include <AEON/Window/Monitor.h>

namespace ae
{
	namespace InputManager
	{
		EventQueue& eventQueue = EventQueue::getInstance(); //!< The single instance of the ae::EventQueue

		// Callback function(s)
		void monitor_callback(GLFWmonitor* glfwMonitor, int connected)
		{
			// Create and enqueue the event
			auto event = std::make_unique<MonitorEvent>(glfwMonitor, connected == GLFW_CONNECTED);
			eventQueue.enqueueEvent(std::move(event));
		}

		void window_close_callback(GLFWwindow* glfwWindow)
		{
			// Erase the close flag
			glfwSetWindowShouldClose(glfwWindow, GLFW_FALSE);

			// Create and enqueue the event
			auto event = std::make_unique<Event>(Event::Type::WindowClosed);
			eventQueue.enqueueEvent(std::move(event));
		}

		void window_size_callback(GLFWwindow* glfwWindow, int width, int height)
		{
			// Create and enqueue the event
			auto event = std::make_unique<WindowResizeEvent>(width, height);
			eventQueue.enqueueEvent(std::move(event));
		}

		void framebuffer_size_callback(GLFWwindow* glfwWindow, int width, int height)
		{
			// Create and enqueue the event
			auto event = std::make_unique<FramebufferResizeEvent>(width, height);
			eventQueue.enqueueEvent(std::move(event));
		}

		void window_content_scale_callback(GLFWwindow* glfwWindow, float xscale, float yscale)
		{
			// Create and enqueue the event
			auto event = std::make_unique<WindowContentScaleEvent>(xscale, yscale);
			eventQueue.enqueueEvent(std::move(event));
		}

		void window_pos_callback(GLFWwindow* glfwWindow, int xpos, int ypos)
		{
			// Create and enqueue the event
			auto event = std::make_unique<WindowMoveEvent>(xpos, ypos);
			eventQueue.enqueueEvent(std::move(event));
		}

		void window_iconify_callback(GLFWwindow* glfwWindow, int iconified)
		{
			// Create and enqueue the event
			auto event = std::make_unique<Event>((iconified) ? Event::Type::WindowMinimized : Event::Type::WindowRestored);
			eventQueue.enqueueEvent(std::move(event));
		}

		void window_maximize_callback(GLFWwindow* glfwWindow, int maximized)
		{
			// Create and enqueue the event
			auto event = std::make_unique<Event>((maximized) ? Event::Type::WindowMaximized : Event::Type::WindowResized);
			eventQueue.enqueueEvent(std::move(event));
		}

		void window_focus_callback(GLFWwindow* glfwWindow, int focused)
		{
			// Create and enqueue the event
			auto event = std::make_unique<Event>((focused) ? Event::Type::WindowFocusGained : Event::Type::WindowFocusLost);
			eventQueue.enqueueEvent(std::move(event));
		}

		void window_refresh_callback(GLFWwindow* glfwWindow)
		{
			// Create and enqueue the event
			auto event = std::make_unique<Event>(Event::Type::WindowDamaged);
			eventQueue.enqueueEvent(std::move(event));
		}

		void key_callback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
		{
			// Create and enqueue the event
			auto event = std::make_unique<KeyEvent>(static_cast<Keyboard::Key>(key), action != GLFW_RELEASE, mods);
			eventQueue.enqueueEvent(std::move(event));
		}

		void character_callback(GLFWwindow* glfwWindow, unsigned int codepoint)
		{
			// Create and enqueue the event
			auto event = std::make_unique<TextEvent>(codepoint);
			eventQueue.enqueueEvent(std::move(event));
		}

		void cursor_position_callback(GLFWwindow* glfwWindow, double xpos, double ypos)
		{
			// Create and enqueue the event
			auto event = std::make_unique<MouseMoveEvent>(xpos, ypos);
			eventQueue.enqueueEvent(std::move(event));
		}

		void cursor_enter_callback(GLFWwindow* glfwWindow, int entered)
		{
			// Create and enqueue the event
			auto event = std::make_unique<Event>((entered) ? Event::Type::MouseEntered : Event::Type::MouseLeft);
			eventQueue.enqueueEvent(std::move(event));
		}

		void mouse_button_callback(GLFWwindow* glfwWindow, int button, int action, int mods)
		{
			// Create and enqueue the event
			auto event = std::make_unique<MouseButtonEvent>(static_cast<Mouse::Button>(button), action != GLFW_RELEASE, mods);
			eventQueue.enqueueEvent(std::move(event));
		}

		void scroll_callback(GLFWwindow* glfwWindow, double xoffset, double yoffset)
		{
			// Check which mouse wheel was affected
			Mouse::Wheel wheel = Mouse::Wheel::Vertical;
			double offset = yoffset;
			if (xoffset != 0.0) {
				wheel = Mouse::Wheel::Horizontal;
				offset = xoffset;
			}

			// Create and enqueue the event
			auto event = std::make_unique<MouseWheelEvent>(wheel, offset);
			eventQueue.enqueueEvent(std::move(event));
		}
	}
}