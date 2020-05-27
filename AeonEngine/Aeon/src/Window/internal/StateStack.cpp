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

#include <AEON/Window/internal/StateStack.h>
#include <AEON/Window/Window.h>
#include <AEON/Window/Application.h>

namespace ae
{
	// Public method(s)
	void StateStack::handleEvent(Event* const event)
	{
		for (auto& state : mStates) {
			if (!state.second->handleEvent(event)) {
				break;
			}
		}

		applyPendingChanges();
	}

	void StateStack::update(const Time& dt)
	{
		for (auto& state : mStates) {
			if (!state.second->update(dt)) {
				break;
			}
		}

		applyPendingChanges();
	}

	void StateStack::draw()
	{
		for (auto& state : mStates) {
			if (!state.second->draw()) {
				break;
			}
		}
	}

	void StateStack::pushState(uint32_t stateID)
	{
		mPendingQueue.emplace(Action::Push, stateID);
	}

	void StateStack::removeState(uint32_t stateID)
	{
		mPendingQueue.emplace(Action::Remove, stateID);
	}

	void StateStack::clearStates()
	{
		mPendingQueue.emplace(Action::Clear);
	}

	State* const StateStack::getState(uint32_t stateID) const
	{
		auto stateItr = mStates.find(stateID);

		// Log an error message if the state wasn't found (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (stateItr == mStates.end()) {
				AEON_LOG_ERROR("Failed to find specified state", "No match found for the state ID provided.\nAborting operation.");
				return nullptr;
			}
		}

		return stateItr->second.get();
	}

	bool StateStack::isEmpty() const noexcept
	{
		return mStates.empty();
	}

	// Public static method(s)
	StateStack& StateStack::getInstance() noexcept
	{
		static StateStack instance;
		return instance;
	}

	// Private constructor(s)
	StateStack::StateStack() noexcept
		: mStates()
		, mStateFactories()
		, mPendingQueue()
	{
	}

	// Private method(s)
	std::unique_ptr<State> StateStack::createState(uint32_t stateID) const
	{
		auto found = mStateFactories.find(stateID);

		// Log an error message if the state hasn't been registered (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (found == mStateFactories.end()) {
				AEON_LOG_ERROR("Attempt to create unregistered state", "No match found for the state ID provided.\nAborting operation.");
				return nullptr;
			}
		}

		// Execute the constructor stored and return its result
		return found->second();
	}

	void StateStack::applyPendingChanges()
	{
		while (!mPendingQueue.empty()) {
			const PendingChange& change = mPendingQueue.front();
			switch (change.action)
			{
			case Action::Push:
				mStates.try_emplace(change.stateID, createState(change.stateID));
				break;
			case Action::Remove:
				mStates.erase(mStates.find(change.stateID));
				break;
			case Action::Clear:
				mStates.clear();
				Application::getInstance().getWindow().close();
			};

			// Remove the action that was just handled
			mPendingQueue.pop();
		}
	}

	// Private PendingChange
		// Public Constructor(s)
	StateStack::PendingChange::PendingChange(Action action, uint32_t stateID)
		: action(action)
		, stateID(stateID)
	{
	}
}