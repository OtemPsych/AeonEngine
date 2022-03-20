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

#ifndef Aeon_Graphics_GUI_IntSlider_H_
#define Aeon_Graphics_GUI_IntSlider_H_

#include <AEON/Graphics/GUI/Button.h>
#include <AEON/Graphics/EllipseShape.h>

namespace ae
{
	class AEON_API IntSlider : public Widget<EllipseShape>
	{
	public:
		// Public constructor(s)
		IntSlider();
		IntSlider(int minValue, int maxValue, int value, float size, int increment = 1);
		IntSlider(const IntSlider&) = delete;
		IntSlider(IntSlider&&) noexcept = default;

		// Public operator(s)
		IntSlider& operator=(const IntSlider&) = delete;
		IntSlider& operator=(IntSlider&&) noexcept = default;

		// Public method(s)
		void setLimits(int minValue, int maxValue, float size, int increment = 1);
		void setValue(int value);
		_NODISCARD inline Text& getText() const noexcept { return *mText; }
		_NODISCARD inline int getValue() const noexcept { return mValue.value; }
	private:
		// Private method(s)
		void init();

		// Private virtual method(s)
		virtual void handleEventSelf(Event* const event) override final;

	private:
		// Private struct(s)
		struct Value {
			int min;
			int max;
			int value;
			int increment;
		};

		// Private member(s)
		Text* mText;
		Value mValue;
		float mSize;
	};
}
#endif // Aeon_Graphics_GUI_IntSlider_H_