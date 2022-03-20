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

#ifndef Aeon_Window_MonitorManager_H_
#define Aeon_Window_MonitorManager_H_

#include <vector>

#include <AEON/Config.h>
#include <AEON/Window/Monitor.h>
#include <AEON/Window/Event.h>

namespace ae
{
	/*!
	 \brief The singleton class that is responsible for the creation and management of all ae::Monitor instances.
	*/
	class AEON_API MonitorManager
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Deleted copy constructor.

		 \since v0.3.0
		*/
		MonitorManager(const MonitorManager&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.3.0
		*/
		MonitorManager(MonitorManager&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \return The caller ae::MonitorManager that would've been returned

		 \since v0.3.0
		*/
		MonitorManager& operator=(const MonitorManager&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \return The caller ae::MonitorManager that would've been returned

		 \since v0.3.0
		*/
		MonitorManager& operator=(MonitorManager&&) = delete;
	public:
		// Public method(s)
		/*!
		 \brief Updates the list of connected monitors.
		 \details This method is automatically called when a monitor is connected or disconnected.

		 \param[in] monitorEvent The pointer to the ae::MonitorEvent containing the monitor that was (dis)connected

		 \since v0.6.0
		*/
		void update(MonitorEvent* const monitorEvent);
		/*!
		 \brief Retrieves the total amount of connected monitors.
		 
		 \return The number of connected monitors detected

		 \par Example:
		 \code
		 ae::MonitorManager& monitorManager = ae::MonitorManager::getInstance();
		 size_t monitorCount = monitorManager.getMonitorCount();
		 \endcode

		 \since v0.3.0
		*/
		_NODISCARD size_t getMonitorCount() const noexcept;
		/*!
		 \brief Retrieves the ae::Monitor situated at the \a index provided.
		 \details The first monitor at index 0 is always the primary monitor (the preferred monitor) of the user.

		 \param[in] index The location within the list of the ae::Monitor to retrieve

		 \return A pointer to the ae::Monitor situated at the \a index provided or a pointer to the primary monitor if the specified monitor wasn't detected

		 \par Example:
		 \code
		 ae::MonitorManager& monitorManager = ae::MonitorManager::getInstance();
		 const ae::Monitor* const secondMonitor = monitorManager.getMonitor(1);
		 \endcode

		 \sa getPrimaryMonitor(), getMonitorCount()

		 \since v0.3.0
		*/
		_NODISCARD const Monitor* const getMonitor(size_t index) const;
		/*!
		 \brief Retrieves the primary monitor (the preferred monitor) of the user.
		 \details This method is equivalent to calling: getMonitor(0).

		 \return A pointer to the primary ae::Monitor of the application user or nullptr if no monitor was detected

		 \par Example:
		 \code
		 ae::MonitorManager& monitorManager = ae::MonitorManager::getInstance();
		 const ae::Monitor* const primaryMonitor = monitorManager.getPrimaryMonitor();
		 // The above call is equivalent to the following:
		 const ae::Monitor* const primaryMonitor = monitorManager.getMonitor(0);
		 \endcode

		 \sa getMonitor()

		 \since v0.3.0
		*/
		_NODISCARD const Monitor* const getPrimaryMonitor() const;

		// Public static method(s)
		/*!
		 \brief Retrieves the single instance of the ae::MonitorManager.
		 \details The ae::MonitorManager will be instantiated for the first time by calling this static method.

		 \return The single instance of ae::MonitorManager

		 \par Example:
		 \code
		 ae::MonitorManager& monitorManager = ae::MonitorManager::getInstance();
		 \endcode

		 \since v0.3.0
		*/
		_NODISCARD static MonitorManager& getInstance() noexcept;
	private:
		// Private constructor(s)
		/*!
		 \brief Default constructor.
		 \details Retrieves and stores the list of connected monitors.

		 \since v0.3.0
		*/
		MonitorManager() noexcept;
	private:
		// Private method(s)
		/*!
		 \brief Sorts the list of connected monitors and the monitors' properties.
		 \details Called when a monitor is connected or disconnected.

		 \since v0.6.0
		*/
		void sortMonitors();

	private:
		// Private member(s)
		std::vector<Monitor> mMonitors; //!< The list of monitors
	};
}
#endif // Aeon_Window_MonitorManager_H_

/*!
 \class ae::MonitorManager
 \ingroup window

 The ae::MonitorManager singleton class is responsible for the creation and
 management of the ae::Monitor instances which represent the monitor setup of
 the application user.

 \author Filippos Gleglakos
 \version v0.3.0
 \date 2019.07.29
 \copyright MIT License
*/