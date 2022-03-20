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

#ifndef Aeon_Graphics_AnimationManager_H_
#define Aeon_Graphics_AnimationManager_H_

#include <queue>
#include <unordered_set>

#include <AEON/Graphics/EasingAnimation.h>

namespace ae
{
	/*!
	 \brief Singleton class that manages the construction and destruction of all animations.
	*/
	class AEON_API AnimationManager
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Deleted copy constructor.

		 \since v0.7.0
		*/
		AnimationManager(const AnimationManager&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.7.0
		*/
		AnimationManager(AnimationManager&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.7.0
		*/
		AnimationManager& operator=(const AnimationManager&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.7.0
		*/
		AnimationManager& operator=(AnimationManager&&) = delete;
	public:
		// Public method(s)
		/*!
		 \brief Adds an animation to be kept track of by the manager.
		 \details The animation will be deleted automatically once it's over.

		 \param[in] animation The animation to be kept track of

		 \since v0.7.0
		*/
		void addAnimation(EasingAnimation* animation);
		/*!
		 \brief Signals to the animations' parent to detach them if they're over.

		 \since v0.7.0
		*/
		void deleteFinishedAnimations();
		inline void addInvalidActor(Actor* actor) { mInvalidActors.insert(actor); }

		// Public static method(s)
		/*!
		 \brief Retrieves the single instance of the ae::AnimationManager.
		 \details The ae::AnimationManager will be instantiated for the time by calling this static method.

		 \return The single instance of the ae::AnimationManager

		 \par Example:
		 \code
		 ae::AnimationManager& animationManager = ae::AnimationManager::getInstance();
		 \endcode

		 \since v0.7.0
		*/
		_NODISCARD static AnimationManager& getInstance() noexcept;
	private:
		// Private constructor(s)
		/*!
		 \brief Default constructor.

		 \since v0.7.0
		*/
		AnimationManager() noexcept;

	private:
		// Private member(s)
		std::queue<EasingAnimation*> mAnimations;    //!< The various animations
		std::unordered_set<Actor*>   mInvalidActors; //!< The actors that may have animations that are now invalid
	};
}
#endif // Aeon_Graphics_AnimationManager_H_

/*!
 \class ae::AnimationManager
 \ingroup graphics

 The ae::AnimationManager singleton class is used to construct animations that
 can be applied on ae::Actor instances and is also responsible for destroying
 said animations once they're over.

 \author Filippos Gleglakos
 \version v0.7.0
 \date 2021.08.02
 \copyright MIT License
*/