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

#ifndef Aeon_Window_Mouse_H_
#define Aeon_Window_Mouse_H_

#include <yvals_core.h>

#include <AEON/Config.h>
#include <AEON/Math/Vector.h>

namespace ae
{
	/*!
	 \brief The namespace containing the functions providing real-time access to the mouse's state.
	*/
	namespace Mouse
	{
		/*!
		 \brief The enumeration containing all the values of the available mouse buttons.
		*/
		enum class Button
		{
			Left,     //!< The left mouse button
			Right,    //!< The right mouse button
			Middle,   //!< The middle mouse button
			XButton1, //!< The first extra mouse button
			XButton2, //!< The second extra mouse button
			XButton3, //!< The third extra mouse button
			XButton4, //!< The fourth extra mouse button
			XButton5  //!< The fifth extra mouse button
		};

		/*!
		 \brief The enumeration containing the available mouse wheels.
		*/
		enum class Wheel
		{
			Vertical,  //!< The vertical mouse wheel
			Horizontal //!< The horizontal mouse wheel
		};

		// Function(s)
		/*!
		 \brief Checks if the indicated \a button is currently pressed down.
		 \details This function retrieves the last reported state of the \a button.

		 \param[in] button The ae::Mouse::Button to check

		 \return True if the \a button is currently pressed down, false otherwise

		 \par Example:
		 \code
		 if (ae::Mouse::isButtonPressed(ae::Mouse::Button::Left)) {
			...
		 }
		 \endcode

		 \since v0.3.0
		*/
		_NODISCARD AEON_API bool isButtonPressed(Button button);
		/*!
		 \brief Retrieves the mouse cursor's current position from the top-left corner of the window.
		 \details The mouse cursor's position is measured in screen coordinates and goes from left to right and top to bottom.

		 \return The mouse cursor's current position from the top-left corner of the window.

		 \par Example:
		 \code
		 ae::Vector2d mousePos = ae::Mouse::getPosition();
		 \endcode

		 \since v0.3.0
		*/
		_NODISCARD AEON_API Vector2d getPosition();
		/*!
		 \brief Hides and locks the mouse cursor to the center of the active window.
		 \details This function is particularly useful when one wishes to implement camera rotation based on the movement of the mouse or other similar input schemes requiring unlimited mouse movement.

		 \param[in] flag True to lock the mouse cursor to the active window, false to unlock it

		 \par Example:
		 \code
		 ae::Mouse::grabMouse(true); // The mouse cursor is locked and hidden
		 \endcode

		 \sa isMouseGrabbed()

		 \since v0.3.0
		*/
		AEON_API void grabMouse(bool flag);
		/*!
		 \brief Checks if the mouse cursor is currently grabbed by the active window.

		 \return True if the mouse cursor is locked by the active window, false otherwise

		 \par Example:
		 \code
		 if (ae::Mouse::isMouseGrabbed()) {
			...
		 }
		 \endcode

		 \sa grabMouse()

		 \since v0.3.0
		*/
		_NODISCARD AEON_API bool isMouseGrabbed();
	}
}
#endif // Aeon_Window_Mouse_H_

/*!
 \namespace ae::Mouse
 \ingroup window

 The ae::Mouse namespace contains the enumeration of all the available mouse
 buttons and mouse wheels, and the functions concerning the real-time state of
 the mouse. It's possible to retrieve the last reported state of a mouse button
 and the current position of the mouse cursor.

 It's also possible to "grab" the mouse to simulate a 3D camera. By grabbing
 the mouse, we mean to hide and lock the mouse cursor in order to implement
 a camera rotation using the movement of the mouse. This sort of movement
 requires an unlimited space of movement which is why grabbing the mouse is
 necessary.

 Usage example:
 \code
 // Check if the right mouse button is currently pressed down
 if (ae::Mouse::isButtonPressed(ae::Mouse::Button::Right)) {
	...
 }

 // Retrieve the mouse cursor's current position
 ae::Vector2d mousePos = ae::Mouse::getPosition();

 // Hide and lock the mouse cursor to the window
 ae::Mouse::grabMouse(true);
 // Retrieve the virtual position of the mouse cursor (the space of movement of the mouse is now considered to be virtual)
 ae::Vector2d virtualMousePos = ae::Mouse::getPosition();
 \endcode

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.20
 \copyright MIT License
*/