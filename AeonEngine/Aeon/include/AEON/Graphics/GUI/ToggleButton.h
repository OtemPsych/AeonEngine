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

#include <AEON/Graphics/GUI/internal/Widget.h>
#include <AEON/Graphics/RectangleShape.h>
#include <AEON/Graphics/Text.h>

namespace ae
{
	/*!
	 \brief Class representing a toggleable GUI button.
	 \details The Idle, Hover and Click states need to be configured.
	*/
	template <typename T = RectangleShape>
	class AEON_API ToggleButton : public Widget<T>
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets the ae::ToggleButton's and the associated text's origins to their centers.

		 \since v0.7.0
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

		// Public method(s)
		/*!
		 \brief (Un)Toggles the ae::ToggleButton based on the \a flag provided

		 \param[in] True to toggle on, false to untoggle

		 \par Example:
		 \code
		 auto toggleButton = std::make_unique<ae::ToggleButton>();
		 toggleButton->toggle(true);
		 \endcode

		 \since v0.7.0
		*/
		void toggle(bool flag) noexcept;
		/*!
		 \brief Retrieves the ae::ToggleButton's text.

		 \return The ae::Text member

		 \par Example:
		 \code
		 auto toggleButton = std::make_unique<ae::ToggleButton>();
		 ae::Text& text = toggleButton->getText();
		 \endcode

		 \since v0.7.0
		*/
		[[nodiscard]] Text& getText() const noexcept;
	private:
		// Private virtual method(s)
		/*!
		 \brief Updates the ae::ToggleButton's state based on the input event.
		 \details Updates the active state based on mouse movement and mouse clicks.

		 \param[in] event The polled input ae::Event

		 \since v0.7.0
		*/
		virtual void handleEventSelf(Event* const event) override final;

	private:
		// Private member(s)
		Text* mText; //!< The toggle button's text
	};
}
#include <AEON/Graphics/GUI/ToggleButton.inl>

/*!
 \class ae::ToggleButton
 \ingroup graphics

 The ae::ToggleButton class provides typical functionality of a GUI rectangular
 button in addition to being toggleable, meaning that once it's clicked it will
 remain pressed down. It contains an ae::Text element used to describe its
 function which is automatically situated at the center of the toggle button.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.12.28
 \copyright MIT License
*/