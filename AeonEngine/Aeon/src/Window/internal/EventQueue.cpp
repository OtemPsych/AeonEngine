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

#include <AEON/Window/internal/EventQueue.h>
#include <AEON/Window/Event.h>

namespace ae
{
	// Public method(s)
	void EventQueue::enqueueEvent(std::unique_ptr<Event> event)
	{
		mQueue.push(std::move(event));
	}

	bool EventQueue::pollEvent(std::unique_ptr<Event>& event)
	{
		if (!mQueue.empty()) {
			event = std::move(mQueue.front());
			mQueue.pop();

			return true;
		}

		return false;
	}

	// Public static method(s)
	EventQueue& EventQueue::getInstance() noexcept
	{
		static EventQueue instance;
		return instance;
	}

	// Private constructor(s)
	EventQueue::EventQueue() noexcept
		: mQueue()
	{
	}
}