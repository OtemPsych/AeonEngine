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

#ifndef Aeon_Graphics_GUI_ToggleButton_H_
#define Aeon_Graphics_GUI_ToggleButton_H_

#include <AEON/Graphics/GUI/internal/Widget.h>
#include <AEON/Graphics/RectangleShape.h>

namespace ae
{
	// Forward declaration(s)
	class Text;

	/*!
	 \brief Class representing a toggleable GUI button.
	 \details The Idle, Hover, Press and Click states need to be configured.
	*/
	class _NODISCARD AEON_API ToggleButton : public Widget<RectangleShape>
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets the ae::ToggleButton's and the associated text's origins to their centers.

		 \since v0.6.0
		*/
		ToggleButton();
		/*!
		 \brief Deleted copy constructor.

		 \since v0.6.0
		*/
		ToggleButton(const ToggleButton&) = delete;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::ToggleButton that will be moved

		 \since v0.6.0
		*/
		ToggleButton(ToggleButton&& rvalue) noexcept;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.6.0
		*/
		ToggleButton& operator=(const ToggleButton&) = delete;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::ToggleButton that will be moved

		 \return The caller ae::ToggleButton

		 \since v0.6.0
		*/
		ToggleButton& operator=(ToggleButton&& rvalue) noexcept;
	public:
		// Public method(s)
		/*!
		 \brief Retrieves the ae::ToggleButton's text.

		 \return The ae::Text member

		 \par Example:
		 \code
		 auto toggleButton = std::make_unique<ae::ToggleButton>();
		 ae::Text& text = toggleButton->getText();
		 \endcode

		 \since v0.6.0
		*/
		_NODISCARD Text& getText() const noexcept;
	private:
		// Private virtual method(s)
		/*!
		 \brief Attaches the text to the active state.

		 \param[in] state The ae::ToggleButton::State that will be enabled

		 \since v0.6.0
		*/
		virtual void enableState(State state) override final;
		/*!
		 \brief Updates the ae::ToggleButton's state based on the input event.
		 \details Updates the active state based on mouse movement and mouse clicks.

		 \param[in] event The polled input ae::Event

		 \since v0.6.0
		*/
		virtual void handleEventSelf(Event* const event) override final;

	private:
		// Private member(s)
		Text* mText;    //!< The toggle button's text
	};
}
#endif // Aeon_Graphics_GUI_ToggleButton_H_

/*!
 \class ae::ToggleButton
 \ingroup graphics

 The ae::ToggleButton class provides typical functionality of a GUI rectangular
 button in addition to being toggleable, meaning that once it's clicked it will
 remain pressed down. It contains an ae::Text element used to describe its
 function which is automatically situated at the center of the toggle button.

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2020.08.17
 \copyright MIT License
*/