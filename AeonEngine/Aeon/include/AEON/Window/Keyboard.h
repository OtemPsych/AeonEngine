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

#ifndef Aeon_Window_Keyboard_H_
#define Aeon_Window_Keyboard_H_

#include <yvals_core.h>
#include <string>

#include <AEON/Config.h>

namespace ae
{
	/*!
	 \brief The namespace containing the functions providing real-time access to the keyboard's state.
	*/
	namespace Keyboard
	{
		/*!
		 \brief The enumeration containing all the values of the available keys.
		*/
		enum class Key
		{
			Unknown     = -1,  //!< An unhandled key

			// Printable keys
			Space       = 32,  //!< The Space key (printable key)
			Apostrophe  = 39,  //!< The ' key (printable key)
			Comma       = 44,  //!< The , key (printable key)
			Hyphen      = 45,  //!< The - key (printable key)
			Period      = 46,  //!< The . key (printable key)
			Slash       = 47,  //!< The / key (printable key)
			Num0        = 48,  //!< The 0 key (printable key)
			Num1        = 49,  //!< The 1 key (printable key)
			Num2        = 50,  //!< The 2 key (printable key)
			Num3        = 51,  //!< The 3 key (printable key)
			Num4        = 52,  //!< The 4 key (printable key)
			Num5        = 53,  //!< The 5 key (printable key)
			Num6        = 54,  //!< The 6 key (printable key)
			Num7        = 55,  //!< The 7 key (printable key)
			Num8        = 56,  //!< The 8 key (printable key)
			Num9        = 57,  //!< The 9 key (printable key)
			Semicolon   = 59,  //!< The ; key (printable key)
			Equal       = 61,  //!< The = key (printable key)
			A           = 65,  //!< The A key (printable key)
			B           = 66,  //!< The B key (printable key)
			C           = 67,  //!< The C key (printable key)
			D           = 68,  //!< The D key (printable key)
			E           = 69,  //!< The E key (printable key)
			F           = 70,  //!< The F key (printable key)
			G           = 71,  //!< The G key (printable key)
			H           = 72,  //!< The H key (printable key)
			I           = 73,  //!< The I key (printable key)
			J           = 74,  //!< The J key (printable key)
			K           = 75,  //!< The K key (printable key)
			L           = 76,  //!< The L key (printable key)
			M           = 77,  //!< The M key (printable key)
			N           = 78,  //!< The N key (printable key)
			O           = 79,  //!< The O key (printable key)
			P           = 80,  //!< The P key (printable key)
			Q           = 81,  //!< The Q key (printable key)
			R           = 82,  //!< The R key (printable key)
			S           = 83,  //!< The S key (printable key)
			T           = 84,  //!< The T key (printable key)
			U           = 85,  //!< The U key (printable key)
			V           = 86,  //!< The V key (printable key)
			W           = 87,  //!< The W key (printable key)
			X           = 88,  //!< The X key (printable key)
			Y           = 89,  //!< The Y key (printable key)
			Z           = 90,  //!< The Z key (printable key)
			LBracket    = 91,  //!< The [ key (printable key)
			Backslash   = 92,  //!< The \ key (printable key)
			RBracket    = 93,  //!< The ] key (printable key)
			GraveAccent = 96,  //!< The ` key (printable key)
			World1      = 161, //!< The first non-US key (printable key)
			World2      = 162, //!< The second non-US key (printable key)

