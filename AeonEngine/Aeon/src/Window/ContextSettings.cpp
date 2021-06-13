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

#include <AEON/Window/ContextSettings.h>

#include <string>

#include <GLFW/glfw3.h>

#include <AEON/System/DebugLogger.h>

namespace ae
{
	// Public constructor(s)
	ContextSettings::ContextSettings(int msaa, int major, int minor, int depth, int stencil, bool sRgb)
		: mAntialiasingLevel(0)
		, mMajorVersion(4)
		, mMinorVersion(5)
		, mDepthBits(24)
		, mStencilBits(8)
		, mSrgbCapable(sRgb)
	{
		setAntialiasingLevel(msaa);
		setContextVersion(major, minor);
		setDepthBits(depth);
		setStencilBits(stencil);
	}

	// Public method(s)
	void ContextSettings::apply() const
	{
		// Window-related hints
		glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
		glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
		glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
		glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_FALSE);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
		glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

		// Framebuffer-related hints
		glfwWindowHint(GLFW_DEPTH_BITS, mDepthBits);
		glfwWindowHint(GLFW_STENCIL_BITS, mStencilBits);
		glfwWindowHint(GLFW_STEREO, GLFW_FALSE);
		glfwWindowHint(GLFW_SAMPLES, mAntialiasingLevel);
		glfwWindowHint(GLFW_SRGB_CAPABLE, mSrgbCapable);
		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

		// Context-related hints
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, mMajorVersion);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, mMinorVersion);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, AEON_DEBUG);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_NO_ROBUSTNESS);
		glfwWindowHint(GLFW_CONTEXT_RELEASE_BEHAVIOR, GLFW_ANY_RELEASE_BEHAVIOR);
		glfwWindowHint(GLFW_CONTEXT_NO_ERROR, !AEON_DEBUG);
	}

	void ContextSettings::setAntialiasingLevel(int msaa)
	{
		// Check if the msaa provided is a valid level (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (msaa != 0 && fmodf(log2f(static_cast<float>(msaa)) * 2.f, 2.f) != 0.f) {
				AEON_LOG_ERROR("Invalid anti-aliasing level", "The antialiasing level of x" + std::to_string(msaa) + " provided isn't a valid level.\nAborting operation.");
				return;
			}
		}
		mAntialiasingLevel = msaa;
	}

	int ContextSettings::getAntialiasingLevel() const noexcept
	{
		return mAntialiasingLevel;
	}

	void ContextSettings::setContextVersion(int major, int minor)
	{
		// Log an error message if the chosen version is inferior to 4.5 (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if ((major == 4 && minor < 5) || major < 4) {
				AEON_LOG_ERROR("Invalid OpenGL version", "The minimum OpenGL version supported is version 4.5.\nAborting operation.");
				return;
			}
		}

		mMajorVersion = major;
		mMinorVersion = minor;
	}

	void ContextSettings::getContextVersion(int& major, int& minor) const noexcept
	{
		major = mMajorVersion;
		minor = mMinorVersion;
	}

	void ContextSettings::setDepthBits(int depth)
	{
		// Log an error message if the chosen depth bits are inferior to 16 and not a multiple of 8 (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (depth < 16 || depth % 8 != 0) {
				AEON_LOG_ERROR("Invalid number of depth bits", "The depth bits \"" + std::to_string(depth) + "\" provided is invalid.\nAborting operation.");
				return;
			}
		}

		mDepthBits = depth;
	}

	int ContextSettings::getDepthBits() const noexcept
	{
		return mDepthBits;
	}

	void ContextSettings::setStencilBits(int stencil)
	{
		// Log an error message if the chosen stencil bits are inferior to 0 (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (stencil < 0) {
				AEON_LOG_ERROR("Invalid number of stencil bits", "The stencil bits \"" + std::to_string(stencil) + "\" provided is invalid.\nAborting operation.");
				return;
			}
		}

		mStencilBits = stencil;
	}

	int ContextSettings::getStencilBits() const noexcept
	{
		return mStencilBits;
	}

	void ContextSettings::setSrgbEnabled(bool flag) noexcept
	{
		mSrgbCapable = flag;
	}

	bool ContextSettings::isSrgbEnabled() const noexcept
	{
		return mSrgbCapable;
	}
}