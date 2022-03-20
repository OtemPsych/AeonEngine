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

#include <AEON/Graphics/GUI/Dropdown.h>
#include <AEON/Graphics/Text.h>

namespace ae
{
	// Public constructor(s)
	Dropdown::Dropdown()
		: Widget()
		, mOnItemClick()
		, mText(nullptr)
		, mItems()
	{
		// Attach a text instance
		auto text = std::make_unique<Text>();
		mText = text.get();
		attachChild(std::move(text));

		// Set the text's origin to its center and align it to the dropdown's center
		auto textTransformComponent = mText->getComponent<Transform2DComponent>();
		textTransformComponent->setOriginFlags(Transform2DComponent::OriginType::All, Transform2DComponent::OriginFlag::Center);
		textTransformComponent->setRelativeAlignment(Transform2DComponent::OriginFlag::Center);
	}

	// Public method(s)
	void Dropdown::addItem(std::unique_ptr<Button<>> item)
	{
		mItems.push_back(item.get());
		item->activateFunctionality(Func::EventHandle | Func::Render, Target::AllTarget, false);
		attachChild(std::move(item));

		mItems.back()->getComponent<Transform2DComponent>()->setRelativeAlignment(Transform2DComponent::OriginFlag::Left | Transform2DComponent::OriginFlag::Bottom);
		updateItemPositions();
	}

	void Dropdown::updateItemPositions()
	{
		float itemOffsetY = 0.f;
		for (Button<>* item : mItems) {
			item->getComponent<Transform2DComponent>()->setPosition(0.f, itemOffsetY);
			itemOffsetY += item->getState(item->getActiveState()).getSize().y;
		}
	}

	// Private method(s)
	void Dropdown::enableItems(bool flag)
	{
		for (Button<>* item : mItems) {
			item->activateFunctionality(Func::EventHandle | Func::Render, Target::AllTarget, flag);
		}
	}

	// Private virtual method(s)
	void Dropdown::updateSelf(const Time& dt)
	{
		// Check if an item has been clicked
		if (mOnItemClick) {
			for (size_t i = 0; i < mItems.size(); ++i) {
				if (mItems[i]->getActiveState() == State::Click) {
					mText->setText(mItems[i]->getText().getText());
					enableItems(false);
					enableState(State::Idle);
					mOnItemClick(mItems[i], i);
				}
			}
		}

		Widget::updateSelf(dt);
	}

	void Dropdown::handleEventSelf(Event* const event)
	{
		// Check if the dropdown has been disabled
		const State ACTIVE_STATE = getActiveState();
		if (ACTIVE_STATE == State::Disabled) {
			return;
		}

		// Check if the dropdown is being hovered over
		if (event->type == Event::Type::MouseMoved && (ACTIVE_STATE == State::Idle || ACTIVE_STATE == State::Hover)) {
			auto mouseMoveEvent = event->as<MouseMoveEvent>();

			// Set the active state to hover or idle based on mouse position
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
		// Check if the dropdown is being clicked
		else if (event->type == Event::Type::MouseButtonPressed) {
			auto mouseButtonEvent = event->as<MouseButtonEvent>();
			if (mouseButtonEvent->button == Mouse::Button::Left) {
				if (!event->handled && ACTIVE_STATE == State::Hover) {
					enableState(State::Click);
					enableItems(true);
					event->handled = true;
				}
				else if (!event->handled && ACTIVE_STATE == State::Click && isHoveredOver(Mouse::getPosition())) {
					enableState(State::Hover);
					enableItems(false);
					event->handled = true;
				}
				else if (!isHoveredOver(Mouse::getPosition())) {
					enableState(State::Idle);
					enableItems(false);
				}
			}
		}
	}
}