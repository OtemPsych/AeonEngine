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

#ifndef Aeon_Window_H_
#define Aeon_Window_H_

// Include all the necessary headers of the Window module
#include <AEON/Window/VideoMode.h>
#include <AEON/Window/Monitor.h>
#include <AEON/Window/MonitorManager.h>
#include <AEON/Window/ContextSettings.h>
#include <AEON/Window/Window.h>
#include <AEON/Window/Event.h>
#include <AEON/Window/Keyboard.h>
#include <AEON/Window/Application.h>
#include <AEON/Window/Mouse.h>
#include <AEON/Window/State.h>

#endif // Aeon_Window_H_

/*!
 \defgroup window Window module

 A module providing functionalities concerning the creation of a window as well
 as the configuration of said window (the OpenGL context options and the video
 mode). This module also has the Mouse and Keyboard namespaces that contain
 functions providing access to the real-time state of the mouse's buttons and
 the keyboard's keys respectively. The Application class is responsible for
 managing the entire application (the API user's application) and has to be
 instantiated by the API user at the start of their application.
*/