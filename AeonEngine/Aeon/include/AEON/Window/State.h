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

#ifndef Aeon_Window_State_H_
#define Aeon_Window_State_H_

#include <yvals_core.h>

#include <AEON/Config.h>
#include <AEON/System/Time.h>
#include <AEON/Window/Event.h>

namespace ae
{
	// Forward declaration(s)
	class StateStack;
	class Application;
	class Window;

	/*!
	 \brief The abstract base class that acts as a layer used to separate the storage of game elements.
	 \details A state can hold the elements of the main menu, of different levels of the game, etc. It separates the management of the application's layers.
	*/
	class _NODISCARD AEON_API State
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		State(const State&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.3.0
		*/
		State(State&&) = delete;
		/*!
		 \brief Virtual destructor.
		 \details The destructor is virtual as the class can be inherited.

		 \since v0.3.0
		*/
		virtual ~State();
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.3.0
		*/
		State& operator=(const State&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.3.0
		*/
		State& operator=(State&&) = delete;
	public:
		// Public method(s)
		/*!
		 \brief Receives the polled input \a event to be handled.
		 \details Derived classes can override this method to handle the \a event in a specific way.

		 \param[in] event A pointer to the polled input ae::Event that was generated

		 \return True if the other ae::State instances should be allowed to handle this \a event as well, false otherwise

		 \since v0.3.0
		*/
		_NODISCARD virtual bool handleEvent(Event* const event);
		/*!
		 \brief Updates the elements that belong to the ae::State.
		 \details Derived classes can override this method to update the user-created game elements.

		 \param[in] dt An ae::Time containing the time difference between this frame and the previous one

		 \return True if the other ae::State instances should be allowed to update their elements, false otherwise

		 \since v0.3.0
		*/
		_NODISCARD virtual bool update(const Time& dt);
		/*!
		 \brief Submits the elements that belong to the ae::State to the appropriate renderer.
		 \details Derived classes can override this method to submit the user-created game elements to a renderer.

		 \return True if the other ae::State instances should be allowed to submit their elements to a renderer, false otherwise

		 \since v0.3.0
		*/
		_NODISCARD virtual bool draw();
	protected:
		// Protected constructor(s)
		/*!
		 \brief Default constructor.
		 \details Retrieves the ae::StateStack's single instance which manages the states, the application instance and the active window instance.

		 \since v0.3.0
		*/
		State();
	protected:
		// Protected method(s)
		/*!
		 \brief Sends a request to the ae::StateStack instance to activate the state associated with the identifier provided.
		 \details The state associated with the identifier will be placed on top of the caller ae::State.
		 \note The identifier has to be associated with a previously registered ae::State.

		 \param[in] stateID The identifier associated with the registered state

		 \sa requestStateRemove(), requestStateClear()

		 \since v0.3.0
		*/
		void requestStatePush(uint32_t stateID);
		/*!
		 \brief Sends a request to the ae::StateStack instance to remove (deactivate) the state associated with the identifier provided.
		 \note The identifier has to be associated with a previously registered ae::State.

		 \param[in] stateID The identifier associated with the registered state

		 \sa requestStatePush(), requestStateClear()

		 \since v0.3.0
		*/
		void requestStateRemove(uint32_t stateID);
		/*!
		 \brief Sends a request to the ae::StateStack instance to remove all of the active states.
		 \note If all the states are removed, the application will automatically terminate.

		 \sa requestStatePush(), requestStateRemove()

		 \since v0.3.0
		*/
		void requestStateClear();
		/*!
		 \brief Sends a request to the ae::Stack instance to provide the pointer to the state associated with the identifier provided.
		 \note The identifier has to be associated with a previously registered ae::State.

		 \param[in] stateID The identifier associated with the registered state

		 \return The pointer to the previously registered ae::State associated to the identifier provided or nullptr if it doesn't exist

		 \par Example:
		 \code
		 // Retrieve the pointer to the state's base class
		 ae::State* const state = requestState(StateID::Game);

		 // Cast the ae::State to the appropriate derived class
		 GameState* const gameState = dynamic_cast<GameState* const>(state);
		 \endcode

		 \since v0.3.0
		*/
		_NODISCARD State* const requestState(uint32_t stateID);

	protected:
		// Protected member(s)
		Application& mApplication; //!< The single instance of the application
		Window&      mWindow;      //!< The active window of the application
	private:
		// Private member(s)
		StateStack& mStack; //!< The single instance of the stack managing all the state instances
	};
}
#endif // Aeon_Window_State_H_

/*!
 \class ae::State
 \ingroup window

 The ae::State abstract base class acts as a layer that is used to separate
 storage of game elements and ease management of game levels (main menu,
 options menu, game - level 1, game - level 2, etc.).

 The API user is forced to create at least one class that inherits the
 ae::State ABC so that Aeon can manage it, provide with polled input events,
 update it, etc.

 Usage example:
 \code
 #include <AEON/Window/State.h>

 class GameLayer : public ae::State
 {
 public:
	GameLayer();
 public:
	virtual bool handleEvent(Event* event) override final;
	virtual bool update(const ae::Time& dt) override final;
	virtual void draw() override final;
 };
 \endcode

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.28
 \copyright MIT License
*/