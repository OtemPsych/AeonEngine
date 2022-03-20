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

#ifndef Aeon_Graphics_ComponentManager_H_
#define Aeon_Graphics_ComponentManager_H_

#include <AEON/Graphics/Transform2DComponent.h>
#include <AEON/Graphics/Render2DComponent.h>
#include <AEON/Graphics/Collider2DComponent.h>

namespace ae
{
	namespace ComponentManager
	{
		/*!
		 \brief The enumeration of all available components.
		*/
		enum class Type { Transform2D, Render2D, Collider2D };

		template <class T>
		AEON_API std::enable_if_t<std::is_base_of_v<Component, T>, Type> getType()
		{
			Type type;
			if (std::is_same_v<T, Transform2DComponent>) {
				type = Type::Transform2D;
			}
			else if (std::is_same_v<T, Render2DComponent>) {
				type = Type::Render2D;
			}
			else if (std::is_same_v<T, Collider2DComponent>) {
				type = Type::Collider2D;
			}

			return type;
		}
	}
}
#endif // Aeon_Graphics_ComponentManager_H_