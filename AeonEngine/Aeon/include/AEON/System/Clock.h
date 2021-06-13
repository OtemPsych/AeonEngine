// MIT License
// 
// Copyright(c) 2019-2021 Filippos Gleglakos
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

#ifndef Aeon_System_Clock_H_
#define Aeon_System_Clock_H_

#include <AEON/System/Time.h>

namespace ae
{
	/*!
	 \brief The utility class used to measure elapsed time.
	*/
	class _NODISCARD AEON_API Clock
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details The ae::Clock starts ticking.

		 \since v0.3.0
		*/
		Clock();
		/*!
		 \brief Copy constructor.

		 \param[in] copy The ae::Clock that will be copied

		 \since v0.6.0
		*/
		Clock(const Clock& copy) = default;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Clock that will be moved

		 \since v0.6.0
		*/
		Clock(Clock&& rvalue) noexcept = default;
	public:
		// Public operator(s)
		/*!
		 \brief Assignment operator.

		 \param[in] other The ae::Clock that will be copied

		 \return The caller ae::Clock

		 \since v0.6.0
		*/
		Clock& operator=(const Clock& other) = default;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Clock that will be moved

		 \return The caller ae::Clock

		 \since v0.6.0
		*/
		Clock& operator=(Clock&& rvalue) noexcept = default;
	public:
		// Public method(s)
		/*!
		 \brief Retrieves the ae::Clock's elapsed time since its last reinitiation or since its construction.

		 \return An ae::Time containing the ae::Clock's elapsed time

		 \par Example:
		 \code
		 ae::Clock clock; // the clock started ticking
		 ...
		 ae::Time elapsedTime = clock.getElapsedTime(); // the clock's elapsed time since its construction
		 \endcode

		 \sa restart()

		 \since v0.3.0
		*/
		_NODISCARD Time getElapsedTime() const;
		/*!
		 \brief Restarts the ae::Clock and retrieves the elapsed time since its last reinitiation or since its construction.

		 \return An ae::Time containing the ae::Clock's elapsed time

		 \par Example:
		 \code
		 ae::Clock clock; // the clock started ticking
		 ...
		 ae::Time elapsedTime = clock.restart(); // the clock's elapsed time since its construction, the clock restarts
		 \endcode

		 \sa getElapsedTime()

		 \since v0.3.0
		*/
		Time restart();

	private:
		// Private member(s)
		Time mStartTime; //!< The time value of clock's last reinitiation in seconds
	};
}
#endif // Aeon_System_Clock_H_

/*!
 \class ae::Clock
 \ingroup system

 The ae::Clock class is a very simple utility class used to measure elapsed time.
 It's used in Aeon's main loop to implement a fixed time-step and can also be
 used by the API user as a timer.

 An ae::Clock object possesses a single member which is its first tick, when the
 clock/timer was initiated. It's possible to retrieve the elapsed time without
 resetting the clock or completely restarting it.

 Usage example:
 \code
 ae::Clock clock; // the timer was initiated
 ...
 // Retrieve the elapsed time without restarting the clock (it keeps ticking)
 ae::Time elapsedTime = clock.getElapsedTime();
 ...
 // Retrieve the elapsed time and restart the clock (the clock was reset)
 ae::Time elapsedTimeReset = clock.restart();
 \endcode

 \author Filippos Gleglakos
 \version v0.6.0
 \date 2020.09.01
 \copyright MIT License
*/