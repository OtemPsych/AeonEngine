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

#pragma once

#include <queue>
#include <memory>

#include <AEON/Config.h>

namespace ae
{
	// Forward declaration(s)
	class Event;

	/*!
	 \brief The singleton class representing the queue of system events generated that will be retrieved automatically by Aeon.
	 \note This class is considered to be internal but may still be used by the API user.
	*/
	class AEON_API EventQueue
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		EventQueue(const EventQueue&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.3.0
		*/
		EventQueue(EventQueue&&) = delete;

		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.3.0
		*/
		EventQueue& operator=(const EventQueue&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.3.0
		*/
		EventQueue& operator=(EventQueue&&) = delete;

		// Public method(s)
		/*!
		 \brief Enqueues a new \a event at the end of the queue to be processed.

		 \param[in] event The ae::Event to enqueue

		 \par Example:
		 \code
		 auto event = std::make_unique<ae::WindowResizeEvent>(640, 480);
		 ae::EventQueue::getInstance().enqueueEvent(std::move(event));
		 \endcode

		 \sa pollEvent()

		 \since v0.3.0
		*/
		void enqueueEvent(std::unique_ptr<Event> event);
		/*!
		 \brief Assigns the ae::Event at the front of the queue to the \a event parameter provided and removes it from the queue.

		 \param[in] event The ae::Event that will be assigned to the event at the front of the queue

		 \return True if there are still events stored in the queue, false otherwise

		 \par Example:
		 \code
		 std::unique_ptr<ae::Event> event;
		 while (ae::EventQueue::getInstance().pollEvent(event)) {
			if (event->type == ae::Event::Type::KeyPressed) {
				auto keyEvent = event->as<ae::KeyEvent>();
				if (keyEvent->key == ae::Keyboard::Key::Escape) {
					...
				}
			}
		 }
		 \endcode

		 \sa enqueueEvent()

		 \since v0.7.0
		*/
		[[nodiscard]] bool pollEvent(std::unique_ptr<Event>& event);

		// Public static method(s)
		/*!
		 \brief Retrieves the single instance of the ae::EventQueue.
		 \details The ae::EventQueue will be instantiated for the first time by calling this static method.

		 \return The single instance of the ae::EventQueue

		 \par Example:
		 \code
		 ae::EventQueue& eventQueue = ae::EventQueue::getInstance();
		 \endcode

		 \since v0.7.0
		*/
		[[nodiscard]] static EventQueue& getInstance() noexcept;
	private:
		// Private constructor(s)
		/*!
		 \brief Default constructor.
		 \details Initializes the queue that'll store the events generated.

		 \since v0.3.0
		*/
		EventQueue() noexcept;

		// Private member(s)
		std::queue<std::unique_ptr<Event>> mQueue; //!< The queue that stores the unpolled events
	};
}

/*!
 \class ae::EventQueue
 \ingroup window

 The ae::EventQueue class is considered an internal singleton class meaning
 that the API user doesn't need to be concerned with it. It's used internally
 to distribute all the generated events to the API user's states.

 It stores the generated events in a queue wherein Aeon retrieves each event
 one at a time and sends them out to the user-created ae::State instances.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.12.20
 \copyright MIT License
*/