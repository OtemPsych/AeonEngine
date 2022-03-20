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

#include <AEON/Window/VideoMode.h>

#include <AEON/Window/MonitorManager.h>

namespace ae
{
	// Public constructor(s)
	VideoMode::VideoMode(const Monitor* const monitor)
		: mResolution()
		, mRefreshRate(0)
		, mRedBits(0)
		, mGreenBits(0)
		, mBlueBits(0)
		, mAssociatedMonitor((monitor) ? monitor : MonitorManager::getInstance().getPrimaryMonitor())
	{
		// Log an error if the pointer to the monitor provided is invalid
		if (!mAssociatedMonitor) {
			AEON_LOG_ERROR("Invalid pointer to monitor", "The pointer to the monitor provided isn't valid.\nPossible problem: no monitors detected.");
		}

		// Retrieve the associated monitor's properties
		const VideoMode& desktopMode = mAssociatedMonitor->getDesktopMode();
		mResolution = desktopMode.getResolution();
		mRefreshRate = desktopMode.getRefreshRate();
		mRedBits = desktopMode.getRedBits();
		mGreenBits = desktopMode.getGreenBits();
		mBlueBits = desktopMode.getBlueBits();
	}

	VideoMode::VideoMode(const Vector2i& resolution, int32_t refreshRate, int32_t redBits, int32_t greenBits, int32_t blueBits, const Monitor* const monitor)
		: VideoMode(resolution.x, resolution.y, refreshRate, redBits, greenBits, blueBits, monitor)
	{
	}

	VideoMode::VideoMode(int32_t width, int32_t height, int32_t refreshRate, int32_t redBits, int32_t greenBits, int32_t blueBits, const Monitor* const monitor)
		: mResolution(width, height)
		, mRefreshRate(refreshRate)
		, mRedBits(redBits)
		, mGreenBits(greenBits)
		, mBlueBits(blueBits)
		, mAssociatedMonitor((monitor) ? monitor : MonitorManager::getInstance().getPrimaryMonitor())
	{
		// Log an error if the pointer to the monitor provided is invalid
		if (!mAssociatedMonitor) {
			AEON_LOG_ERROR("Invalid pointer to monitor", "The pointer to the monitor provided isn't valid.\nPossible problem: no monitors detected.");
		}
	}

	VideoMode::VideoMode(const Vector2i& resolution, const Monitor* const monitor)
		: VideoMode(resolution.x, resolution.y, monitor)
	{
	}

	VideoMode::VideoMode(int32_t width, int32_t height, const Monitor* const monitor)
		: mResolution(width, height)
		, mRefreshRate()
		, mRedBits()
		, mGreenBits()
		, mBlueBits()
		, mAssociatedMonitor((monitor) ? monitor : MonitorManager::getInstance().getPrimaryMonitor())
	{
		// Log an error if the pointer to the monitor provided is invalid
		if (!mAssociatedMonitor) {
			AEON_LOG_ERROR("Invalid pointer to monitor", "The pointer to the monitor provided isn't valid.\nPossible problem: no monitors detected.\nAborting construction.");
			return;
		}

		// Retrieve the associated monitor's properties
		const VideoMode& desktopMode = mAssociatedMonitor->getDesktopMode();
		mRefreshRate = desktopMode.getRefreshRate();
		mRedBits = desktopMode.getRedBits();
		mGreenBits = desktopMode.getGreenBits();
		mBlueBits = desktopMode.getBlueBits();
	}

	VideoMode::VideoMode(const Vector2i& resolution, const VideoMode& vmode) noexcept
		: VideoMode(resolution.x, resolution.y, vmode)
	{
	}

	VideoMode::VideoMode(int32_t width, int32_t height, const VideoMode& vmode) noexcept
		: mResolution(width, height)
		, mRefreshRate(vmode.mRefreshRate)
		, mRedBits(vmode.mRedBits)
		, mGreenBits(vmode.mGreenBits)
		, mBlueBits(vmode.mBlueBits)
		, mAssociatedMonitor(vmode.mAssociatedMonitor)
	{
	}

	// Public operator(s)
	bool VideoMode::operator==(const VideoMode& other) const noexcept
	{
		return (mResolution == other.mResolution) &&
		       (mRefreshRate == other.mRefreshRate) &&
		       (mRedBits == other.mRedBits) &&
		       (mGreenBits == other.mGreenBits) &&
		       (mBlueBits == other.mBlueBits);
	}

	bool VideoMode::operator!=(const VideoMode& other) const noexcept
	{
		return !(*this == other);
	}

	// Public method(s)
	bool VideoMode::isValid(const Monitor* monitor) const
	{
		if (!monitor) {
			monitor = mAssociatedMonitor;
		}

		// Attempt to find if the video mode is in the list of valid modes
		const std::vector<VideoMode>& fullscreenModes = monitor->getFullscreenModes();
		return std::find(fullscreenModes.begin(), fullscreenModes.end(), *this) != fullscreenModes.end();
	}
}