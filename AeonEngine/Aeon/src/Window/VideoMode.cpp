// MIT License
// 
// Copyright(c) 2019 Filippos Gleglakos
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
#include <AEON/Window/Monitor.h>

namespace ae
{
	// Public constructor(s)
	VideoMode::VideoMode(const Vector2i& resolution, int refreshRate, int redBits, int greenBits, int blueBits, const Monitor* const monitor)
		: VideoMode(resolution.x, resolution.y, refreshRate, redBits, greenBits, blueBits, monitor)
	{
	}

	VideoMode::VideoMode(int width, int height, int refreshRate, int redBits, int greenBits, int blueBits, const Monitor* const monitor)
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

	VideoMode::VideoMode(int width, int height, const Monitor* const monitor)
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

		const VideoMode& desktopMode = mAssociatedMonitor->getDesktopMode();
		mRefreshRate = desktopMode.getRefreshRate();
		mRedBits = desktopMode.getRedBits();
		mGreenBits = desktopMode.getGreenBits();
		mBlueBits = desktopMode.getBlueBits();
	}

	VideoMode::VideoMode(const Vector2i& resolution, const VideoMode& vmode) noexcept
		: mResolution(resolution)
		, mRefreshRate(vmode.mRefreshRate)
		, mRedBits(vmode.mRedBits)
		, mGreenBits(vmode.mGreenBits)
		, mBlueBits(vmode.mBlueBits)
		, mAssociatedMonitor(vmode.mAssociatedMonitor)
	{
	}

	VideoMode::VideoMode(int width, int height, const VideoMode& vmode) noexcept
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
		// Check the validity of the monitor provided or of the associated monitor if nullptr
		if (!monitor) {
			monitor = mAssociatedMonitor;
		}

		// Retrieve the monitor's fullscreen modes and compare them
		const std::vector<VideoMode>& fullscreenModes = monitor->getFullscreenModes();
		auto found = std::find(fullscreenModes.begin(), fullscreenModes.end(), *this);

		return (found != fullscreenModes.end());
	}

	const Vector2i& VideoMode::getResolution() const noexcept
	{
		return mResolution;
	}

	int VideoMode::getWidth() const noexcept
	{
		return mResolution.x;
	}

	int VideoMode::getHeight() const noexcept
	{
		return mResolution.y;
	}

	int VideoMode::getRefreshRate() const noexcept
	{
		return mRefreshRate;
	}

	int VideoMode::getRedBits() const noexcept
	{
		return mRedBits;
	}

	int VideoMode::getGreenBits() const noexcept
	{
		return mGreenBits;
	}

	int VideoMode::getBlueBits() const noexcept
	{
		return mBlueBits;
	}

	const Monitor* const VideoMode::getAssociatedMonitor() const noexcept
	{
		return mAssociatedMonitor;
	}
}