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

#include <AEON/Graphics/GUI/Textbox.h>

#include <AEON/System/Clipboard.h>
#include <AEON/Window/Cursor.h>
#include <AEON/Graphics/Text.h>
#include <AEON/Graphics/Sprite.h>
#include <AEON/Graphics/BasicRenderer2D.h>

namespace ae
{
	// Initalize static member(s)
	bool Textbox::mEnableIBeamCursor = false;
	std::pair<bool, bool> Textbox::mShowIBeamCursor(false, false);

	// Public constructor(s)
	Textbox::Textbox()
		: Widget()
		, mSelectedRegion(nullptr)
		, mSelectingText(false)
		, mSelectionEndIndex(std::numeric_limits<size_t>().max())
		, mText(nullptr)
		, mPlaceholder(nullptr)
		, mCaret(std::make_pair(nullptr, 0))
		, mCaretFadeTime(Time::seconds(0.5))
		, mCharacterCount(64)
	{		
		// Attach a selection region instance
		auto selectedRegion = std::make_unique<RectangleShape>(Vector2f(0.f, 20.f));
		selectedRegion->setFillColor(ae::Color(61, 113, 186));
		mSelectedRegion = selectedRegion.get();
		attachChild(std::move(selectedRegion));

		Transform2DComponent* const selectedRegionTransform = mSelectedRegion->getComponent<Transform2DComponent>();
		selectedRegionTransform->setOriginFlags(Transform2DComponent::OriginType::All, Transform2DComponent::OriginFlag::Left | Transform2DComponent::OriginFlag::CenterY);
		selectedRegionTransform->setRelativeAlignment(Transform2DComponent::OriginFlag::Left | Transform2DComponent::OriginFlag::CenterY);

		// Attach a text instance
		auto text = std::make_unique<Text>();
		text->activateFunctionality(Func::AllFunc, Target::Children, false);
		text->activateFunctionality(Func::EventHandle | Func::Render, Target::Self, false);
		mText = text.get();
		attachChild(std::move(text));

		Transform2DComponent* const textTransform = mText->getComponent<Transform2DComponent>();
		textTransform->setOriginFlags(Transform2DComponent::OriginType::Translation, Transform2DComponent::OriginFlag::Left | Transform2DComponent::OriginFlag::CenterY);
		textTransform->setRelativeAlignment(Transform2DComponent::OriginFlag::Left | Transform2DComponent::OriginFlag::CenterY);
		textTransform->move(2.f, 0.f);

		// Attach a placeholder instance to the active state
		auto placeholder = std::make_unique<Text>();
		placeholder->activateFunctionality(Func::AllFunc, Target::Children, false);
		placeholder->activateFunctionality(Func::EventHandle, Target::Self, false);
		mPlaceholder = placeholder.get();
		attachChild(std::move(placeholder));

		Transform2DComponent* const placeholderTransform = mPlaceholder->getComponent<Transform2DComponent>();
		placeholderTransform->setOriginFlags(Transform2DComponent::OriginType::Translation, Transform2DComponent::OriginFlag::Left | Transform2DComponent::OriginFlag::CenterY);
		placeholderTransform->setRelativeAlignment(Transform2DComponent::OriginFlag::Left | Transform2DComponent::OriginFlag::CenterY);
		placeholderTransform->move(2.f, 0.f);

		// Attach a caret instance
		auto caret = std::make_unique<ae::RectangleShape>(ae::Vector2f(2.f, 20.f));
		caret->activateFunctionality(Func::AllFunc, Target::Children, false);
		caret->activateFunctionality(Func::EventHandle | Func::Render, Target::Self, false);
		mCaret.first = caret.get();
		attachChild(std::move(caret));

		Transform2DComponent* const caretTransform = mCaret.first->getComponent<Transform2DComponent>();
		caretTransform->setOriginFlags(Transform2DComponent::OriginType::Translation, Transform2DComponent::OriginFlag::Left | Transform2DComponent::OriginFlag::CenterY);
	}

	// Public method(s)
	void Textbox::focus()
	{
		enableState(State::Click);
	}

	// Public static method(s)
	void Textbox::enableIBeamCursor(bool flag)
	{
		mEnableIBeamCursor = flag;
	}

	// Private method(s)
	bool Textbox::addCharacters(const std::string& str)
	{
		const std::string& currentText = mText->getText();
		if (currentText.size() + str.size() <= mCharacterCount) {
			// Activate text, disable placeholder
			if (currentText.empty()) {
				mText->activateFunctionality(Func::Render, Target::Self, true);
				mPlaceholder->activateFunctionality(Func::Render, Target::Self, false);
			}

			// Add characters at caret index
			std::string textToCaret = (mCaret.second == 0) ? "" : currentText.substr(0, mCaret.second);
			std::string caretToText = (mCaret.second == currentText.size()) ? "" : currentText.substr(mCaret.second);
			mText->setText(textToCaret + str + caretToText);
			mCaret.second += str.size();
			updateCaretPosition();
			
			return true;
		}

		return false;
	}

