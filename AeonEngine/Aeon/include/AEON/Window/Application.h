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

#ifndef Aeon_Window_Application_H_
#define Aeon_Window_Application_H_

#include <AEON/Config.h>
#include <AEON/System/Time.h>
#include <AEON/System/Clock.h>
#include <AEON/Window/Window.h>
#include <AEON/Window/internal/StateStack.h>

namespace ae
{
	// Forward declaration(s)
	class EventQueue;

	/*!
	 \brief The singleton class used to manage the entire application.
	 \details The single instance of this class may be retrieved by calling the static method 'getInstance()'.
	*/
	class AEON_API Application
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Destructor.
		 \details Releases all ressources allocated by GLFW.

		 \since v0.3.0
		*/
		~Application();
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		Application(const Application&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.3.0
		*/
		Application(Application&&) = delete;

		/*!
		 \brief Deleted assignment operator.

		 \since v0.3.0
		*/
		Application& operator=(const Application&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.3.0
		*/
		Application& operator=(Application&&) = delete;

		// Public method(s)
		/*!
		 \brief Creates the ae::Window that will be the ae::Application's active window.

		 \param[in] vidMode The ae::VideoMode containing the properties of the video mode to use
		 \param[in] title The string indicating the name of the window
		 \param[in] style The optional ae::Window::Style flags that will define the window's appearance, resizable and decorated by default
		 \param[in] settings The optional ae::ContextSettings containing the settings of the OpenGL context

		 \par Example:
		 \code
		 ae::Application& app = ae::Application::getInstance();
		 app.createWindow(ae::VideoMode(1280, 720), "My Application");
		 \endcode

		 \since v0.7.0
		*/
		void createWindow(const VideoMode& vidMode, const std::string& title, uint32_t style = Window::Style::Default,
		                  const ContextSettings& settings = ContextSettings());
		/*!
		 \brief Launches the ae::Application's game loop.
		 \details This game loop sends a command to the user's states to update their elements, to handle system events and to render their elements per frame.
		 \note This method should only be called after registering all the states and pushing (activating) at least one of them.

		 \par Example:
		 \code
		 enum StateID { Game, Options, ... };

		 class GameState : public ae::State { ... };

		 ae::Application& app = ae::Application::getInstance();
		 app.createWindow(ae::VideoMode(1280, 720), "My Application");

		 app.registerState<GameState>(StateID::Game);
		 ...

		 app.pushState(StateID::Game);
		 app.run();
		 \endcode

		 \sa registerState(), pushState()

		 \since v0.7.0
		*/
		void run();

		/*!
		 \brief Pushes in (activates) a previously registered state that had been associated with the \a stateID provided.
		 \note A state must only be pushed in if it has previously been registered.

		 \param[in] stateID The identifier associated to a state (usually from an enumeration)

		 \par Example:
		 \code
		 enum StateID { Game, Options, ... };

		 class GameState : public ae::State { ... };

		 ae::Application& app = ae::Application::getInstance();
		 app.createWindow(ae::VideoMode(1280, 720), "My Application");

		 app.registerState<GameState>(StateID::Game);
		 ...

		 app.pushState(StateID::Game);
		 app.run();
		 \endcode

		 \sa registerState(), run()

		 \since v0.7.0
		*/
		void pushState(uint32_t stateID);
		/*!
		 \brief Registers a state so that it may be added and/or removed during the application's execution.
		 \note The parameter T must be a derived class of the ae::State class that will be associated with the identifier provided.\n
		 It's strongly advised to create an enumeration containing all the different states instead of using an integer number.

		 \param[in] stateID The identifier to associate with the state (usually an enumeration value)

		 \par Example:
		 \code
		 enum StateID { Game, Options, ... };

		 class GameState : public ae::State { ... };

		 ae::Application& app = ae::Application::getInstance();
		 app.createWindow(ae::VideoMode(1280, 720), "My Application");

		 app.registerState<GameState>(StateID::Game);
		 \endcode

		 \sa pushState()

		 \since v0.7.0
		*/
		template <class T>
		inline void registerState(uint32_t stateID)
		{
			static_assert(std::is_base_of_v<State, T>, "'registerState' only accepts derived classes of ae::State");
			mStateStack.registerState<T>(stateID);
		}

		/*!
		 \brief Sets a framerate limit for the application (max FPS).
		 \details It may take a few seconds for the desired limit to be reached as the running thread is put to sleep which isn't very reliable.
		 \note Setting a framerate limit while VSync is active may result in a slow framerate.

		 \param[in] limit The desired max framerate

		 \since v0.7.0
		*/
		void setFramerateLimit(int32_t limit) noexcept;
		/*!
		 \brief Retrieves the current frames per second (FPS).

		 \return The last recorded frames per second

		 \since v0.7.0
		*/
		[[nodiscard]] inline int32_t getFramerate() const noexcept { return mFramerate; }
		/*!
		 \brief Retrieves the ae::Application's active window.

		 \return The ae::Window associated to the ae::Application

		 \par Example:
		 \code
		 ae::Application& app = ae::Application::getInstance();
		 ae::Window& appWindow = app.getWindow();
		 \endcode

		 \since v0.7.0
		*/
		[[nodiscard]] inline Window& getWindow() noexcept { return *mWindow; }

		// Public static method(s)
		/*!
		 \brief Retrieves the ae::Application's single instance.
		 \details The ae::Application will be constructed for the first time by calling this static method.

		 \return The ae::Application's single instance.

		 \par Example:
		 \code
		 ae::Application& singleInstance = ae::Application::getInstance();
		 \endcode

		 \since v0.7.0
		*/
		[[nodiscard]] static Application& getInstance();
	private:
		// Private constructor(s)
		/*!
		 \brief Default constructor.
		 \details Disables the framerate limit and retrieves the single instance of the ae::EventQueue and initializes GLFW.

		 \since v0.7.0
		*/
		Application();

		// Private method(s)
		/*!
		 \brief Processes the events generated and distributes them to the API user's states.

		 \since v0.7.0
		*/
		void processEvents();
		/*!
		 \brief Sends the command to the API user's states to update their elements.

		 \param[in] dt The ae::Time containing the time difference between the last frame and the current one

		 \since v0.7.0
		*/
		void update(const Time& dt);
		/*!
		 \brief Sends the command to the API user's states to render their elements and the application prepares for the next frame.

		 \since v0.7.0
		*/
		void render();
		/*!
		 \brief Puts the thread in sleep until the desired framerate is reached.

		 \param[in] timeElapsed The time taken to run the game loop

		 \since v0.7.0
		*/
		void sleep(const ae::Time& timeElapsed);
		/*!
		 \brief Initializes the GLFW library.

		 \since v0.7.0
		*/
		void init() const;

	private:
		// Private member(s)
		std::unique_ptr<Window> mWindow;            //!< The application's active window
		StateStack&             mStateStack;        //!< The manager of the application's user-created states

		std::unique_ptr<Event>  mPolledEvent;       //!< The event that represents the event that's currently being handled
		EventQueue&             mEventQueue;        //!< The queue holding all the unhandled input events

		int32_t                 mFramerate;         //!< The last recorded frames per second
		int32_t                 mFramerateLimit;    //!< The time needed to reach the desired framerate
		double                  mSleepTimeVariance; //!< The dynamically-calculated time needed to reach the desired framerate
	};
}
#endif // Aeon_Window_Application_H_

/*!
 \class ae::Application
 \ingroup window

 The ae::Application singleton class is used to manage the entire application,
 by registering and managing all the states, by creating the window, by
 running the application's game loop and sending commands to the different user
 states to update their elements, polling system events and sending them to the
 user states to handle said events.

 The single instance of this singleton class may be retrieved by calling the
 static method 'getInstance()'.

 Usage example:
 \code
 // StateID.h (user-created header containing the enum for the state IDs)
 enum StateID { Game, Options, ... };

 // GameState.h (user-created header containing the GameState class)
 class GameState : public ae::State
 {
	...
 };

 // main.cpp
 #include <AEON/Window/Application.h>

 #include "StateID.h"
 #include "GameState.h"

 int main()
 {
	ae::Application& app = ae::Application::getInstance();
	app.createWindow(ae::VideoMode(1280, 720), "My Application");

	app.registerLayer<GameState>(StateID::Game);
	...

	app.pushState(StateID::Game);
	app.run();

	return 0;
 }
 \endcode

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.12.31
 \copyright MIT License
*/