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

#ifndef Aeon_Graphics_Button_H_
#define Aeon_Graphics_Button_H_

#include <AEON/Graphics/internal/Widget.h>
#include <AEON/Graphics/RectangleShape.h>

namespace ae
{
	// Forward declaration(s)
	class Text;

	/*!
	 \brief Class representing a GUI button.
	 \details The Idle, Hover and Press states need to be configured.
	*/
	class _NODISCARD AEON_API Button : public Widget<RectangleShape>
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets the ae::Button's and the associated text's origins to their centers.

		 \since v0.5.0
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

		 \since v0.5.0
		*/
		Button(Button&& rvalue) noexcept;
	public:
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

		 \since v0.5.0
		*/
		Button& operator=(Button&& rvalue) noexcept;
	public:
		// Public method(s)
		/*!
		 \brief Retrieves the ae::Button's text.
		 
		 \return The ae::Text member

		 \par Example:
		 \code
		 auto button = std::make_unique<ae::Button>();
		 ae::Text& text = button->getText();
		 \endcode

		 \since v0.5.0
		*/
		_NODISCARD Text& getText() const noexcept;
	private:
		// Private virtual method(s)
		/*!
		 \brief Attaches the text to the active state.

		 \param[in] state The ae::Widget::State that will be enabled

		 \since v0.5.0
		*/
		virtual void enableState(State state) override final;
		/*!
		 \brief Sets the ae::Button to its hover state if it's currently clicked.

		 \param[in] dt The time difference between the previous frame and the current frame

		 \since v0.5.0
		*/
		virtual void updateSelf(const Time& dt) override final;

	private:
		// Private member(s)
		Text* mText; //!< The button's text
	};
}
#endif // Aeon_Graphics_Button_H_

/*!
 \class ae::Button
 \ingroup graphics

 The ae::Button class provides typical functionality of a GUI rectangular
 button. It contains an ae::Text element used to describe its function
 which is automatically situated at the center of the button.

 \author Filippos Gleglakos
 \version v0.5.0
 \date 2020.08.08
 \copyright MIT License
*/