	bool Textbox::removeCharacters()
	{
		const std::string& currentText = mText->getText();
		bool charactersRemoved = false;

		// Remove selected characters
		if (mSelectionEndIndex != std::numeric_limits<size_t>().max()) {
			// Remove forward
			if (mCaret.second < mSelectionEndIndex) {
				std::string textToCaret = (mCaret.second == 0) ? "" : currentText.substr(0, mCaret.second);
				std::string textFromEndIndex = (mSelectionEndIndex == currentText.size()) ? "" : currentText.substr(mSelectionEndIndex);
				mText->setText(textToCaret + textFromEndIndex);
				charactersRemoved = true;
				mCaret.second = mText->getText().size();
				updateCaretPosition();
			}
			// Remove backwards
			else {
				std::string textToEndIndex = (mSelectionEndIndex == 0) ? "" : currentText.substr(0, mSelectionEndIndex);
				std::string textFromCaret = (mCaret.second == currentText.size()) ? "" : currentText.substr(mCaret.second);
				mText->setText(textToEndIndex + textFromCaret);
				charactersRemoved = true;
				mCaret.second = mText->getText().size();
				updateCaretPosition();
			}
		}
		mSelectionEndIndex = std::numeric_limits<size_t>().max();
		mSelectedRegion->setSize(0.f, 0.f);
		mSelectingText = false;

		// Activate placeholder and disable text if empty
		if (mText->getText().empty()) {
			mText->activateFunctionality(Func::Render, Target::Self, false);
			mPlaceholder->activateFunctionality(Func::Render, Target::Self, true);
		}


		return charactersRemoved;
	}

	bool Textbox::removeCharacter(bool backwards)
	{
		const std::string& currentText = mText->getText();
		bool characterRemoved = false;

		// Remove previous character (Backspace)
		if (backwards && mCaret.second != 0) {
			std::string textToCaret = (mCaret.second == 1) ? "" : currentText.substr(0, mCaret.second - 1);
			std::string caretToText = (mCaret.second == currentText.size()) ? "" : currentText.substr(mCaret.second);
			mText->setText(textToCaret + caretToText);
			--mCaret.second;
			updateCaretPosition();

			characterRemoved = true;
		}
		// Remove next character (Delete)
		else if (!backwards && mCaret.second != currentText.size()) {
			std::string textToCaret = (mCaret.second == 0) ? "" : currentText.substr(0, mCaret.second);
			std::string caretToText = (mCaret.second == currentText.size() - 1) ? "" : currentText.substr(mCaret.second + 1);
			mText->setText(textToCaret + caretToText);

			characterRemoved = true;
		}

		// Activate placeholder and disable text if empty
		if (mText->getText().empty()) {
			mText->activateFunctionality(Func::Render, Target::Self, false);
			mPlaceholder->activateFunctionality(Func::Render, Target::Self, true);
		}

		return characterRemoved;
	}

	size_t Textbox::findClosestCharacterIndex()
	{
		const std::string& currentText = mText->getText();
		size_t closestIndex = 0;

		if (!currentText.empty()) {
			const Vector2f MOUSE_COORDS = Application::getInstance().getWindow().mapPixelToCoords(Mouse::getPosition());
			const Matrix4f GLOBAL_TRANSFORM = mText->getGlobalTransform();

			// Closest position of characters
			float closestPosition = std::abs((GLOBAL_TRANSFORM * Vector4f(Vector3f(mText->findCharPos(0)), 1.f)).x - MOUSE_COORDS.x);
			for (size_t i = 1; i < currentText.size(); ++i) {
				float newPos = std::abs((GLOBAL_TRANSFORM * Vector4f(Vector3f(mText->findCharPos(i)), 1.f)).x - MOUSE_COORDS.x);
				if (closestPosition > newPos) {
					closestPosition = newPos;
					closestIndex = i;
				}
			}
			// Closest position of characters and entire string
			float newPos = std::abs(mText->getComponent<ae::Collider2DComponent>()->getGlobalBounds().max.x - MOUSE_COORDS.x);
			if (closestPosition > newPos) {
				closestIndex = currentText.size();
			}
		}

		return closestIndex;
	}

