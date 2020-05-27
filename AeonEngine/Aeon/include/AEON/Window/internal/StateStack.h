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

#ifndef Aeon_Window_StateStack_H_
#define Aeon_Window_StateStack_H_

#include <map>
#include <queue>
#include <functional>

#include <AEON/Config.h>
#include <AEON/System/DebugLogger.h>
#include <AEON/Window/State.h>

namespace ae
{
	/*!
	 \brief The singleton class used internally to manage the user-created ae::State instances.
	 \note This class is considered to be internal but may still be used by the API user.
	*/
	class AEON_API StateStack
	{
	public:
		/*!
		 \brief The enumeration indicating the action to apply to a registered state.
		*/
		enum class Action {
			Push,   //!< Push the registered state
			Remove, //!< Remove the pushed state
			Clear   //!< Remove all the pushed states
		};

	public:
		// Public constructor(s)
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		StateStack(const StateStack&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.3.0
		*/
		StateStack(StateStack&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.3.0
		*/
		StateStack& operator=(const StateStack&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.3.0
		*/
		StateStack& operator=(StateStack&&) = delete;
	public:
		// Public method(s)
		/*!
		 \brief Registers a state so that it may be added and/or removed during the application's execution.
		 \details It's strongly advised to create an enumeration of all the different states to better differentiate them.
		 \note The type T provided has to inherit from the ae::State abstract base class.

		 \param[in] stateID The identifier that will be associated to an ae::State (usually an enumeration value)

		 \par Example:
		 \code
		 enum StateID { Game, Options, ... };
		 ...
		 class GameState : public ae::State { ... };
		 ...
		 ae::StateStack& stateStack = ae::StateStack::getInstance();
		 stateStack.registerState<GameState>(StateID::Game);
		 \endcode

		 \since v0.3.0
		*/
		template <class T, typename = std::enable_if_t<std::is_base_of_v<State, T>>>
		void registerState(uint32_t stateID)
		{
			// Check if the identifier provided has already been registered (ignored in Release mode)
			if _CONSTEXPR_IF (AEON_DEBUG) {
				if (mStateFactories.find(stateID) != mStateFactories.end()) {
					AEON_LOG_ERROR("Attempt to overwrite registered state", "The state identifier provided is already in use.\nAborting operation.");
					return;
				}
			}

			// Register the state and associate it with the identifier provided
			mStateFactories[stateID] = []() {
				return std::make_unique<T>();
			};
		}

		/*!
		 \brief Distributes the polled input \a event to the active ae::State instances.

		 \param[in] event A pointer to the polled input ae::Event that was generated

		 \since v0.3.0
		*/
		void handleEvent(Event* const event);
		/*!
		 \brief Sends the command to the active ae::State instances to update their elements.

		 \param[in] dt An ae::Time containing the time difference between this frame and the previous one

		 \since v0.3.0
		*/
		void update(const Time& dt);
		/*!
		 \brief Sends the command to the active ae::State instances to submit their elements to a renderer.

		 \since v0.3.0
		*/
		void draw();

		/*!
		 \brief Creates a previously registered state that was associated with the identifier provided.
		 \note The identifier has to be associated with a previously registered ae::State.

		 \param[in] stateID The identifier associated with the registered state

		 \sa removeState(), clearStates()

		 \since v0.3.0
		*/
		void pushState(uint32_t stateID);
		/*!
		 \brief Remove a previously pushed state that is associated with the identifier provided.
		 \note The identifier has to be associated with a previously pushed ae::State.

		 \param[in] stateID The identifier associated with the pushed state

		 \sa pushState(), clearStates()

		 \since v0.3.0
		*/
		void removeState(uint32_t stateID);
		/*!
		 \brief Removes all previously pushed states.
		 \note If all the states are removed, the application will create an ae::Event::Type::WindowClosed event to inform the API user to .

		 \sa pushState(), removeState()
		 
		 \since v0.3.0
		*/
		void clearStates();
		/*!
		 \brief Retrieves the pointer to the previously pushed state associated with the identifier provided.
		 \note The identifier has to be associated with a previously pushed ae::State.

		 \param[in] stateID The identifier associated with the pushed state

		 \return The pointer to the previously pushed ae::State associated to the identifier provided or nullptr if it doesn't exist

		 \since v0.3.0
		*/
		_NODISCARD State* const getState(uint32_t stateID) const;

		/*!
		 \brief Checks if the ae::StateStack doesn't possess any pushed ae::State instances.
		 \details This method is used internally to terminate the application when there are no pushed states.

		 \return True if there aren't any pushed states, false otherwise

		 \since v0.3.0
		*/
		_NODISCARD bool isEmpty() const noexcept;

		// Public static method(s)
		/*!
		 \brief Retrieves the single instance of the ae::StateStack.
		 \details The ae::StateStack will be instatiated for the first time by calling this static method.

		 \return The single instance of the ae::StateStack

		 \par Example:
		 \code
		 ae::StateStack& singleInstance = ae::StateStack::getInstance();
		 \endcode

		 \since v0.3.0
		*/
		_NODISCARD static StateStack& getInstance() noexcept;
	private:
		// Private constructor(s)
		/*!
		 \brief Default constructor.
		 \details Initializes the state factory, the list of states and the queue of pending actions to apply to the active states.

		 \since v0.3.0
		*/
		StateStack() noexcept;
	private:
		// Private method(s)
		/*!
		 \brief Creates and retrieves the previously registered ae::State associated to the identifier provided.

		 \param[in] stateID The identifier associated with the registered state

		 \return The ae::State constructed from the factory

		 \since v0.3.0
		*/
		_NODISCARD std::unique_ptr<State> createState(uint32_t stateID) const;
		/*!
		 \brief Applies the pending actions to apply to the pushed ae::State instances.

		 \since v0.3.0
		*/
		void applyPendingChanges();

	private:
		/*!
		 \brief The struct representing a pending action to apply to a pushed ae::State instance.
		 \details These actions are stored internally and applied at the same time one after the other.
		*/
		struct PendingChange
		{
			// Member(s)
			Action   action;  //!< The action to apply to the state instance
			uint32_t stateID; //!< The identifier to the state instance to which the action will be applied

			// Constructor(s)
			/*!
			 \brief Constructs the ae::StateStack::PendingChange by providing the \a action to apply and the identifier to the state instance.

			 \param[in] action The ae::StateStack::Action that will be applied to the ae::State associated to the identifier
			 \param[in] stateID The identifier associated with state instance to which the \a action will be applied, 0 by default

			 \since v0.3.0
			*/
			explicit PendingChange(Action action, uint32_t stateID = 0);
		};

	private:
		// Private member(s)
		std::map<uint32_t, std::unique_ptr<State>>                  mStates;         //!< The pushed states
		std::map<uint32_t, std::function<std::unique_ptr<State>()>> mStateFactories; //!< The factories containing the creation of the registered states
		std::queue<PendingChange>                                   mPendingQueue;   //!< The queue of actions to apply to the pushed states
	};
}
#endif // Aeon_Window_StateStack_H_

/*!
 \class ae::StateStack
 \ingroup window

 The ae::StateStack singleton class is used internally to manage the
 user-created ae::State instances. It's utilized by the ae::Application
 instance to pass on polled input events, to update the states and to send
 them the command to render their respective elements.

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.28
 \copyright MIT License
*/