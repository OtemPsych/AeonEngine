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

#include <AEON/Window/Keyboard.h>

#include <GLFW/glfw3.h>

#include <AEON/Window/Application.h>

namespace ae
{
	namespace Keyboard
	{
		GLFWwindow* currentContext = nullptr; //!< The cached pointer to the current context

		// Inaccessible helper function
		void cacheCurrentContext()
		{
			// Cache the current OpenGL context
			if (!currentContext) {
				currentContext = ae::Application::getInstance().getWindow().getHandle();
			}
		}

		// Function(s)
		bool isKeyPressed(Key key)
		{
			cacheCurrentContext();
			return glfwGetKey(currentContext, static_cast<int>(key));
		}

		std::string getKeyName(Key key)
		{
			return glfwGetKeyName(static_cast<int>(key), 0);
		}
	}
}