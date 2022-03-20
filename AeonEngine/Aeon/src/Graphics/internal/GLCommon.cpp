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

#include <AEON/Graphics/internal/GLCommon.h>

#include <GL/glew.h>

namespace ae
{
	namespace gl
	{
		// Function(s)
		void checkError(const char* statement)
		{
			// Retrieve the OpenGL error (if one occurred)
			GLenum GLError = glGetError();

			// Check if an error occurred
			if (GLError != GL_NO_ERROR)
			{
				std::string errorTypeStr = "";

				// Check which OpenGL error occurred
				switch (GLError)
				{
				case GL_INVALID_ENUM:
					errorTypeStr = "GL_INVALID_ENUM";
					break;
				case GL_INVALID_VALUE:
					errorTypeStr = "GL_INVALID_VALUE";
					break;
				case GL_INVALID_OPERATION:
					errorTypeStr = "GL_INVALID_OPERATION";
					break;
				case GL_INVALID_FRAMEBUFFER_OPERATION:
					errorTypeStr = "GL_INVALID_FRAMEBUFFER_OPERATION";
					break;
				case GL_OUT_OF_MEMORY:
					errorTypeStr = "GL_OUT_OF_MEMORY";
					break;
				case GL_STACK_UNDERFLOW:
					errorTypeStr = "GL_STACK_UNDERFLOW";
					break;
				case GL_STACK_OVERFLOW:
					errorTypeStr = "GL_STACK_OVERFLOW";
				}

				// Log the OpenGL error message
				AEON_LOG_ERROR("OpenGL Error", "Type: " + errorTypeStr + "\nOpenGL Statement: " + statement);
			}
		}
	}
}