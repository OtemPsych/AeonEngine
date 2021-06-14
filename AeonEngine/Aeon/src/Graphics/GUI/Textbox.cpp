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

#include <AEON/Graphics/GUI/Textbox.h>

#include <AEON/System/Clipboard.h>

#include <AEON/Graphics/Text.h>

namespace ae
{
	Textbox::Textbox()
		: Widget()
		, mText(nullptr)
		, mPlaceholder(nullptr)
	{		
		// Attach a text instance to the active state
		auto text = std::make_unique<Text>();
		mText = text.get();
		getState(getActiveState()).attachChild(std::move(text));

		// Set the text's origin to its top center and align it relative to the textbox's top center
		mText->setOriginFlags(OriginFlag::Left | OriginFlag::CenterY);
		mText->setRelativeAlignment(OriginFlag::Left | OriginFlag::CenterY, Vector2f(2.f, 0.f));
		mText->activateFunctionality(Func::Render, Target::Self, false);

		// Attach a placeholder instance to the active state
		auto placeholder = std::make_unique<Text>();
		mPlaceholder = placeholder.get();
		getState(getActiveState()).attachChild(std::move(placeholder));

		// Set the placeholder's origin to its top center and align it relative to the textbox's top center
		mPlaceholder->setOriginFlags(OriginFlag::Left | OriginFlag::CenterY);
		mPlaceholder->setRelativeAlignment(OriginFlag::Left | OriginFlag::CenterY, Vector2f(2.f, 0.f));
	}

	Text& Textbox::getText() noexcept
	{
		return *mText;
	}

	Text& Textbox::getPlaceholder() noexcept
	{
		return *mPlaceholder;
	}

	void Textbox::enableState(State state)
	{
		// Detach the text and placeholder from the currently-active state
		auto text = getState(getActiveState()).detachChild(*mText);
		auto placeholder = getState(getActiveState()).detachChild(*mPlaceholder);

		// Activate the new state requested
		Widget::enableState(state);

		// Attach the previously-detached text and placeholder to the active state
		getState(getActiveState()).attachChild(std::move(text));
		getState(getActiveState()).attachChild(std::move(placeholder));
	}

	void Textbox::updateSelf(const Time& dt)
	{
		if (!mText->getText().empty()) {
			mText->activateFunctionality(Func::EventHandle | Func::Update | Func::Render, Target::Self, true);
			mPlaceholder->activateFunctionality(Func::Render, Target::Self, false);
		}
		Widget::updateSelf(dt);
	}

	void Textbox::handleEventSelf(Event* const event)
	{
		// Check if the toggle button has been disabled
		const State ACTIVE_STATE = getActiveState();
		if (ACTIVE_STATE == State::Disabled) {
			return;
		}

		// Check if the textbox is being hovered over
		if (event->type == Event::Type::MouseMoved && (ACTIVE_STATE == State::Idle || ACTIVE_STATE == State::Hover)) {
			auto mouseMoveEvent = event->as<MouseMoveEvent>();

			// Set the active state to hover or idle based on mouse position
			const bool HOVERED_OVER = isHoveredOver(mouseMoveEvent->position);
			if (HOVERED_OVER && ACTIVE_STATE == State::Idle) {
				enableState(State::Hover);
			}
			else if (!HOVERED_OVER && ACTIVE_STATE == State::Hover) {
				enableState(State::Idle);
			}
		}
		// Check if the textbox is being clicked
		else if (event->type == Event::Type::MouseButtonPressed) {
			auto mouseButtonEvent = event->as<MouseButtonEvent>();
			if (mouseButtonEvent->button == Mouse::Button::Left) {
				if (ACTIVE_STATE == State::Hover && !event->handled) {
					enableState(State::Click);
					event->handled = true;
				}
				else if (!isHoveredOver(Mouse::getPosition())) {
					enableState(State::Idle);
				}
			}
		}

		if (event->type == Event::Type::TextEntered && !event->handled && ACTIVE_STATE == State::Click) {
			auto textEvent = event->as<TextEvent>();
			const std::string& currentText = mText->getText();
			if (getGlobalBounds().max.x > mText->getGlobalBounds().max.x + mText->getAlignmentPadding().x * 2.f) {
				mText->activateFunctionality(Func::EventHandle | Func::Update | Func::Render, Target::Self, true);
				mPlaceholder->activateFunctionality(Func::Render, Target::Self, false);
				mText->setText(currentText + static_cast<char>(textEvent->unicode));
				event->handled = true;
			}
		}

		if (event->type == Event::Type::KeyPressed && !event->handled && ACTIVE_STATE == State::Click) {
			auto keyEvent = event->as<KeyEvent>();
			if (keyEvent->key == Keyboard::Key::Backspace) {
				const std::string& currentText = mText->getText();
				if (!currentText.empty()) {
					mText->setText(currentText.substr(0, currentText.size() - 1));
					if (mText->getText().empty()) {
						mText->activateFunctionality(Func::Render, Target::Self, false);
						mPlaceholder->activateFunctionality(Func::EventHandle | Func::Update | Func::Render, Target::Self, true);
					}
					event->handled = true;
				}
			}
			else if (keyEvent->key == Keyboard::Key::V && keyEvent->control) {
				const std::string& currentText = mText->getText();
				mText->setText(currentText + Clipboard::getString());
				event->handled = true;
			}
		}
	}
}