	void Textbox::updateCaretPosition()
	{
		const Vector2f& size = getState(Textbox::State::Click).getSize();
		ae::Transform2DComponent* const caretTransform = mCaret.first->getComponent<ae::Transform2DComponent>();
		Vector2f newPos(mCaret.first->getSize().x, size.y / 2.f);

		if (mCaret.second != 0) {
			mText->update(ae::Time::Zero);

			// Last character
			if (mCaret.second == mText->getText().size()) {
				const float BEFORE_LAST_CHAR_POS = (mCaret.second > 0) ? mText->findCharPos(mCaret.second - 1).x : 0.f;
				const float MODEL_BOUNDS_POS = mText->getComponent<ae::Collider2DComponent>()->getModelBounds().max.x;
				newPos.x += Math::max(BEFORE_LAST_CHAR_POS, MODEL_BOUNDS_POS);
			}
			// First to before last character
			else {
				newPos.x += mText->findCharPos(mCaret.second).x;
			}
		}

		caretTransform->setPosition(newPos);
		updateCaretFade(true);
	}

	void Textbox::updateSelectedRegion()
	{
		if (!mText->getText().empty()) {
			const float START_POS = mCaret.first->getComponent<Transform2DComponent>()->getPosition().x;

			float endPos = 0.f;
			// Last character
			if (mSelectionEndIndex == mText->getText().size()) {
				const float BEFORE_LAST_CHAR_POS = (mSelectionEndIndex > 0) ? mText->findCharPos(mSelectionEndIndex - 1).x : 0.f;
				const float MODEL_BOUNDS_POS = mText->getComponent<ae::Collider2DComponent>()->getModelBounds().max.x;
				endPos = Math::max(BEFORE_LAST_CHAR_POS, MODEL_BOUNDS_POS);
			}
			// First to before last character
			else {
				endPos = mText->findCharPos(mSelectionEndIndex).x;
			}

			mSelectedRegion->setSize(std::abs(endPos - START_POS), mText->getCharacterSize());
			mSelectedRegion->getComponent<Transform2DComponent>()->setPosition(START_POS, 0.f);
			mSelectedRegion->getComponent<Transform2DComponent>()->setScale((endPos < START_POS) ? -1.f : 1.f, 1.f);
		}
	}

	void Textbox::updateSelectedRegionOnMouseMove()
	{
		mSelectionEndIndex = findClosestCharacterIndex();
		updateSelectedRegion();
	}

	void Textbox::updateCaretIndexOnMouseClick()
	{
		mCaret.second = findClosestCharacterIndex();
		updateCaretPosition();

		mSelectionEndIndex = std::numeric_limits<size_t>().max();
		mSelectedRegion->setSize(0.f, 0.f);
	}

	void Textbox::updateCaretFade(bool forceShow)
	{
		const Color& color = mCaret.first->getFillColor();
		if (forceShow) {
			mCaret.first->setFillColor(Color(color.r, color.g, color.b, 255));
			mCaretFadeElapsedTime = mCaretFadeTime;
		}
		else if (mCaretFadeElapsedTime <= Time::Zero) {
			mCaret.first->setFillColor(Color(color.r, color.g, color.b, (color.a == 255) ? 0 : 255));
			mCaretFadeElapsedTime = mCaretFadeTime;
		}
	}

	// Private virtual method(s)
	void Textbox::enableState(uint32_t state)
	{
		// Activate the new state requested
		Widget::enableState(state);

		// Select index based on mouse position
		if (state == State::Click) {
			updateCaretIndexOnMouseClick();
		}
		mCaret.first->activateFunctionality(Func::Render, Target::Self, state == State::Click);
	}

	void Textbox::updateSelf(const Time& dt)
	{
		if (getActiveState() == State::Click) {
			mCaretFadeElapsedTime -= dt;
			updateCaretFade(false);
		}

		if (!mText->getText().empty()) {
			mText->activateFunctionality(Func::Render, Target::Self, true);
			mPlaceholder->activateFunctionality(Func::Render, Target::Self, false);
		}

		// Check which cursor needs to be shown once
		if (mShowIBeamCursor.first) {
			Window& window = Application::getInstance().getWindow();
			window.setCursor((mShowIBeamCursor.second) ? Cursor::Type::IBeam : Cursor::Type::Arrow);
			mShowIBeamCursor.first = false;
			mShowIBeamCursor.second = false;
		}
		Actor::updateSelf(dt);
	}

