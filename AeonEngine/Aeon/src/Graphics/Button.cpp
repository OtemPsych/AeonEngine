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

#include <AEON/Graphics/Button.h>
#include <AEON/Graphics/Text.h>

namespace ae
{
	// Public constructor(s)
	Button::Button()
		: Widget()
		, mText(nullptr)
	{
		// Attach a text member to the active state
		auto text = std::make_unique<Text>();
		mText = text.get();
		getState(getActiveState()).attachChild(std::move(text));

		// Set the button's origin to the center
		setOriginFlags(OriginFlag::Center);

		// Set the text's origin to the center and align it relative to the button's center
		mText->setOriginFlags(OriginFlag::Center);
		mText->setRelativeAlignment(OriginFlag::Center);
	}

	Button::Button(Button&& rvalue) noexcept
		: Widget(std::move(rvalue))
		, mText(std::move(rvalue.mText))
	{
	}

	// Public operator(s)
	Button& Button::operator=(Button&& rvalue) noexcept
	{
		// Copy the rvalue's trivial data and move the rest
		Widget::operator=(std::move(rvalue));
		mText = std::move(rvalue.mText);

		return *this;
	}

	// Public method(s)
	Text& Button::getText() const noexcept
	{
		return *mText;
	}

	// Private virtual method(s)
	void Button::enableState(State state)
	{
		auto text = getState(getActiveState()).detachChild(*mText);
		Widget::enableState(state);
		getState(getActiveState()).attachChild(std::move(text));
		correctProperties();
	}

	void Button::updateSelf(const Time& dt)
	{
		// Set the active state to hover if it's currently clicked
		if (getActiveState() == State::Click) {
			enableState(State::Hover);
		}
		else {
			Widget::updateSelf(dt);
		}
	}
}