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

#include <AEON/Window/Event.h>

#include <GLFW/glfw3.h>

namespace ae
{
	// Event
		// Public constructor(s)
	Event::Event(Type type) noexcept
		: type(type)
		, handled(false)
	{
	}

		// Virtual destructor
	Event::~Event()
	{
	}

	// MonitorEvent
		// Public constructor(s)
	MonitorEvent::MonitorEvent(GLFWmonitor* const handle, bool connected) noexcept
		: Event((connected) ? Type::MonitorConnected : Type::MonitorDisconnected)
		, monitor((connected) ? nullptr : static_cast<const Monitor*>(glfwGetMonitorUserPointer(handle)))
		, handle(handle)
	{
	}

	// WindowResizeEvent
		// Public constructor(s)
	WindowResizeEvent::WindowResizeEvent(int width, int height) noexcept
		: Event(Type::WindowResized)
		, size(width, height)
	{
	}

	// FramebufferResizeEvent
		// Public constructor(s)
	FramebufferResizeEvent::FramebufferResizeEvent(int width, int height) noexcept
		: Event(Type::FramebufferResized)
		, size(width, height)
	{
	}

	// WindowContentScaleEvent
		// Public constructor(s)
	WindowContentScaleEvent::WindowContentScaleEvent(float scaleX, float scaleY) noexcept
		: Event(Type::WindowContentScaleChanged)
		, scale(scaleX, scaleY)
	{
	}

	// WindowMoveEvent
		// Public constructor(s)
	WindowMoveEvent::WindowMoveEvent(int posX, int posY) noexcept
		: Event(Type::WindowMoved)
		, position(posX, posY)
	{
	}

	// KeyEvent
		// Public constructor(s)
	KeyEvent::KeyEvent(Keyboard::Key key, bool pressed, int mods) noexcept
		: Event((pressed) ? Type::KeyPressed : Type::KeyReleased)
		, key(key)
		, shift(mods & GLFW_MOD_SHIFT)
		, control(mods & GLFW_MOD_CONTROL)
		, alt(mods & GLFW_MOD_ALT)
		, system(mods & GLFW_MOD_SUPER)
		, capsLock(mods & GLFW_MOD_CAPS_LOCK)
		, numLock(mods & GLFW_MOD_NUM_LOCK)
	{
	}

	// TextEvent
		// Public constructor(s)
	TextEvent::TextEvent(uint32_t unicode) noexcept
		: Event(Type::TextEntered)
		, unicode(unicode)
	{
	}

	// MouseMoveEvent
		// Public constructor(s)
	MouseMoveEvent::MouseMoveEvent(double posX, double posY) noexcept
		: Event(Type::MouseMoved)
		, position(posX, posY)
	{
	}

	// MouseButtonEvent
		// Public constructor(s)
	MouseButtonEvent::MouseButtonEvent(Mouse::Button button, bool pressed, int mods) noexcept
		: Event((pressed) ? Type::MouseButtonPressed : Type::MouseButtonReleased)
		, button(button)
		, shift(mods & GLFW_MOD_SHIFT)
		, control(mods & GLFW_MOD_CONTROL)
		, alt(mods & GLFW_MOD_ALT)
		, system(mods & GLFW_MOD_SUPER)
		, capsLock(mods & GLFW_MOD_CAPS_LOCK)
		, numLock(mods & GLFW_MOD_NUM_LOCK)
	{
	}

	// MouseWheelEvent
		// Public constructor(s)
	MouseWheelEvent::MouseWheelEvent(Mouse::Wheel wheel, double offset) noexcept
		: Event(Type::MouseWheelScrolled)
		, wheel(wheel)
		, offset(offset)
	{
	}

	// FontEvent
		// Public constructor(s)
	FontEvent::FontEvent(const Font* const font) noexcept
		: Event(Type::FontUpdated)
		, font(font)
	{
	}
}