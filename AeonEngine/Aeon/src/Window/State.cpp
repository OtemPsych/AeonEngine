#include <AEON/Window/State.h>
#include <AEON/Window/Application.h>
#include <AEON/Window/internal/StateStack.h>

namespace ae
{
	// Public destructor
	State::~State()
	{
	}

	// Public Method(s)
	bool State::handleEvent(Event* const event)
	{
		return true;
	}

	bool State::update(const Time& dt)
	{
		return true;
	}

	bool State::draw()
	{
		return true;
	}

	// Protected constructor(s)
	State::State()
		: mApplication(Application::getInstance())
		, mWindow(mApplication.getWindow())
		, mStack(StateStack::getInstance())
	{
	}

	// Protected method(s)
	void State::requestStatePush(uint32_t stateID)
	{
		mStack.pushState(stateID);
	}

	void State::requestStateRemove(uint32_t stateID)
	{
		mStack.removeState(stateID);
	}

	void State::requestStateClear()
	{
		mStack.clearStates();
	}

	State* const State::requestState(uint32_t stateID)
	{
		return mStack.getState(stateID);
	}
}