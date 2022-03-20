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
	 \brief Class representing a GUI button.
	 \details The Idle and Hover states need to be configured.
	*/
	template <typename T = RectangleShape>
	class AEON_API Button : public Widget<T>
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets the ae::Button's and the associated text's origins to their centers.

		 \since v0.7.0
		*/
		Button();
		/*!
		 \brief Deleted copy constructor.

		 \since v0.5.0
		*/
		Button(const Button&) = delete;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Button that will be moved

		 \since v0.7.0
		*/
		Button(Button&& rvalue) noexcept;
		/*!
		 \brief Virtual destructor.
		 \details This is needed as the API user may derive from it.

		 \since v0.7.0
		*/
		virtual ~Button() = default;

		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.5.0
		*/
		Button& operator=(const Button&) = delete;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Button that will be moved

		 \return The caller ae::Button

		 \since v0.6.0
		*/
		Button& operator=(Button&& rvalue) noexcept;

		// Public method(s)
		/*!
		 \brief Retrieves the ae::Button's text.
		 
		 \return The ae::Text member

		 \par Example:
		 \code
		 auto button = std::make_unique<ae::Button>();
		 ae::Text& text = button->getText();
		 \endcode

		 \since v0.7.0
		*/
		[[nodiscard]] Text& getText() const noexcept;
	protected:
		// Protected virtual method(s)
		/*!
		 \brief Sets the ae::Button to its hover state if it's currently clicked.

		 \param[in] dt The time difference between the previous frame and the current frame

		 \since v0.7.0
		*/
		virtual void updateSelf(const Time& dt) override;
		/*!
		 \brief Updates the ae::Button's state based on the input event.
		 \details Updates the active state based on mouse movement and mouse clicks.

		 \param[in] event The polled input ae::Event

		 \since v0.6.0
		*/
		virtual void handleEventSelf(Event* const event) override;

	private:
		// Private member(s)
		Text* mText; //!< The button's text
	};
}
#include <AEON/Graphics/GUI/Button.inl>

/*!
 \class ae::Button
 \ingroup graphics

 The ae::Button class provides typical functionality of a GUI rectangular
 button. It contains an ae::Text element used to describe its function
 which is automatically situated at the center of the button.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.12.27
 \copyright MIT License
*/