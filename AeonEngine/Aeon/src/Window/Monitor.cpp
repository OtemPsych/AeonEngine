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

#include <AEON/Window/Monitor.h>

#include <string>

#include <GLFW/glfw3.h>

namespace ae
{
	// Public constructor(s)
	Monitor::Monitor(GLFWmonitor* handle)
		: mVModes()
		, mOriginalVMode(nullptr)
		, mName()
		, mWorkarea()
		, mVirtualPos()
		, mPhysicalSize()
		, mContentScale()
		, mGamma(1.f)
		, mHandle(handle)
	{
		// Log an error message if the handle provided isn't valid (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (!mHandle) {
				AEON_LOG_ERROR("Invalid GLFW monitor handle", "The GLFW monitor handle provided isn't valid.\nAborting construction.");
				return;
			}
		}

		// Retrieve and store the list of the monitor's fullscreen video modes
			// Retrieve list of video modes
		int vmodeCount;
		const GLFWvidmode* glfwModes = glfwGetVideoModes(mHandle, &vmodeCount);

			// Insert the supported video modes in descending order
		mVModes.reserve(vmodeCount);
		for (int i = vmodeCount - 1; i >= 0; --i) {
			const GLFWvidmode& glfwMode = glfwModes[i];
			mVModes.emplace_back(glfwMode.width, glfwMode.height, glfwMode.refreshRate, glfwMode.redBits, glfwMode.greenBits, glfwMode.blueBits, this);
		}
		
		// Retrieve and store the current video mode of the monitor and log an error message if it isn't valid
		const GLFWvidmode* glfwMode = glfwGetVideoMode(mHandle);
		if (glfwMode) {
			mOriginalVMode = std::make_unique<VideoMode>(glfwMode->width, glfwMode->height, glfwMode->refreshRate, glfwMode->redBits, glfwMode->greenBits, glfwMode->blueBits, this);
		}
		else {
			AEON_LOG_ERROR("Null pointer to current GLFW video mode", "An error occured during the retrieval of the pointer to the monitor's current GLFW video mode.\nAborting operation.");
		}

		// Retrieve and set the remaining properties
		mName = glfwGetMonitorName(mHandle);
		glfwGetMonitorWorkarea(mHandle, &mWorkarea.position.x, &mWorkarea.position.y, &mWorkarea.size.x, &mWorkarea.size.y);
		glfwGetMonitorPos(mHandle, &mVirtualPos.x, &mVirtualPos.y);
		glfwGetMonitorPhysicalSize(mHandle, &mPhysicalSize.x, &mPhysicalSize.y);
		glfwGetMonitorContentScale(mHandle, &mContentScale.x, &mContentScale.y);
		glfwSetMonitorUserPointer(mHandle, this);
	}

	Monitor::Monitor(Monitor&& rvalue)
		: mVModes(std::move(rvalue.mVModes))
		, mOriginalVMode(std::move(rvalue.mOriginalVMode))
		, mName(std::move(rvalue.mName))
		, mWorkarea(std::move(rvalue.mWorkarea))
		, mVirtualPos(std::move(rvalue.mVirtualPos))
		, mPhysicalSize(std::move(rvalue.mPhysicalSize))
		, mContentScale(std::move(rvalue.mContentScale))
		, mGamma(rvalue.mGamma)
		, mHandle(std::exchange(rvalue.mHandle, nullptr))
	{
		// Set the monitor user pointer
		glfwSetMonitorUserPointer(mHandle, this);
	}

	// Public operator(s)
	Monitor& Monitor::operator=(Monitor&& rvalue)
	{
		// Move/copy the rvalue's member data
		mVModes = std::move(rvalue.mVModes);
		mOriginalVMode = std::move(rvalue.mOriginalVMode);
		mName = std::move(rvalue.mName);
		mWorkarea = std::move(rvalue.mWorkarea);
		mVirtualPos = std::move(rvalue.mVirtualPos);
		mPhysicalSize = std::move(rvalue.mPhysicalSize);
		mContentScale = std::move(rvalue.mContentScale);
		mGamma = rvalue.mGamma;
		mHandle = std::exchange(rvalue.mHandle, nullptr);

		// Set the monitor user pointer
		glfwSetMonitorUserPointer(mHandle, this);

		return *this;
	}

	// Public method(s)
	void Monitor::update()
	{
		glfwGetMonitorPos(mHandle, &mVirtualPos.x, &mVirtualPos.y);
	}

	const std::vector<VideoMode>& Monitor::getFullscreenModes() const noexcept
	{
		return mVModes;
	}

	const VideoMode& Monitor::getDesktopMode() const
	{
		return *mOriginalVMode;
	}

	const std::string& Monitor::getName() const noexcept
	{
		return mName;
	}

	const Box2i& Monitor::getWorkarea() const noexcept
	{
		return mWorkarea;
	}

	const Vector2i& Monitor::getVirtualPosition() const noexcept
	{
		return mVirtualPos;
	}

	const Vector2i& Monitor::getPhysicalSize() const noexcept
	{
		return mPhysicalSize;
	}

	const Vector2f& Monitor::getContentScale() const noexcept
	{
		return mContentScale;
	}

	void Monitor::setGamma(float gamma)
	{
		// Check if the gamma provided is valid (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (gamma <= 0.f) {
				AEON_LOG_ERROR("Invalid gamma exponent", "The gamma exponent " + std::to_string(gamma) + " is invalid.\nAborting operation.");
				return;
			}
		}

		mGamma = gamma;
		glfwSetGamma(mHandle, gamma);
	}

	float Monitor::getGamma() const noexcept
	{
		return mGamma;
	}

	GLFWmonitor* const Monitor::getHandle() const noexcept
	{
		return mHandle;
	}
}