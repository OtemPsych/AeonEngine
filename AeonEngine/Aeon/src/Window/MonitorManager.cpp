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

#include <AEON/Window/MonitorManager.h>

#include <functional>

#include <GLFW/glfw3.h>

#include <AEON/Window/Monitor.h>

namespace ae
{
	// Public method(s)
	void MonitorManager::update(MonitorEvent* const monitorEvent)
	{
		// The custom sort function for the list of connected monitors
		auto sortMonitors = [](std::vector<Monitor>& monitors) {
			// Retrieve the updated list of connected monitors
			int monitorCount;
			GLFWmonitor** glfwMonitors = glfwGetMonitors(&monitorCount);

			// Reorder stored list of connected monitors
			int currentMonitorIndex = 0;
			for (int i = 0; i < monitorCount; ++i) {
				if (monitors[i].getHandle() != glfwMonitors[i]) {
					if (monitors[currentMonitorIndex].getHandle() == glfwMonitors[i]) {
						std::swap(monitors[i], monitors[currentMonitorIndex]);
						currentMonitorIndex = std::exchange(i, 0);
					}
				}
			}

			// Update the monitors' properties that are dependent on the monitor setup
			for (Monitor& monitor : monitors) {
				monitor.update();
			}
		};

		// Check if the monitor provided by the event is in the list
		auto found = std::find_if(mMonitors.begin(), mMonitors.end(), [monitorEvent](const Monitor& monitor) {
			return monitorEvent->handle == monitor.getHandle();
		});

		// Proceed based on the event type
		if (monitorEvent->type == Event::Type::MonitorConnected) {
			// Verify that the monitor isn't already in the list of connected monitors
			if (found != mMonitors.end()) {
				AEON_LOG_WARNING("Attempt to insert already connected monitor", "The monitor provided is already in the list of connected monitors.\nAborting operation.");
				return;
			}

			// Add the newly connected monitor and provide the monitor event with the pointer to said monitor
			monitorEvent->monitor = &mMonitors.emplace_back(monitorEvent->handle);

			// Sort the list of connected monitors and make sure that the monitor event still holds the correct pointer after the sort
			sortMonitors(mMonitors);
			if (monitorEvent->monitor->getHandle() != monitorEvent->handle) {
				monitorEvent->monitor = &(*std::find_if(mMonitors.begin(), mMonitors.end(), [monitorEvent](const Monitor& monitor) {
					return monitorEvent->handle == monitor.getHandle();
				}));
			}
		}
		else if (monitorEvent->type == Event::Type::MonitorDisconnected) {
			// Verify that the monitor is in the list of connected monitors
			if (found == mMonitors.end()) {
				AEON_LOG_WARNING("Attempt to remove inexistent monitor", "The monitor to be disconnected wasn't found in the list of connected monitors.\nAborting operation.");
				return;
			}

			// Remove the disconnected monitor
			mMonitors.erase(found);
			mMonitors.shrink_to_fit();

			// Sort the list of connected monitors and nullify the event's pointer to the monitor
			sortMonitors(mMonitors);
			monitorEvent->monitor = nullptr;
		}
	}

	size_t MonitorManager::getMonitorCount() const noexcept
	{
		return mMonitors.size();
	}

	const Monitor* const MonitorManager::getMonitor(size_t index) const
	{
		// Log an error message if the index isn't within the array's limits (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (index >= mMonitors.size()) {
				AEON_LOG_ERROR("Invalid monitor index", "The index provided isn't situated within the array's limits.\nRetrieving primary monitor.");
				return getPrimaryMonitor();
			}
		}

		return &mMonitors[index];
	}

	const Monitor* const MonitorManager::getPrimaryMonitor() const
	{
		// Log an error message if the list of connected monitors is empty
		if (mMonitors.empty()) {
			AEON_LOG_ERROR("No connected monitors found", "The list of connected monitors is empty.\nRetrieving erroneous data.");
			return nullptr;
		}

		return &mMonitors.front();
	}

	// Public static method(s)
	MonitorManager& MonitorManager::getInstance() noexcept
	{
		static MonitorManager instance;
		return instance;
	}

	// Private constructor(s)
	MonitorManager::MonitorManager() noexcept
		: mMonitors()
	{
		// Retrieve the list of connected monitors
		int monitorCount;
		GLFWmonitor** glfwMonitors = glfwGetMonitors(&monitorCount);

		// Store the list of connected monitors
		mMonitors.reserve(monitorCount);
		for (int i = 0; i < monitorCount; ++i) {
			mMonitors.emplace_back(glfwMonitors[i]);
		}
	}
}