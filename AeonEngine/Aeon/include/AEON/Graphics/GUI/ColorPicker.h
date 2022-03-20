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

#include <AEON/Graphics/GUI/IntSlider.h>
#include <AEON/Graphics/RectangleShape.h>
#include <AEON/Graphics/EllipseShape.h>

namespace ae
{
	/*!
	 \brief The class used to show a GUI color picker.
	*/
	class AEON_API ColorPicker : public Actor
	{
	public:
		// Public constructor(s)
		explicit ColorPicker(const Vector2f& size, size_t sliceCount = 35);

		// Public method(s)
		void setSize(const Vector2f& size);
		void setSliceCount(size_t count);
		void setHue(float hue);
		void setColor(const Color& color);

		[[nodiscard]] inline EllipseShape* const getSelection() noexcept { return mSelection; }
		[[nodiscard]] inline float getHue() const noexcept { return mHue; }

	private:
		// Private method(s)
		void updateSlices();
		void updateSliderBackgroundSlices();
		void updateSelection();
		void init(const Vector2f& size);

		// Private virtual method(s)
		virtual void updateSelf(const Time& dt) override final;
		virtual void handleEventSelf(Event* const event) override final;

	private:
		// Private member(s)
		std::vector<RectangleShape*> mSlices;
		std::vector<RectangleShape*> mSliderBackgroundSlices;
		IntSlider*                   mHueSlider;
		EllipseShape*                mSelection;
		float                        mHue;
		bool                         mSelectionHeld;
	};
}