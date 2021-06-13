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

#ifndef Aeon_Graphics_H_
#define Aeon_Graphics_H_

// Include all the necessary headers of the Graphics module
#include <AEON/Graphics/BlendMode.h>
#include <AEON/Graphics/Camera.h>
#include <AEON/Graphics/Camera2D.h>
#include <AEON/Graphics/Camera3D.h>
#include <AEON/Graphics/CameraFPS.h>
#include <AEON/Graphics/Color.h>
#include <AEON/Graphics/GLResourceFactory.h>
#include <AEON/Graphics/Shader.h>
#include <AEON/Graphics/Transformable.h>
#include <AEON/Graphics/Transformable2D.h>
#include <AEON/Graphics/Texture.h>
#include <AEON/Graphics/Texture2D.h>
#include <AEON/Graphics/Material.h>
#include <AEON/Graphics/Renderer2D.h>
#include <AEON/Graphics/Font.h>
#include <AEON/Graphics/RenderTexture.h>
#include <AEON/Graphics/RenderStates.h>
#include <AEON/Graphics/TextureAtlas.h>
#include <AEON/Graphics/Actor2D.h>
#include <AEON/Graphics/Sprite.h>
#include <AEON/Graphics/EllipseShape.h>
#include <AEON/Graphics/RectangleShape.h>
#include <AEON/Graphics/Text.h>

#endif // Aeon_Graphics_H_

/*!
 \defgroup graphics Graphics module
 A module providing all the graphical functionalities of the game engine.
 In this module, we find the classes that can be used to draw shapes and models
 to the screen.
*/