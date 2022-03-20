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

#include <AEON/Graphics/EasingAnimation.h>

#include <AEON/Graphics/AnimationManager.h>

namespace ae
{
	// Initialize private static member(s)
	std::unordered_map<EasingAnimation::Ease, EasingAnimation::EasingFunction> EasingAnimation::easingFunctions;

	// Public constructor(s)
	EasingAnimation::EasingAnimation(const Time& duration, float startValue, float endValue, std::function<void(float)> func, Ease ease)
		: Actor()
		, mAnimationFunc(func)
		, mStartValue(startValue)
		, mEndValue(endValue)
		, mDuration(duration)
		, mElapsed(Time::Zero)
		, mEasingFunction(EasingAnimation::getEasingFunction(ease))
	{
		// Disable unnecessary functionalities
		activateFunctionality(Func::AllFunc, Target::Children, false);
		activateFunctionality(Func::EventHandle | Func::Render, Target::Self, false);

		// Add the easing animation to the manager to be kept track of
		//AnimationManager::getInstance().addAnimation(this);
	}

	// Private virtual method(s)
	void EasingAnimation::updateSelf(const Time& dt)
	{
		if (mElapsed != mDuration) {
			// Update elapsed time
			const Time NEW_ELAPSED = mElapsed + dt;
			mElapsed = (NEW_ELAPSED > mDuration) ? mDuration : NEW_ELAPSED;

			if (!getParent()) {
				mElapsed == mDuration;
				return;
			}

			// Update property
			const float FACTOR = mEasingFunction((mElapsed / mDuration).asSeconds());
			mAnimationFunc(ae::Math::lerp(mStartValue, mEndValue, FACTOR));
		}
	}

	// Private static method(s)
	const EasingAnimation::EasingFunction& EasingAnimation::getEasingFunction(Ease ease)
	{
		// Add the easing functions to the hashmap when first called
		if (easingFunctions.empty()) {
			easingFunctions.insert(std::make_pair(Ease::InSine, Math::easeInSine<double>));
			easingFunctions.insert(std::make_pair(Ease::OutSine, Math::easeOutSine<double>));
			easingFunctions.insert(std::make_pair(Ease::InOutSine, Math::easeInOutSine<double>));
			easingFunctions.insert(std::make_pair(Ease::InQuad, Math::easeInQuad<double>));
			easingFunctions.insert(std::make_pair(Ease::OutQuad, Math::easeOutQuad<double>));
			easingFunctions.insert(std::make_pair(Ease::InOutQuad, Math::easeInOutQuad<double>));
			easingFunctions.insert(std::make_pair(Ease::InCubic, Math::easeInCubic<double>));
			easingFunctions.insert(std::make_pair(Ease::OutCubic, Math::easeOutCubic<double>));
			easingFunctions.insert(std::make_pair(Ease::InOutCubic, Math::easeInOutCubic<double>));
			easingFunctions.insert(std::make_pair(Ease::InQuart, Math::easeInQuart<double>));
			easingFunctions.insert(std::make_pair(Ease::OutQuart, Math::easeOutQuart<double>));
			easingFunctions.insert(std::make_pair(Ease::InOutQuart, Math::easeInOutQuart<double>));
			easingFunctions.insert(std::make_pair(Ease::InQuint, Math::easeInQuint<double>));
			easingFunctions.insert(std::make_pair(Ease::OutQuint, Math::easeOutQuint<double>));
			easingFunctions.insert(std::make_pair(Ease::InOutQuint, Math::easeInOutQuint<double>));
			easingFunctions.insert(std::make_pair(Ease::InExpo, Math::easeInExpo<double>));
			easingFunctions.insert(std::make_pair(Ease::OutExpo, Math::easeOutExpo<double>));
			easingFunctions.insert(std::make_pair(Ease::InOutExpo, Math::easeInOutExpo<double>));
			easingFunctions.insert(std::make_pair(Ease::InCirc, Math::easeInCirc<double>));
			easingFunctions.insert(std::make_pair(Ease::OutCirc, Math::easeOutCirc<double>));
			easingFunctions.insert(std::make_pair(Ease::InOutCirc, Math::easeInOutCirc<double>));
			easingFunctions.insert(std::make_pair(Ease::InBack, Math::easeInBack<double>));
			easingFunctions.insert(std::make_pair(Ease::OutBack, Math::easeOutBack<double>));
			easingFunctions.insert(std::make_pair(Ease::InOutBack, Math::easeInOutBack<double>));
			easingFunctions.insert(std::make_pair(Ease::InElastic, Math::easeInElastic<double>));
			easingFunctions.insert(std::make_pair(Ease::OutElastic, Math::easeOutElastic<double>));
			easingFunctions.insert(std::make_pair(Ease::InOutElastic, Math::easeInOutElastic<double>));
			easingFunctions.insert(std::make_pair(Ease::InBounce, Math::easeInBounce<double>));
			easingFunctions.insert(std::make_pair(Ease::OutBounce, Math::easeOutBounce<double>));
			easingFunctions.insert(std::make_pair(Ease::InOutBounce, Math::easeInOutBounce<double>));
		}

		return easingFunctions.find(ease)->second;
	}
}