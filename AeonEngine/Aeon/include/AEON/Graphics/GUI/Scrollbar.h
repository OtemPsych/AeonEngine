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

#include <AEON/Graphics/GUI/internal/Widget.h>
#include <AEON/Graphics/RectangleShape.h>

namespace ae
{
	class AEON_API Scrollbar : public Widget<RectangleShape>
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::Scrollbar by providing an optional start position.
		 
		 \param[in] startPosition The starting scroll position within the content area, 0 by default

		 \par Example:
		 \code
		 auto scrollbar = std::make_unique<ae::Scrollbar>(20.f);
		 \endcode

		 \since v0.7.0
		*/
		explicit Scrollbar(float startPosition = 0.f);
		/*!
		 \brief Virtual destructor.
		 \details In case the API user wishes to add additional functionalities.

		 \since v0.7.0
		*/
		virtual ~Scrollbar() = default;

		// Public method(s)
		/*!
		 \brief Scrolls by a certain distance from the current position based on the wheel offset provided.

		 \param[in] wheelOffset The wheel offset from the scroll event

		 \par Example:
		 \code
		 auto scrollbar = std::make_unique<ae::Scrollbar>();
		 ...

		 if (event->type == ae::Event::Type::MouseWheelScrolled) {
			auto mouseWheelEvent = event->as<ae::MouseWheelEvent>();
			if (mouseWheelEvent->wheel == ae::Mouse::Wheel::Vertical) {
				scrollbar->scroll(mouseWheelEvent->offset);
			}
		 }
		 \endcode

		 \sa scrollTo()

		 \since v0.7.0
		*/
		void scroll(float wheelOffset);
		/*!
		 \brief Scrolls to a specific \a position within the content area.

		 \param[in] position The position to scroll to

		 \par Example:
		 \code
		 auto scrollbar = std::make_unique<ae::Scrollbar>();
		 scrollbar->setContainer(0.f, 200.f);
		 scrollbar->setContent(0.f, 5000.f);
		 scrollbar->scrollTo(350.f);
		 \endcode

		 \sa scroll()

		 \since v0.7.0
		*/
		void scrollTo(float position);

