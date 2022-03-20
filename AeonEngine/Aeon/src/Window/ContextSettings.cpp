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

#include <AEON/Window/ContextSettings.h>

#include <cassert>
#include <cmath>

#include <GLFW/glfw3.h>

namespace ae
{
	// Public constructor(s)
	ContextSettings::ContextSettings(int32_t msaa, int32_t major, int32_t minor, int32_t depth, int32_t stencil, bool sRgb)
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

	void ContextSettings::setAntialiasingLevel(int32_t msaa)
	{
		assert(msaa == 0.f || fmodf(log2f(static_cast<float>(msaa)) * 2.f, 2.f) == 0.f);
		mAntialiasingLevel = msaa;
	}

	void ContextSettings::setContextVersion(int32_t major, int32_t minor)
	{
		assert((major == 4 && minor >= 5) || major > 4); // version >= 4.5

		mMajorVersion = major;
		mMinorVersion = minor;
	}

	void ContextSettings::getContextVersion(int32_t& major, int32_t& minor) const noexcept
	{
		major = mMajorVersion;
		minor = mMinorVersion;
	}

	void ContextSettings::setDepthBits(int32_t depth)
	{
		assert(depth >= 16 && depth % 8 == 0);
		mDepthBits = depth;
	}

	void ContextSettings::setStencilBits(int32_t stencil)
	{
		assert(stencil >= 0);
		mStencilBits = stencil;
	}
}