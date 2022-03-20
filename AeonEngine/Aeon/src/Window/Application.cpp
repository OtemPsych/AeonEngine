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

#include <AEON/Window/Application.h>

#include <thread>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <AEON/Window/internal/EventQueue.h>
#include <AEON/Window/MonitorManager.h>
#include <AEON/Graphics/GLResourceFactory.h>
#include <AEON/Graphics/AnimationManager.h>
#include <AEON/Graphics/internal/FontManager.h>

namespace ae
{
	// Public destructor
	Application::~Application()
	{
		glfwTerminate();
	}

	// Public method(s)
	void Application::createWindow(const VideoMode& vidMode, const std::string& title, uint32_t style,
	                               const ContextSettings& settings)
	{
		// Create the window and initialize GLEW
		mWindow = std::make_unique<Window>(vidMode, title, style, settings);
		glewExperimental = GL_TRUE;
		GLenum glewError = glewInit();
		if (glewError != GLEW_OK) {
			AEON_LOG_ERROR("Initialization of GLEW failed", "Error: " + std::string(reinterpret_cast<const char*>(glewGetErrorString(glewError))));
		}

		// Initialize the FreeType library
		FontManager::getInstance();

		// Set the window as the active target
		mWindow->activate();
	}

	void Application::run()
	{
		// The variables needed to count the frames per second
		const Time ONE_SECOND = Time::seconds(1.0);
		int32_t recordedFps = 0;
		Time timeCounter;

		// Start the main clock
		Time prevTime, currTime, timeElapsed;
		Clock clock;

		// The application's game loop
		while (mWindow->isOpen())
		{
			prevTime = currTime;
			currTime = clock.getElapsedTime();
			timeElapsed = currTime - prevTime;

			processEvents();
			update(timeElapsed);
			render();

			// FPS Counter
			if ((timeCounter += timeElapsed) >= ONE_SECOND) {
				timeCounter = Time::Zero;
				mFramerate = recordedFps;
				recordedFps = 0;
			}
			else {
				++recordedFps;
			}

			sleep(timeElapsed);
		}
	}

	void Application::pushState(uint32_t stateID)
	{
		mStateStack.pushState(stateID);
	}

	void Application::setFramerateLimit(int32_t limit) noexcept
	{
		if (mFramerateLimit < limit) {
			mSleepTimeVariance = 0.0;
		}
		mFramerateLimit = limit;
	}

	// Public static method(s)
	Application& Application::getInstance()
	{
		static Application instance;
		return instance;
	}

	// Private constructor(s)
	Application::Application()
		: mWindow(nullptr)
		, mStateStack(StateStack::getInstance())
		, mPolledEvent(nullptr)
		, mEventQueue(EventQueue::getInstance())
		, mFramerate(0)
		, mFramerateLimit(0)
		, mSleepTimeVariance(0.001)
	{
		// Initialize GLFW
		init();
	}

	// Private method(s)
	void Application::processEvents()
	{
		while (mEventQueue.pollEvent(mPolledEvent))	{
			if (mPolledEvent->type == Event::Type::WindowClosed) {
				GLResourceFactory::getInstance().destroy();
			}
			else if (mPolledEvent->type == Event::Type::MonitorConnected || mPolledEvent->type == Event::Type::MonitorDisconnected) {
				MonitorEvent* const monitorEvent = mPolledEvent->as<MonitorEvent>();
				MonitorManager::getInstance().update(monitorEvent);
				monitorEvent->handled = true;
			}

			// Send the event to the window and to the user-created states
			mWindow->handleEvent(mPolledEvent.get());
			mStateStack.handleEvent(mPolledEvent.get());
		}
	}

	void Application::update(const Time& dt)
	{
		AnimationManager::getInstance().deleteFinishedAnimations();
		mStateStack.update(dt);
	}

	void Application::render()
	{
		mStateStack.draw();
		mWindow->display();
	}

	void Application::sleep(const ae::Time& timeElapsed)
	{
		if (mWindow->isVerticalSyncEnabled() || mFramerateLimit <= 0) {
			return;
		}

		const int32_t FPS_DELTA = std::abs(mFramerate - mFramerateLimit);
		if (mFramerate > mFramerateLimit) {
			mSleepTimeVariance += 0.000001 * FPS_DELTA;
		}
		else if (mFramerate < mFramerateLimit) {
			mSleepTimeVariance = Math::max(mSleepTimeVariance - 0.000001 * FPS_DELTA, 0.0);
		}

		if (mSleepTimeVariance > 0.0) {
			std::this_thread::sleep_for(std::chrono::duration<double>(mSleepTimeVariance));
		}
	}

	void Application::init() const
	{
		if (!glfwInit()) {
			AEON_LOG_ERROR("Initialization of GLFW failed", "Failed to initialize the GLFW library.");
			return;
		}
	}
}