	void Textbox::handleEventSelf(Event* const event)
	{
		// Check if the toggle button has been disabled
		const State ACTIVE_STATE = getActiveState();
		if (ACTIVE_STATE == State::Disabled) {
			return;
		}

		// Check if the textbox is being hovered over
		if (event->type == Event::Type::MouseMoved) {
			auto mouseMoveEvent = event->as<MouseMoveEvent>();
			const bool HOVERED_OVER = isHoveredOver(mouseMoveEvent->position);

			// Indicate that the I-Beam cursor should be shown
			if (mEnableIBeamCursor) {
				mShowIBeamCursor.first = true;
				if (!event->handled && HOVERED_OVER) {
					mShowIBeamCursor.second = true;
				}
			}

			// Update textbox state
			if (ACTIVE_STATE == State::Idle || ACTIVE_STATE == State::Hover) {
				if (!event->handled && HOVERED_OVER) {
					if (ACTIVE_STATE == State::Idle) {
						enableState(State::Hover);
					}
					event->handled = true;
				}
				else if (!HOVERED_OVER && ACTIVE_STATE == State::Hover) {
					enableState(State::Idle);
				}
			}

			// Update selected region
			if (mSelectingText) {
				updateSelectedRegionOnMouseMove();
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
				else if (ACTIVE_STATE == State::Click && !event->handled) {
					updateCaretIndexOnMouseClick();
					mSelectingText = true;
				}
			}
		}
		// Check if text is no longer being selected
		else if (mSelectingText && event->type == Event::Type::MouseButtonReleased) {
			auto mouseButtonEvent = event->as<MouseButtonEvent>();
			if (mouseButtonEvent->button == Mouse::Button::Left) {
				mSelectingText = false;
			}
		}
		// Check if text is being entered
		else if (event->type == Event::Type::TextEntered && !event->handled && ACTIVE_STATE == State::Click) {
			auto textEvent = event->as<TextEvent>();
			if (textEvent->unicode < 128) {
				std::string str;
				str.push_back(static_cast<char>(textEvent->unicode));
				if (addCharacters(str)) {
					event->handled = true;
				}
			}
		}
		// Check if a key is being pressed
		else if (event->type == Event::Type::KeyPressed && !event->handled && ACTIVE_STATE == State::Click) {
			auto keyEvent = event->as<KeyEvent>();
			if (keyEvent->key == Keyboard::Key::Backspace) {
				if (removeCharacters() || removeCharacter(true)) {
					event->handled = true;
				}
			}
			else if (keyEvent->key == Keyboard::Key::Delete) {
				if (removeCharacters() || removeCharacter(false)) {
					event->handled = true;
				}
			}
			else if (keyEvent->key == Keyboard::Key::V && keyEvent->control) {
				if (addCharacters(Clipboard::getString())) {
					event->handled = true;
				}
			}
			else if (keyEvent->key == Keyboard::Key::Left) {
				if (mCaret.second > 0) {
					--mCaret.second;
					updateCaretPosition();
					if (keyEvent->shift) {
						if (!mSelectingText) {
							mSelectionEndIndex = mCaret.second + 1;
							mSelectingText = true;
						}
						updateSelectedRegion();
					}
					else {
						mSelectionEndIndex = std::numeric_limits<size_t>().max();
						mSelectedRegion->setSize(0.f, 0.f);
						mSelectingText = false;
					}
				}
			}
			else if (keyEvent->key == Keyboard::Key::Right) {
				if (mCaret.second + 1 <= mText->getText().size()) {
					++mCaret.second;
					updateCaretPosition();
					if (keyEvent->shift) {
						if (!mSelectingText) {
							mSelectionEndIndex = mCaret.second - 1;
							mSelectingText = true;
						}
						updateSelectedRegion();
					}
					else {
						mSelectionEndIndex = std::numeric_limits<size_t>().max();
						mSelectedRegion->setSize(0.f, 0.f);
						mSelectingText = false;
					}
				}
			}
			else if (keyEvent->key == Keyboard::Key::Home) {
				mSelectionEndIndex = std::exchange(mCaret.second, 0);
				updateCaretPosition();
				if (keyEvent->shift) {
					updateSelectedRegion();
				}
				else {
					mSelectionEndIndex = std::numeric_limits<size_t>().max();
					mSelectedRegion->setSize(0.f, 0.f);
					mSelectingText = false;
				}
			}
			else if (keyEvent->key == Keyboard::Key::End) {
				mSelectionEndIndex = std::exchange(mCaret.second, mText->getText().size());
				updateCaretPosition();
				if (keyEvent->shift) {
					updateSelectedRegion();
				}
				else {
					mSelectionEndIndex = std::numeric_limits<size_t>().max();
					mSelectedRegion->setSize(0.f, 0.f);
					mSelectingText = false;
				}
			}
			else if (keyEvent->key == Keyboard::Key::A && keyEvent->control) {
				mCaret.second = 0;
				mSelectionEndIndex = mText->getText().size();
				updateCaretPosition();
				updateSelectedRegion();
			}
		}
	}
}