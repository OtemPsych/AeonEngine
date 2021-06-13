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

#ifndef Aeon_Window_Event_H_
#define Aeon_Window_Event_H_

#include <yvals_core.h>
#include <type_traits>
#include <vector>

#include <AEON/Config.h>
#include <AEON/Math/Vector.h>
#include <AEON/System/DebugLogger.h>
#include <AEON/Window/Monitor.h>
#include <AEON/Window/Keyboard.h>
#include <AEON/Window/Mouse.h>

namespace ae
{
	// Forward declaration(s)
	class Font;

	/*!
	 \brief The base class representing a system event and its properties.
	 \details This class is inherited by several dedicated classes based on the event generated.
	*/
	class _NODISCARD AEON_API Event
	{
	public:
		/*!
		 \brief The enumeration containing the different event types that can be processed.
		*/
		enum class Type {
			MonitorConnected,          //!< A monitor was connected (data in MonitorEvent)
			MonitorDisconnected,       //!< A monitor was disconnected (data in MonitorEvent)
			WindowClosed,              //!< The window's close flag was set
			WindowResized,             //!< The window was resized (data in WindowResizeEvent)
			FramebufferResized,        //!< The window's framebuffer was resized (data in FramebufferResizeEvent)
			WindowContentScaleChanged, //!< The window's content scale was changed (data in WindowContentScaleEvent)
			WindowMoved,               //!< The window's position was changed (data in WindowMoveEvent)
			WindowMinimized,           //!< The window was minimized (i.e. iconified)
			WindowMaximized,           //!< The window was maximized (i.e. zoomed)
			WindowRestored,            //!< The window's iconification state was restored (restored from minimization or maximization)
			WindowFocusGained,         //!< The window gained input focus
			WindowFocusLost,           //!< The window lost input focus
			WindowDamaged,             //!< The window's contents are damaged and need to be refreshed

			PathDrop,                  //!< The filepaths of files were dropped on the window (data in PathDropEvent)

			KeyPressed,                //!< A keyboard key was pressed (data in KeyEvent)
			KeyReleased,               //!< A keyboard key was released (data in KeyEvent)
			TextEntered,               //!< A text character was produced (data in TextEvent)

			MouseMoved,                //!< The mouse's cursor was moved (data in MouseMoveEvent)
			MouseEntered,              //!< The mouse's cursor entered the window's content area
			MouseLeft,                 //!< The mouse's cursor left the window's content area
			MouseButtonPressed,        //!< A mouse button was pressed (data in MouseButtonEvent)
			MouseButtonReleased,       //!< A mouse button was released (data in MouseButtonEvent)
			MouseWheelScrolled,        //!< A mouse wheel was scrolled (data in MouseWheelEvent)

			JoystickConnected,         //!< A joystick/controller was connected (data in JoystickEvent)
			JoystickDisconnected,      //!< A joystick/controller was disconnected (data in JoystickEvent)

			FontUpdated                //!< A font's texture atlas was updated (data in FontEvent)
		};

	public:
		// Public member(s)
		const Type type;    //!< The type of the event generated
		bool       handled; //!< Whether the event has already been handled by another element

	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::Event by providing the \a type of the event generated.
		 \note The API user shouldn't construct their own ae::Event objects.

		 \param[in] type The ae::Event::Type of the event generated

