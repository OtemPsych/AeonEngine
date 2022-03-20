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

#include <AEON/Graphics/GUI/IntSlider.h>

namespace ae
{
	// Public constructor(s)
	IntSlider::IntSlider()
		: Widget()
		, mText(nullptr)
		, mValue({ 0, 0, 0 })
		, mSize(0.f)
	{
		init();
	}

	IntSlider::IntSlider(int minValue, int maxValue, int value, float size, int increment)
		: Widget()
		, mText(nullptr)
		, mValue({ minValue, maxValue, value, increment })
		, mSize(size)
	{
		init();
		setValue(value);
	}

	// Public method(s)
	void IntSlider::setLimits(int minValue, int maxValue, float size, int increment)
	{
		mValue.min = minValue;
		mValue.max = maxValue;
		mValue.increment = increment;
		mSize = size;

		setValue(mValue.value);
	}

	void IntSlider::setValue(int value)
	{
		mValue.value = Math::clamp(value, mValue.min, mValue.max);

		const int RANGE = mValue.max - mValue.min;
		const float SLICED_SIZE = mSize / RANGE;

		// Update position and value
		mText->setText(std::to_string(mValue.value));
		Transform2DComponent* const transform = getComponent<Transform2DComponent>();
		transform->setPosition(SLICED_SIZE * (value - mValue.min), transform->getPosition().y);
	}

	// Private method(s)
	void IntSlider::init()
	{
		// Attach a text
		auto text = std::make_unique<Text>();
		mText = text.get();
		attachChild(std::move(text));
	}

	// Private virtual method(s)
	void IntSlider::handleEventSelf(Event* const event)
	{
		// Check if the slider has been disabled
		const State ACTIVE_STATE = getActiveState();
		if (ACTIVE_STATE == State::Disabled) {
			return;
		}

		// Check if the slider is being hovered over or moved
		if (event->type == Event::Type::MouseMoved) {
			auto mouseMoveEvent = event->as<MouseMoveEvent>();
			if (ACTIVE_STATE == State::Idle || ACTIVE_STATE == State::Hover) {
				const bool HOVERED_OVER = isHoveredOver(mouseMoveEvent->position);
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
			else if (ACTIVE_STATE == State::Click) {
				const int RANGE = mValue.max - mValue.min;
				const float SLICED_SIZE = mSize / RANGE;
				const float MOUSE_POS = mouseMoveEvent->position.x;
				const float BACKGROUND_POS = getParent()->getComponent<ae::Collider2DComponent>()->getGlobalBounds().min.x;

				// Find closest slice
				float closestDistance = std::abs(BACKGROUND_POS - MOUSE_POS);
				int closestSlice = 0;
				for (int i = 1; i <= RANGE; i += mValue.increment) {
					const float NEW_CLOSEST_SLICE = std::abs(BACKGROUND_POS + SLICED_SIZE * i - MOUSE_POS);
					if (NEW_CLOSEST_SLICE < closestDistance) {
						closestDistance = NEW_CLOSEST_SLICE;
						closestSlice = i;
					}
				}

				// Update position and value
				setValue(mValue.min + closestSlice);
			}
		}
		// Check if the slider is clicked
		else if (event->type == Event::Type::MouseButtonPressed && !event->handled && ACTIVE_STATE == State::Hover) {
			auto mouseButtonEvent = event->as<MouseButtonEvent>();
			if (mouseButtonEvent->button == Mouse::Button::Left) {
				enableState(State::Click);
				event->handled = true;
			}
		}
		// Check if the slider is released
		else if (event->type == Event::Type::MouseButtonReleased && ACTIVE_STATE == State::Click) {
			auto mouseButtonEvent = event->as<MouseButtonEvent>();
			if (mouseButtonEvent->button == Mouse::Button::Left) {
				enableState((isHoveredOver(Mouse::getPosition())) ? State::Hover : State::Idle);
			}
		}
	}
}