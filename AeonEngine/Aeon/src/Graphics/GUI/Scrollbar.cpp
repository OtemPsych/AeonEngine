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

#include <AEON/Graphics/GUI/Scrollbar.h>

namespace ae
{
	// Public constructor(s)
	Scrollbar::Scrollbar(float actualSize)
		: Widget()
		, mActualSize(actualSize)
		, mVisibleSize(actualSize)
	{
		// Set the states' size
		updateSize(actualSize, actualSize);

		setOriginFlags(OriginFlag::Right | OriginFlag::Top);
	}

	// Public method(s)
	void Scrollbar::scrollTo(float pos)
	{
		setPosition(getPosition().x, Math::clamp(pos, 0.f, mActualSize));
	}

	void Scrollbar::scroll(float offset)
	{
		setPosition(getPosition().x, Math::clamp(offset, 0.f, mActualSize));
	}

	void Scrollbar::updateSize(float actualSize, float visibleSize)
	{
		// Set the states' size
		for (size_t i = 0; i < State::StateCount; ++i) {
			RectangleShape& state = getState(static_cast<State>(i));
			state.setSize(state.getSize().x, visibleSize * visibleSize / actualSize);
		}

		mActualSize = actualSize;
		mVisibleSize = visibleSize;
	}

	// Private virtual method(s)
	void Scrollbar::handleEventSelf(Event* const event)
	{
		// Check if the scrollbar has been disabled
		const State ACTIVE_STATE = getActiveState();
		if (ACTIVE_STATE == State::Disabled) {
			return;
		}

		// Check if the scrollbar is being hovered over or moved
		if (event->type == Event::Type::MouseMoved) {
			auto mouseMoveEvent = event->as<MouseMoveEvent>();
			if (ACTIVE_STATE == State::Idle || ACTIVE_STATE == State::Hover) {
				const bool HOVERED_OVER = isHoveredOver(mouseMoveEvent->position);
				if (HOVERED_OVER && ACTIVE_STATE == State::Idle) {
					enableState(State::Hover);
				}
				else if (!HOVERED_OVER && ACTIVE_STATE == State::Hover) {
					enableState(State::Idle);
				}
			}
			else if (ACTIVE_STATE == State::Click) {
				scrollTo(mouseMoveEvent->position.y);
			}
		}
		// Check if the scrollbar is clicked
		else if (event->type == Event::Type::KeyPressed && !event->handled && ACTIVE_STATE == State::Hover) {
			auto mouseButtonEvent = event->as<MouseButtonEvent>();
			if (mouseButtonEvent->button == Mouse::Button::Left) {
				enableState(State::Click);
				event->handled = true;
			}
		}
		// Check if the scrollbar is released
		else if (event->type == Event::Type::KeyReleased && ACTIVE_STATE == State::Click) {
			auto mouseButtonEvent = event->as<MouseButtonEvent>();
			if (mouseButtonEvent->button == Mouse::Button::Left) {
				enableState((isHoveredOver(Mouse::getPosition())) ? State::Hover : State::Idle);
			}
		}
	}
}