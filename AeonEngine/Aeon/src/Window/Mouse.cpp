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

#include <AEON/Window/Mouse.h>

#include <GLFW/glfw3.h>

#include <AEON/Window/Application.h>

namespace ae
{
	namespace Mouse
	{
		bool isButtonPressed(Button button)
		{
			return glfwGetMouseButton(Application::getInstance().getWindow().getHandle(), static_cast<int>(button));
		}

		Vector2d getPosition()
		{
			Vector2d pos;
			glfwGetCursorPos(Application::getInstance().getWindow().getHandle(), &pos.x, &pos.y);

			return pos;
		}

		void grabMouse(bool flag)
		{
			int cursorMode = (flag) ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL;
			glfwSetInputMode(Application::getInstance().getWindow().getHandle(), GLFW_CURSOR, cursorMode);
		}

		bool isMouseGrabbed()
		{
			int mouseGrabbed = glfwGetInputMode(Application::getInstance().getWindow().getHandle(), GLFW_CURSOR);
			return (mouseGrabbed == GLFW_CURSOR_DISABLED);
		}
	}
}