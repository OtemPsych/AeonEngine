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

#include <AEON/Window/Cursor.h>

#include <GLFW/glfw3.h>

#include <AEON/System/DebugLogger.h>

namespace ae
{
	// Public constructor(s)
	Cursor::Cursor() noexcept
		: mCursor(nullptr)
	{
	}

	Cursor::Cursor(unsigned char* pixels, const Vector2i& size, const Vector2i& hotspot)
		: mCursor(nullptr)
	{
		createCustom(pixels, size, hotspot);
	}

	Cursor::Cursor(Type type)
		: mCursor(nullptr)
	{
		createStandard(type);
	}

	Cursor::~Cursor()
	{
		if (mCursor) {
			glfwDestroyCursor(mCursor);
		}
	}

	// Public method(s)
	void Cursor::createCustom(unsigned char* pixels, const Vector2i& size, const Vector2i& hotspot)
	{
		// Create the image
		GLFWimage image;
		image.width = size.x;
		image.height = size.y;
		image.pixels = pixels;

		// Create the custom cursor
		GLFWcursor* cursor = glfwCreateCursor(&image, hotspot.x, hotspot.y);
		replaceCursor(cursor);
	}

	void Cursor::createStandard(Type type)
	{
		GLFWcursor* cursor = glfwCreateStandardCursor(static_cast<int>(type));
		replaceCursor(cursor);
	}

	// Private method(s)
	void Cursor::replaceCursor(GLFWcursor* cursor)
	{
		// Check if cursor creation failed
		if (!cursor) {
			AEON_LOG_ERROR("Cursor creation failed", "Failed to create custom cursor.\nAborting operation.");
			return;
		}

		// Replace the active cursor
		if (mCursor) {
			glfwDestroyCursor(mCursor);
		}
		mCursor = cursor;
	}
}