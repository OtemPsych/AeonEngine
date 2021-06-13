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

#ifndef Aeon_Graphics_GUI_Textbox_H_
#define Aeon_Graphics_GUI_Textbox_H_

#include <AEON/Graphics/GUI/internal/Widget.h>
#include <AEON/Graphics/RectangleShape.h>

namespace ae
{
	// Forward declaration(s)
	class Text;
	class Sprite;

	/*!
	 \brief Class representing a GUI textbox.
	 \details The Idle, Hover and Click states need to be configured.
	*/
	class _NODISCARD AEON_API Textbox : public Widget<RectangleShape>
	{
	public:
		Textbox();
	public:
		Text& getText() noexcept;
		Text& getPlaceholder() noexcept;
	private:
		virtual void enableState(State state) override final;
		virtual void updateSelf(const Time& dt) override final;
		virtual void handleEventSelf(Event* const event) override final;
		
	private:
		unsigned int mCharacterCount;
		Text*        mText;
		Text*        mPlaceholder;
	};
}
#endif // Aeon_Graphics_GUI_Textbox_H_

/*!
 \class ae::Textbox
 \ingroup graphics

 The ae::Textbox class provided typical functionality of a GUI textbox, such as
 typing text

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2020.08.17
 \copyright MIT License
*/