		 \since v0.3.0
		*/
		explicit Event(Type type) noexcept;
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		Event(const Event&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.3.0
		*/
		Event(Event&&) = delete;
		/*!
		 \brief Virtual destructor.
		 \details The virtual destructor is needed as ae::Event is a base class.

		 \since v0.3.0
		*/
		virtual ~Event();
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.3.0
		*/
		Event& operator=(const Event&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.3.0
		*/
		Event& operator=(Event&&) = delete;
	public:
		// Public method(s)
		/*!
		 \brief Retrieves the converted pointer to the derived class of the ae::Event.
		 \details If the ae::Event's base pointer isn't convertible to the class provided, nullptr will be returned.
		 \note The type T provided has to inherit from the ae::Event base class.

		 \return A pointer of type T converted from the ae::Event's base pointer

		 \par Example:
		 \code
		 ...
		 ae::Event::Type type = event->type;
		 if (type == ae::Event::Type::MouseWheelScrolled) {              // the data is stored in an ae::MouseWheelEvent
			auto mouseWheelEvent = event->as<ae::MouseWheelEvent>();     // OK, the conversion is possible
			auto windowResizeEvent = event->as<ae::WindowResizeEvent>(); // nullptr, the conversion isn't possible
			auto erroneousEvent = event->as<int>();                      // error!
		 }
		 \endcode

		 \since v0.3.0
		*/
		template <class T, typename = std::enable_if_t<std::is_base_of_v<Event, T>>>
		_NODISCARD T* const as()
		{
			// Attempt to convert the base pointer to a pointer of type T
			T* const Tptr = dynamic_cast<T* const>(this);

			// Log an error message if the base pointer isn't convertible to a type T (ignored in Release mode)
			if _CONSTEXPR_IF (AEON_DEBUG) {
				if (!Tptr) {
					AEON_LOG_ERROR("Inconvertible event pointer", "The current event's base pointer cannot be converted to the type provided.\nReturning nullptr.");
				}
			}

			return Tptr;
		}
	};

	/*!
	 \brief The derived class representing a monitor system event and its properties.
	 \details An ae::MonitorEvent's associated types are: Type::MonitorConnected and Type::MonitorDisconnected.\n
	 This class inherits the ae::Event base class.
	*/
	class _NODISCARD AEON_API MonitorEvent : public Event
	{
	public:
		// Public member(s)
		const Monitor*     monitor; //!< The pointer to the monitor that was either connected or disconnected
		GLFWmonitor* const handle; //!< The GLFW handle to the monitor that was either connected or disconnected

	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::MonitorEvent by providing a pointer to the \a monitor in question and whether it was connected or disconnected.

		 \param[in] handle The GLFW handle to a monitor that was either connected or disconnected
		 \param[in] connected True to signify that the \a monitor was connected, false to signify that it was disconnected

		 \since v0.3.0
		*/
		MonitorEvent(GLFWmonitor* const handle, bool connected) noexcept;
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		MonitorEvent(const MonitorEvent&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.3.0
		*/
		MonitorEvent(MonitorEvent&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.3.0
		*/
		MonitorEvent& operator=(const MonitorEvent&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.3.0
		*/
		MonitorEvent& operator=(MonitorEvent&&) = delete;
	};

	/*!
	 \brief The derived class representing a window resizing event and its properties.
	 \details An ae::WindowResizeEvent's associated types are: Type::WindowResized.\n
	 This class inherits the ae::Event base class.
	*/
	class _NODISCARD AEON_API WindowResizeEvent : public Event
	{
	public:
		// Public member(s)
		const Vector2i size; //!< The window's new size in screen coordinates

	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::WindowResizeEvent by providing the new \a width and \a height of the window.

		 \param[in] width The new width of the window in screen coordinates
		 \param[in] height The new height of the window in screen coordinates

		 \since v0.3.0
		*/
		WindowResizeEvent(int width, int height) noexcept;
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		WindowResizeEvent(const WindowResizeEvent&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.3.0
		*/
		WindowResizeEvent(WindowResizeEvent&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.3.0
		*/
		WindowResizeEvent& operator=(const WindowResizeEvent&) = delete;
		/*!
		 \brief Deleted move assignment operator.
		 
		 \since v0.3.0
		*/
		WindowResizeEvent& operator=(WindowResizeEvent&&) = delete;
	};

	/*!
	 \brief The derived class representing a framebuffer resizing event and its properties.
	 \details An ae::FramebufferResizeEvent's associated types are: Type::FramebufferResized.\n
	 This class inherits the ae::Event base class.
	*/
	class _NODISCARD AEON_API FramebufferResizeEvent : public Event
	{
	public:
		// Public member(s)
		const Vector2i size; //!< The framebuffer's new size in pixels

	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::FramebufferResizeEvent by providing the new \a width and \a height of the framebuffer.

		 \param[in] width The new width of the framebuffer in pixels
		 \param[in] height The new height of the framebuffer in pixels

		 \since v0.3.0
		*/
		FramebufferResizeEvent(int width, int height) noexcept;
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		FramebufferResizeEvent(const FramebufferResizeEvent&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.3.0
		*/
		FramebufferResizeEvent(FramebufferResizeEvent&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.3.0
		*/
		FramebufferResizeEvent& operator=(const FramebufferResizeEvent&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.3.0
		*/
		FramebufferResizeEvent& operator=(FramebufferResizeEvent&&) = delete;
	};

	/*!
	 \brief The derived class representing a window content scale change event and its properties.
	 \details An ae::WindowContentScaleEvent's associated types are: Type::WindowContentScaleChanged.\n
	 This class inherits the ae::Event base class.
	*/
	class _NODISCARD AEON_API WindowContentScaleEvent : public Event
	{
	public:
		// Public member(s)
		const Vector2f scale; //!< The window's new content scale

	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::WindowContentScaleEvent by providing the new content \a scaleX and \a scaleY of the window.

		 \param[in] scaleX The new horizontal content scale of the window
		 \param[in] scaleY The new vertical content scale of the window

		 \since v0.3.0
		*/
		WindowContentScaleEvent(float scaleX, float scaleY) noexcept;
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		WindowContentScaleEvent(const WindowContentScaleEvent&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.3.0
		*/
		WindowContentScaleEvent(WindowContentScaleEvent&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.3.0
		*/
		WindowContentScaleEvent& operator=(const WindowContentScaleEvent&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.3.0
		*/
		WindowContentScaleEvent& operator=(WindowContentScaleEvent&&) = delete;
	};

	/*!
	 \brief The derived class representing a window position change event and its properties.
	 \details An ae::WindowMoveEvent's associated types are: Type::WindowMoved.\n
	 This class inherits the ae::Event base class.
	*/
	class _NODISCARD AEON_API WindowMoveEvent : public Event
	{
	public:
		// Public member(s)
		const Vector2i position; //!< The window's new position

	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::WindowMoveEvent by providing the new position of the window.

		 \param[in] posX The new horizontal position of the window
		 \param[in] posY The new vertical position of the window

		 \since v0.3.0
		*/
		WindowMoveEvent(int posX, int posY) noexcept;
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		WindowMoveEvent(const WindowMoveEvent&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.3.0
		*/
		WindowMoveEvent(WindowMoveEvent&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.3.0
		*/
		WindowMoveEvent& operator=(const WindowMoveEvent&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.3.0
		*/
		WindowMoveEvent& operator=(WindowMoveEvent&&) = delete;
	};

	/*!
	 \brief The derived class representing a filepath drop and its properties.
	 \details An ae::PathDropEvent's associated types are: Type::PathDrop.\n
	 This class inherits the ae::Event base class.
	*/
	class _NODISCARD AEON_API PathDropEvent : public Event
	{
	public:
		// Public member(s)
		std::vector<std::string> paths;

	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::PathDropEvent by providing the number of paths and the \a paths themselves.

		 \param[in] count The number of paths dropped
		 \param[in] paths The C-style strings of the paths dropped

		 \since v0.6.0
		*/
		PathDropEvent(int count, const char** paths) noexcept;
		/*!
		 \brief Deleted copy constructor.

		 \since v0.6.0
		*/
		PathDropEvent(const PathDropEvent&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.6.0
		*/
		PathDropEvent(PathDropEvent&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.6.0
		*/
		PathDropEvent& operator=(const PathDropEvent&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.6.0
		*/
		PathDropEvent& operator=(PathDropEvent&&) = delete;
	};

	/*!
	 \brief The derived class representing a recently-changed state of a key and its properties.
	 \details An ae::KeyEvent's associated types are: Type::KeyPressed and Type::KeyReleased.\n
	 This class inherits the ae::Event base class.
	*/
	class _NODISCARD AEON_API KeyEvent : public Event
	{
	public:
		// Public member(s)
		const Keyboard::Key key;      //!< The key whose state was changed
		const bool          shift;    //!< Whether one or more Shift keys were held down
		const bool          control;  //!< Whether one or more Control keys were held down
		const bool          alt;      //!< Whether one or more Alt keys were held down
		const bool          system;   //!< Whether one or more System (OS-specific key) keys were held down
		const bool          capsLock; //!< Whether the Caps Lock key was enabled
		const bool          numLock;  //!< Whether the Num Lock key was enabled

	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::KeyEvent by providing the affected \a key, whether it was \a pressed and the modifier key flags.

		 \param[in] key The ae::Keyboard::Key whose state was changed
		 \param[in] pressed True to signify that the \a key was pressed, false to signify that it was released
		 \param[in] mods The modifier key flags (Shift, Control, Alt, System, Caps Lock, Num Lock)

		 \since v0.3.0
		*/
		KeyEvent(Keyboard::Key key, bool pressed, int mods) noexcept;
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		KeyEvent(const KeyEvent&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.3.0
		*/
		KeyEvent(KeyEvent&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.3.0
		*/
		KeyEvent& operator=(const KeyEvent&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.3.0
		*/
		KeyEvent& operator=(KeyEvent&&) = delete;
	};

	/*!
	 \brief The derived class representing a text input event and its properties.
	 \details An ae::TextEvent's associated types are: Type::TextEntered.\n
	 This class inherits the ae::Event base class.
	*/
	class _NODISCARD AEON_API TextEvent : public Event
	{
	public:
		// Public member(s)
		const uint32_t unicode; //!< The unicode code point of the character inputted

	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::TextEvent by providing the \a unicode code point of the character.

		 \param[in] unicode The unicode code point of the character inputted

		 \since v0.3.0
		*/
		explicit TextEvent(uint32_t unicode) noexcept;
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		TextEvent(const TextEvent&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.3.0
		*/
		TextEvent(TextEvent&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.3.0
		*/
		TextEvent& operator=(const TextEvent&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.3.0
		*/
		TextEvent& operator=(TextEvent&&) = delete;
	};

	/*!
	 \brief The derived class representing a changed mouse cursor position event and its properties.
	 \details An ae::MouseMoveEvent's associated types are: Type::MouseMoveEvent.\n
	 This class inherits the ae::Event base class.
	*/
	class _NODISCARD AEON_API MouseMoveEvent : public Event
	{
	public:
		// Public member(s)
		const Vector2d position; //!< The mouse cursor's new position

	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::MouseMoveEvent by providing the new position of the mouse cursor.

		 \param[in] posX The horizontal position of the mouse cursor
		 \param[in] posY The vertical position of the mouse cursor

		 \since v0.3.0
		*/
		MouseMoveEvent(double posX, double posY) noexcept;
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		MouseMoveEvent(const MouseMoveEvent&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.3.0
		*/
		MouseMoveEvent(MouseMoveEvent&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.3.0
		*/
		MouseMoveEvent& operator=(const MouseMoveEvent&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.3.0
		*/
		MouseMoveEvent& operator=(MouseMoveEvent&&) = delete;
	};

	/*!
	 \brief The derived class representing a recently-changed state of a mouse button event and its properties.
	 \details An ae::MouseButtonEvent's associated types are: Type::MouseButtonPressed and Type::MouseButtonReleased.\n
	 This class inherits the ae::Event base class.
	*/
	class _NODISCARD AEON_API MouseButtonEvent : public Event
	{
	public:
		// Public member(s)
		const Mouse::Button button;   //!< The mouse button whose state was changed
		const bool          shift;    //!< Whether one or more Shift keys were held down
		const bool          control;  //!< Whether one or more Control keys were held down
		const bool          alt;      //!< Whether one or more Alt keys were held down
		const bool          system;   //!< Whether one or more System (OS-specific key) keys were held down
		const bool          capsLock; //!< Whether the Caps Lock key was enabled
		const bool          numLock;  //!< Whether the Num Lock key was enabled

	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::MouseButtonEvent by providing the \a button whose state was changed, whether it was \a pressed and the modifier key flags.
		 
		 \param[in] button The ae::Mouse::Button whose state was changed
		 \param[in] pressed True to signify that the \a button was pressed down, false to signify that it was released
		 \param[in] mods The modifier key flags (Shift, Control, Alt, System, Caps Lock, Num Lock)

		 \since v0.3.0
		*/
		MouseButtonEvent(Mouse::Button button, bool pressed, int mods) noexcept;
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		MouseButtonEvent(const MouseButtonEvent&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.3.0
		*/
		MouseButtonEvent(MouseButtonEvent&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.3.0
		*/
		MouseButtonEvent& operator=(const MouseButtonEvent&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.3.0
		*/
		MouseButtonEvent& operator=(MouseButtonEvent&&) = delete;
	};

	/*!
	 \brief The derived class representing a mouse wheel scroll event and its properties.
	 \details An ae::MouseWheelEvent's associated types are: Type::MouseWheelScrolled.\n
	 This class inherits the ae::Event base class.
	*/
	class _NODISCARD AEON_API MouseWheelEvent : public Event
	{
	public:
		// Public member(s)
		const Mouse::Wheel wheel; //!< The affected mouse wheel
		const double       offset; //!< The wheel's offset (positive is upwards or leftwards, negative is downwards or rightwards)

	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::MouseWheelEvent by providing the affected \a wheel and the \a wheel's \a offset.

		 \param[in] wheel The affected ae::Mouse::Wheel
		 \param[in] offset The wheel's offset (positive is upwards or leftwards, negative is downwards or rightwards)

		 \since v0.3.0
		*/
		MouseWheelEvent(Mouse::Wheel wheel, double offset) noexcept;
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		MouseWheelEvent(const MouseWheelEvent&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.3.0
		*/
		MouseWheelEvent(MouseWheelEvent&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.3.0
		*/
		MouseWheelEvent& operator=(const MouseWheelEvent&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.3.0
		*/
		MouseWheelEvent& operator=(MouseWheelEvent&&) = delete;
	};

	/*!
	 \brief The derived class representing a font update event and the associated font's properties.
	 \details An ae::FontEvent's associated types are: Type::FontUpdated.\n
	 This class inherits the ae::Event base class.
	*/
	class _NODISCARD AEON_API FontEvent : public Event
	{
	public:
		// Public member(s)
		const Font* const font; //!< The pointer to the affected font

	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::FontEvent by providing the affected font.

		 \param[in] font A pointer to the affected ae::Font

		 \since v0.5.0
		*/
		FontEvent(const Font* const font) noexcept;
		/*!
		 \brief Deleted copy constructor.

		 \since v0.5.0
		*/
		FontEvent(const FontEvent&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.5.0
		*/
		FontEvent(FontEvent&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.5.0
		*/
		FontEvent& operator=(const FontEvent&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.5.0
		*/
		FontEvent& operator=(FontEvent&&) = delete;
	};
}
#endif // Aeon_Window_Event_H_

/*!
 \class ae::Event
 \ingroup window

 The ae::Event class is used to represent a system event that was generated,
 usually due to user input, and for the API user the possibility to handle these
 system events in order to update their game's state.

 An ae::Event instance contains all the information necessary about a specific
 system event. It contains the type of the event as well as all the details
 about that particular event.

 The API user first has to retrieve the type of the event and then convert the
 pointer provided to an ae::Event object of a derived class based on the type
 retrieved. The conversion can be done manually using a dynamic_cast or using
 the as() template function.

 Usage example:
 \code
 ae::Event::Type type = event->type;
 if (!event->handled && type == ae::Event::Type::KeyPressed) {
	auto keyEvent = event->as<ae::KeyEvent>();
	// or
	auto keyEvent = dynamic_cast<ae::KeyEvent>(event);

	if (keyEvent->key == ae::Keyboard::Key::W) {
		...
	}
 }
 \endcode

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.21
 \copyright MIT License
*/

/*!
 \class ae::MonitorEvent
 \ingroup window

 The ae::MonitorEvent class is used to represent a system event that was
 generated concerning a monitor of the application user's monitor setup. It
 contains data about whether a monitor was just connected or disconnected.

 When a monitor is either connected or disconnected, the ae::MonitorManager
 instance will be notified and it will automatically update its list of
 connected monitors by either adding in the connected monitor or by removing
 the disconnected monitor. Since the removal of a disconnected monitor is
 automatic, the pointer retrieved may not be valid when the API user receives it.

 Usage example:
 \code
 // The 'event' parameter is provided by the overloaded method 'handleEvent()' of the ae::State class
 if (event->type == ae::Event::Type::MonitorConnected) {
	auto monitorEvent = event->as<ae::MonitorEvent>();
	const ae::Monitor* const connectedMonitor = monitorEvent->monitor;
 }
 \endcode

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.29
 \copyright MIT License
*/

/*!
 \class ae::WindowResizeEvent
 \ingroup window

 The ae::WindowResizeEvent class is used to represent a system event that was
 generated concerning the window's size. It contains the window's new size
 in screen coordinates following its resizing.

 This class is used for a similar purpose to the ae::FramebufferResizeEvent,
 but the main difference is that the ae::WindowResizeEvent provides the new
 window's size in screen coordinates and not in pixels. This is especially
 important for systems that don't map screen coordinates and pixels 1:1.

 Usage example:
 \code
 // The 'event' parameter is provided by the overloaded method 'handleEvent()' of the ae::State class
 if (event->type == ae::Event::Type::WindowResized) {
	auto windowResizeEvent = event->as<ae::WindowResizeEvent>();
	const ae::Vector2i& newWindowSize = windowResizeEvent->size;
 }
 \endcode

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.21
 \copyright MIT License
*/

/*!
 \class ae::FramebufferResizeEvent
 \ingroup window

 The ae::FramebufferResizeEvent class is used to represent a system event that
 was generated concerning the window's framebuffer's size. It contains the
 framebuffer's new size in pixels.

 This class is used for a similar purpose to the ae::WindowResizeEvent, but the
 main difference is that the ae::FramebufferResizeEvent provides the new
 framebuffer's size in pixels and not in screen coordinates. This is especially
 important for systems that don't map pixels and screen coordinates 1:1.

 Usage example:
 \code
 // The 'event' parameter is provided by the overloaded method 'handleEvent()' of the ae::State class
 if (event->type == ae::Event::Type::FramebufferResized) {
	auto framebufferResizeEvent = event->as<ae::FramebufferResizeEvent>();
	const ae::Vector2i& newFramebufferSize = framebufferResizeEvent->size;
 }
 \endcode

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.21
 \copyright MIT License
*/

/*!
 \class ae::WindowContentScaleEvent
 \ingroup window
 
 The ae::WindowContentScaleEvent class is used to represent a system event that
 was generated concerning the window's content scale. It contains the new
 horizontal and vertical content scale.

 The content scale is the ratio between the window's current DPI and the
 platform's default DPI. It's especially important for text rendering and
 any GUI/UI elements. If the UI elements scaled by the window's content scale
 look appropriate on the API user's machine then it should also appear at a
 reasonable size on the application users' machines.

 Usage example:
 \code
 // The 'event' parameter is provided by the overloaded method 'handleEvent()' of the ae::State class
 if (event->type == ae::Event::Type::WindowContentScaleChanged) {
	auto windowContentScaleEvent = event->as<ae::WindowContentScaleEvent>();
	const ae::Vector2f& newContentScale = windowContentScaleEvent->scale;
 }
 \endcode

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.21
 \copyright MIT License
*/

/*!
 \class ae::WindowMoveEvent
 \ingroup window

 The ae::WindowMoveEvent class is used to represent a system event that was
 generated concerning the window's changed position. It contains the window's
 new position after being moved by either the application user or by the system.

 The stored position represents the upper-left corner of the window's content
 area and it's in screen coordinates. The coordinate system used is right to
 left and top to bottom. So, the position (0,0) is located at the upper-left
 corner of the monitor to which the window belongs.

 Usage example:
 \code
 // The 'event' parameter is provided by the overloaded method 'handleEvent()' of the ae::State class
 if (event->type == ae::Event::Type::WindowMoved) {
	auto windowMoveEvent = event->as<ae::WindowMoveEvent>();
	const ae::Vector2i& windowPosition = windowMoveEvent->position;
 }
 \endcode

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.21
 \copyright MIT License
*/

/*!
 \class ae::PathDropEvent
 \ingroup window

 The ae::PathDropEvent is used to represent a drop of one or multiple filepaths
 on the window. It contains the list of all the filepaths dropped in a stl
 vector.

 Usage example:
 \code
  // The 'event' parameter is provided by the overloaded method 'handleEvent()' of the ae::State class
 if (event->type == ae::Event::Type::PathDrop) {
	auto pathDropEvent = event->as<ae::PathDropEvent>();
	const std::vector<std::string>& paths = pathDropEvent->paths;
 }
 \endcode

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2021.06.13
 \copyright MIT License
*/

/*!
 \class ae::KeyEvent
 \ingroup window

 The ae::KeyEvent class is used to represent a system event that was generated
 concerning the recently-changed state of a key. It contains the key whose
 state was changed and whether any modifier key flags were raised.

 The state of the key is either pressed or released and is indicated by the
 event's type, and the modifier key flags are whether the shift, control, alt
 or system (OS-specific key) were held down when the event was generated, or if
 the Caps Lock key or the Num Lock key were enabled when it was generated.

 Usage example:
 \code
 // The 'event' parameter is provided by the overloaded method 'handleEvent()' of the ae::State class
 if (event->type == ae::Event::Type::KeyPressed) {
	auto keyEvent = event->as<ae::KeyEvent>();
	if (keyEvent->key == ae::Keyboard::Key::W && keyEvent->shift) {
		...
	}
 }
 else if (event->type == ae::Event::Type::KeyReleased) {
	...
 }
 \endcode

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.21
 \copyright MIT License
*/

/*!
 \class ae::TextEvent
 \ingroup window

 The ae::TextEvent class is used to represent a system event that was generated
 concerning a character input. It contains the unicode code point of the
 character inputted.

 Unlike key input, text input obeys keyboard layouts and modifier keys (such as:
 Shift, Control, Alt) and supports composing characters using dead keys.

 Usage example:
 \code
 // The 'event' parameter is provided by the overloaded method 'handleEvent()' of the ae::State class
 if (event->type == ae::Event::Type::TextEntered) {
	auto textEvent = event->as<ae::TextEvent>();
	uint32_t unicode = textEvent->unicode; // the unicode code point (the character)
 }
 \endcode

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.22
 \copyright MIT License
*/

/*!
 \class ae::MouseMoveEvent
 \ingroup window

 The ae::MouseMoveEvent class is used to represent a system event that was
 generated concerning the recently changed position of the mouse cursor. It
 contains the new position of the mouse cursor.

 The mouse cursor position is measured in screen coordinates and is relative to
 the top-left corner of the window's content area. On platforms that provide it,
 the full sub-pixel cursor position is passed on.

 Usage example:
 \code
 // The 'event' parameter is provided by the overloaded method 'handleEvent()' of the ae::State class
 if (event->type == ae::Event::Type::MouseMoved) {
	auto mouseMoveEvent = event->as<ae::MouseMoveEvent>();
	const ae::Vector2d& newMousePosition = mouseMoveEvent->position;
 }
 \endcode

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.22
 \copyright MIT License
*/

/*!
 \class ae::MouseButtonEvent
 \ingroup window

 The ae::MouseButtonEvent class is used to represent a system event that was
 generated concerning the recently changed state of a mouse button. It contains
 the button whose state was changed and whether it was pressed or released
 which is indicated by the event's type.

 The modifier key flags are whether the shift, control, alt or system
 (OS-specific key) were held down when the event was generated, or if the Caps
 Lock key or the Num Lock key were enabled when it was generated.

 Usage example:
 \code
 // The 'event' parameter is provided by the overloaded method 'handleEvent()' of the ae::State class
 if (event->type == ae::Event::Type::MouseButtonPressed) {
	auto mouseButtonEvent = event->as<ae::MouseButtonEvent>();
	if (mouseButtonEvent->button == ae::Mouse::Button::Left && mouseButtonEvent->shift) {
		...
	}
 }
 else if (event->type == ae::Event::Type::MouseButtonReleased) {
	...
 }
 \endcode

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.23
 \copyright MIT License
*/

/*!
 \class ae::MouseWheelEvent
 \ingroup window

 The ae::MouseWheelEvent class is used to represent a system event that was
 generated concerning the offset of a mouse wheel. It contains the affected
 mouse wheel (vertical or horizontal) and its offset.

 The offset is positive when the mouse wheel was scrolled upwards or leftwards,
 and negative when the mouse wheel was scrolled downwards or rightwards.

 Usage example:
 \code
 // The 'event' parameter is provided by the overloaded method 'handleEvent()' of the ae::State class
 if (event->type == ae::Event::Type::MouseWheelScrolled) {
	auto mouseWheelEvent = event->as<ae::MouseWheelEvent>();
	double wheelOffset = mouseWheelEvent->offset;
	if (mouseWheelEvent->wheel == ae::Mouse::Wheel::Vertical) {
		...
	}
 }
 \endcode

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.22
 \copyright MIT License
*/

/*!
 \class ae::FontEvent
 \ingroup window

 The ae::FontEvent class is used to represent an API event that was generated
 concerning the update of the specified font's texture atlas. It contains a
 pointer to the affected font.

 This event is primarily used internally in order for all ae::Text instances to
 be informed when they should update their uv coordinates.

 Usage example:
 \code
 // The 'event' parameter is provided by the overloaded method 'handleEvent()' of the ae::State class
 if (event->type == ae::Event::Type::FontUpdated) {
	auto fontEvent = event->as<ae::FontEvent>();
	if (fontEvent->font == mFont) {
		updateTextUVCoordinates();
	}
 }
 \endcode

 \author Filippos Gleglakos
 \version v0.5.0
 \date 2020.08.07
 \copyright MIT License
*/