			// Function keys
			Escape      = 256, //!< The Escape key (function key)
			Enter       = 257, //!< The Enter key (function key)
			Tab         = 258, //!< The Tab key (function key)
			Backspace   = 259, //!< The Backspace key (function key)
			Insert      = 260, //!< The Insert key (function key)
			Delete      = 261, //!< The Delete key (function key)
			Right       = 262, //!< The Right Arrow key (function key)
			Left        = 263, //!< The Left Arrow key (function key)
			Down        = 264, //!< The Down Arrow key (function key)
			Up          = 265, //!< The Up Arrow key (function key)
			PageUp      = 266, //!< The Page Up key (function key)
			PageDown    = 267, //!< The Page Down key (function key)
			Home        = 268, //!< The Home key (function key)
			End         = 269, //!< The End key (function key)
			CapsLock    = 280, //!< The Caps Lock key (function key)
			ScrollLock  = 281, //!< The Scroll Lock key (function key)
			NumLock     = 282, //!< The Num Lock key (function key)
			PrintScreen = 283, //!< The Print Screen key (function key)
			Pause       = 284, //!< The Pause/Break key (function key)
			F1          = 290, //!< The F1 key (function key)
			F2          = 291, //!< The F2 key (function key)
			F3          = 292, //!< The F3 key (function key)
			F4          = 293, //!< The F4 key (function key)
			F5          = 294, //!< The F5 key (function key)
			F6          = 295, //!< The F6 key (function key)
			F7          = 296, //!< The F7 key (function key)
			F8          = 297, //!< The F8 key (function key)
			F9          = 298, //!< The F9 key (function key)
			F10         = 299, //!< The F10 key (function key)
			F11         = 300, //!< The F11 key (function key)
			F12         = 301, //!< The F12 key (function key)
			F13         = 302, //!< The F13 key (function key)
			F14         = 303, //!< The F14 key (function key)
			F15         = 304, //!< The F15 key (function key)
			F16         = 305, //!< The F16 key (function key)
			F17         = 306, //!< The F17 key (function key)
			F18         = 307, //!< The F18 key (function key)
			F19         = 308, //!< The F19 key (function key)
			F20         = 309, //!< The F20 key (function key)
			F21         = 310, //!< The F21 key (function key)
			F22         = 311, //!< The F22 key (function key)
			F23         = 312, //!< The F23 key (function key)
			F24         = 313, //!< The F24 key (function key)
			F25         = 314, //!< The F25 key (function key)
			Numpad0     = 320, //!< The Numpad 0 key (function key)
			Numpad1     = 321, //!< The Numpad 1 key (function key)
			Numpad2     = 322, //!< The Numpad 2 key (function key)
			Numpad3     = 323, //!< The Numpad 3 key (function key)
			Numpad4     = 324, //!< The Numpad 4 key (function key)
			Numpad5     = 325, //!< The Numpad 5 key (function key)
			Numpad6     = 326, //!< The Numpad 6 key (function key)
			Numpad7     = 327, //!< The Numpad 7 key (function key)
			Numpad8     = 328, //!< The Numpad 8 key (function key)
			Numpad9     = 329, //!< The Numpad 9 key (function key)
			Decimal     = 330, //!< The Numpad Decimal key (function key)
			Divide      = 331, //!< The Numpad / key (function key)
			Multiply    = 332, //!< The Numpad * key (function key)
			Subtract    = 333, //!< The Numpad - key (function key)
			Add         = 334, //!< The Numpad + key (function key)
			NumpadEnter = 335, //!< The Numpad Enter key (function key)
			NumpadEqual = 336, //!< The Numpad = key (function key)
			LShift      = 340, //!< The left Shift key (function key)
			LControl    = 341, //!< The left Control key (function key)
			LAlt        = 342, //!< The left Alt key (function key)
			LSystem     = 343, //!< The left OS-specific key (function key)
			RShift      = 344, //!< The right Shift key (function key)
			RControl    = 345, //!< The right Control key (function key)
			RAlt        = 346, //!< The right Alt key (function key)
			RSystem     = 347, //!< The right OS-specific key (function key)
			Menu        = 348  //!< The Menu key (function key)
		};

		// Function(s)
		/*!
		 \brief Checks if the indicated \a key is currently pressed down.
		 \details This function retrieves the last reported state of the \a key.

		 \param[in] key The ae::Keyboard::Key to check

		 \return True if the \a key is currently pressed down, false otherwise

		 \par Example:
		 \code
		 if (ae::Keyboard::isKeyPressed(ae::Keyboard::Key::W)) {
			...
		 }
		 \endcode

		 \since v0.3.0
		*/
		_NODISCARD AEON_API bool isKeyPressed(Key key);
		/*!
		 \brief Retrieves the name of the indicated \a key based on the user's keyboard-layout.
		 \details The name retrieved will depend on the keyboard's language and layout.\n
		 If the 'Z' key is provided, a US keyboard will retrieve 'Z' whereas a QWERTZ keyboard will retrieve 'Y'.
		 \note Only keys that are marked as printable will return valid results.

		 \param[in] key The ae::Keyboard::Key whose name will be retrieved

		 \return A string contaning the keyboard-layout dependent name of the \a key provided

		 \par Example:
		 \code
		 std::string keyName = ae::Keyboard::getKeyName(ae::Keyboard::Key::W);
		 \endcode

		 \since v0.3.0
		*/
		_NODISCARD AEON_API std::string getKeyName(Key key);
	}
}
#endif // Aeon_Window_Keyboard_H_

/*!
 \namespace ae::Keyboard
 \ingroup window

 The ae::Keyboard namespace contains the enumeration of all the available keys
 and the functions concerning the real-time state of the keyboard. The two
 available functions can be called to retrieve the real-time state of a key and
 to retrieve the name of a key specific to the keyboard-layout of the
 application user.

 Usage example:
 \code
 // Check if the key Z is pressed down
 if (ae::Keyboard::isKeyPressed(ae::Keyboard::Key::Z)) {
	...
 }

 // Retrieve the keyboard-layout name of the key Z
 // QWERTY-Layout: "Z", QWERTZ-Layout: "Y"
 std::string keyName = ae::Keyboard::getKeyName(ae::Keyboard::Key::Z);

 \endcode

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.20
 \copyright MIT License
*/