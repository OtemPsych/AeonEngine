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
	Button<T>::Button()
		: Widget<T>()
		, mText(nullptr)
	{
		// Attach a text instance
		auto text = std::make_unique<Text>();
		mText = text.get();
		Actor::attachChild(std::move(text));

		// Set the text's origin to its center and align it relative to the button's center
		Transform2DComponent* const textTransform = mText->getComponent<Transform2DComponent>();
		textTransform->setOriginFlags(Transform2DComponent::OriginType::All, Transform2DComponent::OriginFlag::Center);
		textTransform->setRelativeAlignment(Transform2DComponent::OriginFlag::Center);
	}

	template <typename T>
	Button<T>::Button(Button<T>&& rvalue) noexcept
		: Widget<T>(std::move(rvalue))
		, mText(rvalue.mText)
	{
	}

	// Public operator(s)
	template <typename T>
	Button<T>& Button<T>::operator=(Button<T>&& rvalue) noexcept
	{
		// Copy the rvalue's trivial data and move the rest
		Widget<T>::operator=(std::move(rvalue));
		mText = rvalue.mText;

		return *this;
	}

	// Public method(s)
	template <typename T>
	Text& Button<T>::getText() const noexcept
	{
		return *mText;
	}

	// Protected virtual method(s)
	template <typename T>
	void Button<T>::updateSelf(const Time& dt)
	{
		// Set the active state to hover if it's currently clicked
		if (Widget<T>::getActiveState() == Widget<T>::State::Click) {
			Widget<T>::enableState(Widget<T>::State::Hover);
		}
		Actor::updateSelf(dt);
	}

	template <typename T>
	void Button<T>::handleEventSelf(Event* const event)
	{
		// Check if the button has been disabled
		const Widget<T>::State ACTIVE_STATE = Widget<T>::getActiveState();
		if (ACTIVE_STATE == Widget<T>::State::Disabled) {
			return;
		}

		// Check if the button is being hovered over
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
		// Check if the button is being clicked
		else if (event->type == Event::Type::MouseButtonPressed && !event->handled && ACTIVE_STATE == Widget<T>::State::Hover) {
			auto mouseButtonEvent = event->as<MouseButtonEvent>();
			if (mouseButtonEvent->button == Mouse::Button::Left && Widget<T>::isHoveredOver(Mouse::getPosition())) {
				Widget<T>::enableState(Widget<T>::State::Click);
				event->handled = true;
			}
		}
	}
}