		/*!
		 \brief Sets the container's (the visible space available) minimum position and maximum position.

		 \param[in] min The container's minimum position (top position)
		 \param[in] max The container's maximum position (bottom position)

		 \par Example:
		 \code
		 auto scrollbar = std::make_unique<ae::Scrollbar>();
		 scrollbar->setContainer(0.f, 200.f);
		 \endcode

		 \since v0.7.0
		*/
		void setContainer(float min, float max) noexcept;
		/*!
		 \brief Sets the content's (the entire space that can be scrolled through) minimum position and maximum position.

		 \param[in] min The content's minimum position (top position)
		 \param[in] max The content's maximum position (bottom position)

		 \par Example:
		 \code
		 auto scrollbar = std::make_unique<ae::Scrollbar>();
		 scrollbar->setContent(0.f, 5000.f);
		 \endcode

		 \since v0.7.0
		*/
		void setContent(float min, float max) noexcept;
		/*!
		 \brief Sets the area in which the scrollbar can scroll in.
		 \details The horizontal area is not scrollable.

		 \param[in] scrollArea The area available defined by a position and a size

		 \par Example:
		 \code
		 auto scrollbar = std::make_unique<ae::Scrollbar>();
		 scrollbar->setScrollArea(ae::Box2f(0.f, 20.f, 10.f, 300.f)); // Position: (0,20), Size: (10, 300)
		 \endcode

		 \since v0.7.0
		*/
		void setScrollArea(const ae::Box2f& scrollArea) noexcept;
		/*!
		 \brief Sets the time needed to reach the end of the scroll and the time available to speed up scrolling when the user continually scrolls.

		 \param[in] duration How long it takes to reach the end position upon scrolling (for smoother scrolling)
		 \param[in] speedUpDuration How long the user has to increase the scroll speed (scroll faster when scrolling continually)

		 \par Example:
		 \code
		 auto scrollbar = std::make_unique<ae::Scrollbar>();
		 scrollbar->setScrollDuration(ae::Time::seconds(0.15), ae::Time::seconds(0.05));
		 \endcode

		 \sa setScrollSpeed()

		 \since v0.7.0
		*/
		void setScrollDuration(const ae::Time& duration, const ae::Time& speedUpDuration) noexcept;
		/*!
		 \brief Sets the scroll speed and the scroll speed-up which increases the scroll speed as the user is continually scrolling.

		 \param[in] speed The base scroll speed
		 \param[in] speedUp The speed increase applied when the user is continually scrolling

		 \par Example:
		 \code
		 auto scrollbar = std::make_unique<ae::Scrollbar>();
		 scrollbar->setScrollSpeed(100.f, 60.f);
		 \endcode

		 \sa setScrollDuration()

		 \since v0.7.0
		*/
		void setScrollSpeed(float speed, float speedUp) noexcept;
		/*!
		 \brief Sets the function that will be called once a scroll occurs.

		 \param[in] func The custom function called

		 \par Example:
		 \code
		 ae::Camera2D* camera = ...;
		 auto scrollbar = std::make_unique<ae::Scrollbar>();
		 scrollbar->setOnScroll([camera](float newPos) {
			camera->setPosition(0.f, newPos, 0.f);
		 });
		 \endcode

		 \since v0.7.0
		*/
		inline void setOnScroll(std::function<void(float)> func) noexcept { mOnScroll = func; }
	protected:
		// Protected virtual method(s)
		/*!
		 \brief Updates the ae::Scrollbar's state based on the input event.
		 \details Updates the scroll position based on mouse wheel scrolling and mouse clicks.

		 \param[in] event The polled input ae::Event

		 \since v0.7.0
		*/
		virtual void handleEventSelf(ae::Event* const event) override;
		/*!
		 \brief Updates the scroll position if the user has scrolled.

		 \param[in] dt The time difference between the previous frame and the current frame

		 \since v0.7.0
		*/
		virtual void updateSelf(const ae::Time& dt) override;
	private:
		// Private method(s)
		/*!
		 \brief Updates the scroll widget's position within the scroll area.

		 \since v0.7.0
		*/
		void updateScroll() noexcept;
		/*!
		 \brief Updates the scroll widget's size based on the container and content heights.

		 \since v0.7.0
		*/
		void updateSize() noexcept;

	private:
		// Private member(s)
		ae::Box2f                  mScrollArea;      //!< The available space that the scrollbar can scroll in
		float                      mContainerMin;    //!< The visible space's minimum position
		float                      mContainerMax;    //!< The visible space's maximum position
		float                      mContentMin;      //!< The entire space's minimum position
		float                      mContentMax;      //!< The entire space's maximum position

		std::function<void(float)> mOnScroll;        //!< The user-provided function called when a scroll occurs
		ae::Time                   mDuration;        //!< The time needed to reach the end position
		ae::Time                   mElapsed;         //!< The time elapsed before reaching the end position
		ae::Time                   mSpeedUpDuration; //!< The time available to the user to speed up scrolling
		ae::Time                   mSpeedUpElapsed;  //!< The time elapsed until the speed-up resets
		float                      mStartPosition;   //!< The starting scroll position 
		float                      mEndPosition;     //!< The position at the start of the scroll
		float                      mCurrentPosition; //!< The current scroll position in the content area
		float                      mBaseSpeed;       //!< The base scroll speed
		float                      mSpeed;           //!< The scroll speed
		float                      mSpeedUp;         //!< The speed increase while the user is scrolling
	};
}

/*!
 \class ae::Scrollbar
 \ingroup graphics

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2022.01.01
 \copyright MIT License
*/