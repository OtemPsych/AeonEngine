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

#include <functional>

#include <AEON/Graphics/Actor.h>

namespace ae
{
	/*!
	 \brief Class representing an animation/transition affecting an actor's property.
	*/
	class AEON_API EasingAnimation : public Actor
	{
	public:
		// Public enum(s)
		/*!
		 \brief The various easing animations available.
		*/
		enum class Ease
		{
			InSine,
			OutSine,
			InOutSine,
			InQuad,
			OutQuad,
			InOutQuad,
			InCubic,
			OutCubic,
			InOutCubic,
			InQuart,
			OutQuart,
			InOutQuart,
			InQuint,
			OutQuint,
			InOutQuint,
			InExpo,
			OutExpo,
			InOutExpo,
			InCirc,
			OutCirc,
			InOutCirc,
			InBack,
			OutBack,
			InOutBack,
			InElastic,
			OutElastic,
			InOutElastic,
			InBounce,
			OutBounce,
			InOutBounce
		};

	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::EasingAnimation by providing the duration, the start value, the end value, the custom function that applies the animation and its easing type.

		 \param[in] duration Determines how long the animation should last before reaching the end position
		 \param[in] startValue Where the the value is before the animation begins
		 \param[in] endValue Where the value should be by the time the animation ends
		 \param[in] func The custom function that is used to apply the animation to the desired property
		 \param[in] ease The easing type to apply to the actor's position

		 \par Example:
		 \code
		 auto rect = std::make_unique<ae::RectangleShape>(...);
		 ...
		 ae::Transform2DComponent* const rectTransform = rect->getComponent<ae::Transform2DComponent>();

		 ae::Time duration = ae::Time::seconds(0.5);
		 float startPosition = rectTransform->getPosition().y;
		 float endPosition = 250.f;
		 auto customFunction = [rectTransform](float newPos) {
			rectTransform->setPosition(rectTransform->getPosition().x, newPos);
		 };
		 ae::EasingAnimation::Ease easeType = ae::EasingAnimation::Ease::OutCubic;
		 auto moveAnimation = std::make_unique<ae::EasingAnimation>(duration, startPosition, endPosition, customFunction, easeType);
		 \endcode

		 \since v0.7.0
		*/
		EasingAnimation(const Time& duration, float startValue, float endValue, std::function<void(float)> func, Ease ease);
		/*!
		 \brief Deleted copy constructor.

		 \since v0.7.0
		*/
		EasingAnimation(const EasingAnimation&) = delete;
		/*!
		 \brief Move constructor.

		 \since v0.7.0
		*/
		EasingAnimation(EasingAnimation&&) noexcept = default;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.7.0
		*/
		EasingAnimation& operator=(const EasingAnimation&) = delete;
		/*!
		 \brief Move assignment operator.

		 \return The caller ae::EasingAnimation

		 \since v0.7.0
		*/
		EasingAnimation& operator=(EasingAnimation&&) noexcept = default;
	public:
		// Public method(s)
		/*!
		 \brief Checks whether the animation has finished.

		 \return True if the animation has finished, false otherwise

		 \since v0.7.0
		*/
		[[nodiscard]] inline bool isOver() const noexcept { return mElapsed == mDuration; }
	private:
		// Private typedef(s)
		using EasingFunction = std::function<double(double t)>;

		// Private virtual method(s)
		/*!
		 \brief Updates the actor's property based on the time elapsed.

		 \param[in] dt The delta time between this frame and the previous one

		 \since v0.7.0
		*/
		virtual void updateSelf(const Time& dt) override final;

		// Private static method(s)
		/*!
		 \brief Retrieves the easing function associated to the easing type provided.

		 \param[in] ease The easing type associated with the desired easing function

		 \since v0.7.0
		*/
		static const EasingFunction& getEasingFunction(Ease ease);

	private:
		// Private static member(s)
		static std::unordered_map<Ease, EasingFunction> easingFunctions; //!< The associated easing functions with the easing type

		// Private member(s)
		std::function<void(float)> mAnimationFunc;  //!< The custom user function to which the animation is applied
		const float                mStartValue;     //!< The start value of the actor
		const float                mEndValue;       //!< The end value of the actor
		const Time                 mDuration;       //!< The time to ease the position from the start to the end
		Time                       mElapsed;        //!< The time elapsed clamped between 0 and the duration
		const EasingFunction&      mEasingFunction; //!< The easing function to apply to the actor's position
	};
}

/*!
 \class ae::EasingAnimation
 \ingroup graphics

 The ae::EasingAnimation class is used to apply an animation to an actor's
 position which is particularly useful for GUI elements' transitions that
 provide a more dynamic way to introduce buttons, textboxes, etc. to the user.

 All ae::EasingAnimation instances are automatically deleted from the parent's
 node graph once its animation has finished.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.08.02
 \copyright MIT License
*/