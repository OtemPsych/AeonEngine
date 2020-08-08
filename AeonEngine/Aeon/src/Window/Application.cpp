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

#include <AEON/Window/Application.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <AEON/System/Clock.h>
#include <AEON/Window/internal/EventQueue.h>
#include <AEON/Window/MonitorManager.h>
#include <AEON/Graphics/GLResourceFactory.h>

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

		// Log an informational message indicating GLEW's version (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			AEON_LOG_INFO("GLEW Version", "Using GLEW " + std::string(reinterpret_cast<const char*>(glewGetString(GLEW_VERSION))));
		}

		// Set the window as the active target
		mWindow->activate();
	}

	void Application::pushState(uint32_t stateID)
	{
		mStateStack.pushState(stateID);
	}

	void Application::run()
	{
		// The variables needed to count the frames per second
		const Time ONE_SECOND = Time::seconds(1.0);
		int recordedFps = 0;
		Time timeCounter;

		// Start the main clock
		Time timeSinceLastUpdate;
		Time timeElapsed;
		Clock clock;

		// The application's game loop
		while (mWindow->isOpen())
		{
			processEvents();

			// Retrieve the time elapsed and restart the clock
			timeElapsed = clock.restart();

			// Update until the fixed time interval is reached
			timeSinceLastUpdate += timeElapsed;
			while (timeSinceLastUpdate > mTimeStep) {
				timeSinceLastUpdate -= mTimeStep;
				update(mTimeStep);
			}
			render();

			// FPS Counter
			if ((timeCounter += timeElapsed) >= ONE_SECOND) {
				timeCounter = Time::Zero;
				mCurrentFPS = recordedFps;
				recordedFps = 0;
			}
			else {
				++recordedFps;
			}
		}
	}

	void Application::setFixedTimeStep(int timeStep)
	{
		mTimeStep = Time::seconds(1.0 / static_cast<double>(timeStep));
	}

	int Application::getFPS() const noexcept
	{
		return mCurrentFPS;
	}

	Window& Application::getWindow() noexcept
	{
		return *mWindow;
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
		, mCurrentFPS(0)
		, mTimeStep(Time::seconds(1.0 / 60.0))
	{
		// Initialize GLFW
		init();
	}

	// Private method(s)
	void Application::init() const
	{
		if (!glfwInit()) {
			AEON_LOG_ERROR("Initialization of GLFW failed", "Failed to initialize the GLFW library.");
			return;
		}

		// Log an information message indicating that GLFW initialization succeeded (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			AEON_LOG_INFO("Initialization of GLFW succeeded", "Succeeded in initializing the GLFW library.");
		}
	}

	void Application::processEvents()
	{
		// Poll every input event that has been generated thus far
		while (mEventQueue.pollEvent(mPolledEvent))
		{
			// Automatically handle certain events
			if (mPolledEvent->type == Event::Type::MonitorConnected || mPolledEvent->type == Event::Type::MonitorDisconnected) {
				MonitorEvent* const monitorEvent = mPolledEvent->as<MonitorEvent>();
				MonitorManager::getInstance().update(monitorEvent);
				monitorEvent->handled = true;
			}
			else if (mPolledEvent->type == Event::Type::WindowClosed) {
				GLResourceFactory::getInstance().destroy();
			}

			// Send the event to the window and to the user-created states
			mWindow->handleEvent(mPolledEvent.get());
			mStateStack.handleEvent(mPolledEvent.get());
		}
	}

	void Application::update(const Time& dt)
	{
		mStateStack.update(dt);
	}

	void Application::render()
	{
		//mWindow->clear();
		mStateStack.draw();
		mWindow->display();
	}
}