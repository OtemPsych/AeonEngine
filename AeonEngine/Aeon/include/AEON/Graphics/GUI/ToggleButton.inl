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

namespace ae
{
	// Public constructor(s)
	template <typename T>
	ToggleButton<T>::ToggleButton()
		: Widget<T>()
		, mText(nullptr)
	{
		// Attach a text instance to the active state
		auto text = std::make_unique<Text>();
		mText = text.get();
		Actor::attachChild(std::move(text));

		// Set the text's origin to its center and align it relative to the toggle button's center
		Transform2DComponent* const textTransform = mText->getComponent<Transform2DComponent>();
		textTransform->setOriginFlags(Transform2DComponent::OriginType::All, Transform2DComponent::OriginFlag::Center);
		textTransform->setRelativeAlignment(Transform2DComponent::OriginFlag::Center);
	}

	template <typename T>
	ToggleButton<T>::ToggleButton(ToggleButton<T>&& rvalue) noexcept
		: Widget<T>(std::move(rvalue))
		, mText(rvalue.mText)
	{
	}

	// Public operator(s)
	template <typename T>
	ToggleButton<T>& ToggleButton<T>::operator=(ToggleButton<T>&& rvalue) noexcept
	{
		// Copy the rvalue's trivial data and move the rest
		Widget<T>::operator=(std::move(rvalue));
		mText = rvalue.mText;

		return *this;
	}

	// Public method(s)
	template <typename T>
	void ToggleButton<T>::toggle(bool flag) noexcept
	{
		const Widget<T>::State ACTIVE_STATE = Widget<T>::getActiveState();
		if (ACTIVE_STATE == Widget<T>::State::Click && !flag) {
			Widget<T>::enableState(Widget<T>::isHoveredOver(Mouse::getPosition()) ? Widget<T>::State::Hover : Widget<T>::State::Idle);
		}
		else if (ACTIVE_STATE != Widget<T>::State::Click && flag) {			
			Widget<T>::enableState(Widget<T>::State::Click);
		}
	}

	template <typename T>
	Text& ToggleButton<T>::getText() const noexcept
	{
		return *mText;
	}

	// Private virtual method(s)
	template <typename T>
	void ToggleButton<T>::handleEventSelf(Event* const event)
	{
		// Check if the button has been disabled
		const Widget<T>::State ACTIVE_STATE = Widget<T>::getActiveState();
		if (ACTIVE_STATE == Widget<T>::State::Disabled) {
			return;
		}

		// Check if the toggle button is being hovered over
		if (event->type == Event::Type::MouseMoved && (ACTIVE_STATE == Widget<T>::State::Idle || ACTIVE_STATE == Widget<T>::State::Hover)) {
			auto mouseMoveEvent = event->as<MouseMoveEvent>();

			// Set the active state to hover or idle based on mouse position
			const bool HOVERED_OVER = Widget<T>::isHoveredOver(mouseMoveEvent->position);
			if (!event->handled && HOVERED_OVER) {
				if (ACTIVE_STATE == Widget<T>::State::Idle) {
					Widget<T>::enableState(Widget<T>::State::Hover);
				}
				event->handled = true;
			}
			else if (!HOVERED_OVER && ACTIVE_STATE == Widget<T>::State::Hover) {
				Widget<T>::enableState(Widget<T>::State::Idle);
			}
		}
		// Check if the toggle button is being clicked
		else if (event->type == Event::Type::MouseButtonPressed && !event->handled && (ACTIVE_STATE == Widget<T>::State::Hover || ACTIVE_STATE == Widget<T>::State::Click)) {
			auto mouseButtonEvent = event->as<MouseButtonEvent>();
			if (mouseButtonEvent->button == Mouse::Button::Left && Widget<T>::isHoveredOver(Mouse::getPosition())) {
				if (ACTIVE_STATE == Widget<T>::State::Hover) {
					Widget<T>::enableState(Widget<T>::State::Click);
					event->handled = true;
				}
				else {
					Widget<T>::enableState(Widget<T>::State::Hover);
					event->handled = true;
				}
			}
		}
	}
}