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

#include <AEON/Graphics/GUI/Scrollbar.h>

namespace ae
{
	// Public constructor(s)
	Scrollbar::Scrollbar(float startPosition)
		: Widget()
		, mScrollArea()
		, mContainerMin(0.f)
		, mContainerMax(1.f)
		, mContentMin(0.f)
		, mContentMax(1.f)
		, mOnScroll()
		, mDuration(ae::Time::seconds(0.15))
		, mElapsed(ae::Time::Zero)
		, mSpeedUpDuration(ae::Time::seconds(0.05))
		, mSpeedUpElapsed(ae::Time::Zero)
		, mStartPosition(startPosition)
		, mEndPosition(startPosition)
		, mCurrentPosition(startPosition)
		, mBaseSpeed(100.f)
		, mSpeed(100.f)
		, mSpeedUp(60.f)
	{
	}

	// Public method(s)
	void Scrollbar::scroll(float wheelOffset)
	{
		scrollTo(mCurrentPosition - wheelOffset * mSpeed);
	}

	void Scrollbar::scrollTo(float position)
	{
		if (mContentMax - mContentMin <= mContainerMax - mContainerMin) {
			if (mCurrentPosition > mContentMin) {
				mEndPosition = mContentMin;
			}
			else {
				return;
			}
		}
		else {
			mEndPosition = ae::Math::clamp(position, mContentMin, mContentMax);
		}

		if (mCurrentPosition != mEndPosition) {
			mSpeed += mSpeedUp;
			mStartPosition = mCurrentPosition;
			mElapsed = ae::Time::Zero;
			mSpeedUpElapsed = ae::Time::Zero;
		}
	}

	void Scrollbar::setContainer(float min, float max) noexcept
	{
		mContainerMin = min;
		mContainerMax = max;

		updateSize();
		updateScroll();
	}

	void Scrollbar::setContent(float min, float max) noexcept
	{
		mContentMin = min;
		mContentMax = max;

		updateSize();
		updateScroll();
	}

	void Scrollbar::setScrollArea(const ae::Box2f& scrollArea) noexcept
	{
		mScrollArea = scrollArea;

		updateSize();
		updateScroll();
	}

	void Scrollbar::setScrollDuration(const ae::Time& duration, const ae::Time& speedUpDuration) noexcept
	{
		mDuration = duration;
		mSpeedUpDuration = speedUpDuration;
	}

	void Scrollbar::setScrollSpeed(float speed, float speedUp) noexcept
	{
		mBaseSpeed = speed;
		mSpeed = speed;
		mSpeedUp = speedUp;
	}

	// Protected virtual method(s)
	void Scrollbar::handleEventSelf(ae::Event* const event)
	{
		// Check if the scrollbar has been disabled
		const State ACTIVE_STATE = getActiveState();
		if (ACTIVE_STATE == State::Disabled) {
			return;
		}

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
				const float SCROLL_HEIGHT = getState(State::Idle).getSize().y;
				const float NEW_POS = ae::Math::clamp(static_cast<float>(mouseMoveEvent->position.y) - SCROLL_HEIGHT / 2.f, mContentMin, mContainerMax);
				const float CONTAINER_HEIGHT = mContainerMax - mContainerMin;
				scrollTo(NEW_POS * ae::Math::max(mContentMax - mContentMin, CONTAINER_HEIGHT) / CONTAINER_HEIGHT);
			}
		}
		else if (event->type == Event::Type::MouseButtonPressed && !event->handled && ACTIVE_STATE == State::Hover) {
			auto mouseButtonEvent = event->as<MouseButtonEvent>();
			if (mouseButtonEvent->button == Mouse::Button::Left) {
				enableState(State::Click);
				event->handled = true;
			}
		}
		else if (event->type == Event::Type::MouseButtonReleased && ACTIVE_STATE == State::Click) {
			auto mouseButtonEvent = event->as<MouseButtonEvent>();
			if (mouseButtonEvent->button == Mouse::Button::Left) {
				enableState(isHoveredOver(ae::Mouse::getPosition()) ? State::Hover : State::Idle);
			}
		}
	}

	void Scrollbar::updateSelf(const ae::Time& dt)
	{
		// Reset speed if no longer scrolling after a certain amount of time
		if ((mSpeedUpElapsed += dt) >= mSpeedUpDuration) {
			mSpeed = mBaseSpeed;
		}

		if (mElapsed != mDuration) {
			mElapsed = ae::Time::seconds(ae::Math::min(mDuration.asSeconds(), (mElapsed + dt).asSeconds()));

			const float FACTOR = static_cast<float>(ae::Math::sin(1.5707963 * (mElapsed / mDuration).asSeconds()));
			mCurrentPosition = ae::Math::lerp(mStartPosition, mEndPosition, FACTOR);

			updateScroll();
			mOnScroll(mCurrentPosition);

			if (mElapsed == mDuration) {
				mStartPosition = mEndPosition;
			}
		}

		Widget::updateSelf(dt);
	}

	// Private method(s)
	void Scrollbar::updateScroll() noexcept
	{
		const float CONTAINER_HEIGHT = mContainerMax - mContainerMin;
		const float CONTENT_HEIGHT = ae::Math::max(mContentMax - mContentMin, CONTAINER_HEIGHT);
		const float SCROLL_HEIGHT = getState(State::Idle).getSize().y;

		const float POS_Y = mScrollArea.min.y + (mScrollArea.max.y - SCROLL_HEIGHT) * (mCurrentPosition / CONTENT_HEIGHT);
		getComponent<ae::Transform2DComponent>()->setPosition(mScrollArea.min.x, POS_Y);
	}

	void Scrollbar::updateSize() noexcept
	{
		const float CONTAINER_HEIGHT = mContainerMax - mContainerMin;
		const float CONTENT_HEIGHT = ae::Math::max(mContentMax - mContentMin, CONTAINER_HEIGHT);
		
		for (uint32_t i = 0; i < State::StateCount; ++i) {
			ae::RectangleShape& state = getState(static_cast<State>(i));
			state.setSize(mScrollArea.max.x, CONTAINER_HEIGHT / CONTENT_HEIGHT * mScrollArea.max.y);
		}
	}
}