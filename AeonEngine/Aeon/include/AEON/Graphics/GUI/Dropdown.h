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

#pragma once

#include <AEON/Graphics/GUI/Button.h>

namespace ae
{
	class AEON_API Dropdown : public Widget<RectangleShape>
	{
	public:
		// Public constructor(s)
		Dropdown();
		Dropdown(const Dropdown&) = delete;
		Dropdown(Dropdown&&) noexcept = default;
	public:
		// Public operator(s)
		Dropdown& operator=(const Dropdown&) = delete;
		Dropdown& operator=(Dropdown&&) noexcept = default;
	public:
		// Public method(s)
		void addItem(std::unique_ptr<Button<>> item);
		void updateItemPositions();
		inline void setOnItemClick(std::function<void(Button<>*, size_t)> func) noexcept { mOnItemClick = func; }
		[[nodiscard]] inline Text& getText() const noexcept { return *mText; }
		[[nodiscard]] inline std::vector<Button<>*>& getItems() noexcept { return mItems; }
	private:
		// Private method(s)
		void enableItems(bool flag);

		// Private virtual method(s)
		virtual void updateSelf(const Time& dt) override final;
		virtual void handleEventSelf(Event* const event) override final;

	private:
		// Private member(s)
		std::function<void(Button<>*, size_t)> mOnItemClick;
		Text*                                  mText;
		std::vector<Button<>*>                 mItems;
	};
}