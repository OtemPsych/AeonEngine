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

#ifndef Aeon_Graphics_GUI_Scrollbar_H_
#define Aeon_Graphics_GUI_Scrollbar_H_

#include <AEON/Graphics/GUI/internal/Widget.h>
#include <AEON/Graphics/RectangleShape.h>

namespace ae
{
	class AEON_API Scrollbar : public Widget<RectangleShape>
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Default constructor.
		 \details Sets the ae::Scrollbar's origin to the top right corner.

		 \param[in] actualSize The associated container's actual size
		 
		 \since v0.6.0
		*/
		explicit Scrollbar(float actualSize);
		/*!
		 \brief Deleted copy constructor.

		 \since v0.6.0
		*/
		Scrollbar(const Scrollbar&) = delete;
		/*!
		 \brief Move constructor.

		 \param[in] rvalue The ae::Scrollbar that will be moved.

		 \since v0.6.0
		*/
		Scrollbar(Scrollbar&& rvalue) noexcept = default;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.6.0
		*/
		Scrollbar& operator=(const Scrollbar&) = delete;
		/*!
		 \brief Move assignment operator.

		 \param[in] rvalue The ae::Scrollbar that will be moved

		 \return The caller ae::Scrollbar

		 \since v0.6.0
		*/
		Scrollbar& operator=(Scrollbar&&) noexcept = default;
	public:
		// Public method(s)
		void scrollTo(float pos);
		void scroll(float offset);
		void updateSize(float actualSize, float visibleSize);
	private:
		// Private virtual method(s)
		/*!
		 \brief Checks for mouse mouvement and mouse clicks.

		 \param[in] event The polled input ae::Event

		 \since v0.6.0
		*/
		virtual void handleEventSelf(Event* const event) override final;

	private:
		// Private member(s)
		float mActualSize;  //!< The associated container's actual size
		float mVisibleSize; //!< The associated container's visible size
	};
}
#endif // Aeon_Graphics_GUI_VerticalScrollbar_H_