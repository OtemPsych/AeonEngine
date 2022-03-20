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

#include <AEON/Graphics/Actor.h>
#include <AEON/Graphics/Camera.h>
#include <AEON/Graphics/AnimationManager.h>
#include <AEON/Graphics/EasingAnimation.h>

namespace ae
{
	// Public constructor(s)
	Actor::Actor()
		: mParent(nullptr)
		, mChildren()
		, mAnimations()
		, mComponents()
		, mFuncs({
			{ Func::EventHandle, { { Target::Self, true }, { Target::Children, true } } },
			{ Func::Update,      { { Target::Self, true }, { Target::Children, true } } },
			{ Func::Render,      { { Target::Self, true }, { Target::Children, true } } } })
		, mGlobalTransform(Matrix4f::identity())
	{
	}

	Actor::Actor(const Actor& copy)
		: mParent(nullptr)
		, mChildren()
		, mAnimations()
		, mComponents()
		, mFuncs(copy.mFuncs)
		, mGlobalTransform(Matrix4f::identity())
	{
	}

	// Public operator(s)
	Actor& Actor::operator=(const Actor& other)
	{
		if (this == &other) {
			return *this;
		}

		mParent = nullptr;
		mChildren.clear();
		while (!mAnimations.empty()) {
			mAnimations.pop();
		}
		mFuncs = other.mFuncs;
		mGlobalTransform = Matrix4f::identity();

		return *this;
	}

	// Public method(s)
	void Actor::attachChild(std::unique_ptr<Actor> child)
	{
		child->mParent = this;

		EasingAnimation* validAnimation = dynamic_cast<EasingAnimation*>(child.get());
		if (validAnimation) {
			mAnimations.push(validAnimation);
		}

		mChildren.push_back(std::move(child));

		//// Update Z-ordering
		//Transform2DComponent* const transform = getComponent<Transform2DComponent>();
		//int zIndex = 0;
		//if (transform) {
		//	zIndex = static_cast<int>(transform->getPosition().z);
		//	transform->setDirty(true);
		//}
		//updateZOrdering(zIndex);
	}

	std::unique_ptr<Actor> Actor::detachChild(const Actor& child)
	{
		// Retrieve the stored child
		auto childItr = std::find_if(mChildren.begin(), mChildren.end(), [&child](std::unique_ptr<Actor>& p) {
			return p.get() == &child;
		});
		if (childItr == mChildren.end()) {
			AEON_LOG_WARNING("Invalid child", "The child provided wasn't stored in the list. Returning null.");
			return nullptr;
		}

		// Nullify the child's parent, update its z-ordering and remove it from the list
		std::unique_ptr<Actor> detachedChild = std::move(*childItr);
		detachedChild->mParent = nullptr;
		while (!detachedChild->mAnimations.empty()) {
			detachedChild->detachChild(*detachedChild->mAnimations.front());
			detachedChild->mAnimations.pop();
		}

		mChildren.erase(childItr);

		return detachedChild;
	}

	void Actor::handleEvent(Event* const event)
	{
		if (mFuncs[Func::EventHandle][Target::Children]) {
			for (auto childItr = mChildren.rbegin(); childItr != mChildren.rend(); ++childItr) {
				(*childItr)->handleEvent(event);
			}
		}
		if (mFuncs[Func::EventHandle][Target::Self]) {
			handleEventSelf(event);
		}
	}

	void Actor::update(const Time& dt)
	{
		cleanupFinishedAnimations();

		if (mFuncs[Func::Update][Target::Self]) {
			updateSelf(dt);
		}
		if (mFuncs[Func::Update][Target::Children]) {
			for (auto& child : mChildren) {
				child->update(dt);
			}
		}
	}

	void Actor::render(RenderStates states)
	{
		if (hasComponent<Transform2DComponent>()) {
			states.transform *= getComponent<Transform2DComponent>()->getTransform();
		}

		if (mFuncs[Func::Render][Target::Self]) {
			renderSelf(states);
		}
		if (mFuncs[Func::Render][Target::Children]) {
			for (auto& child : mChildren) {
				child->render(states);
			}
		}
	}

	void Actor::activateFunctionality(uint32_t func, uint32_t target, bool flag)
	{
		for (auto& funcItr : mFuncs) {
			for (auto& targetItr : funcItr.second) {
				if (func & funcItr.first && target & targetItr.first) {
					targetItr.second = flag;
				}
			}
		}
	}

	bool Actor::isFunctionalityActive(uint32_t func, uint32_t target)
	{
		for (const auto& funcItr : mFuncs) {
			for (const auto& targetItr : funcItr.second) {
				if (func & funcItr.first && target & targetItr.first && !targetItr.second) {
					return false;
				}
			}
		}
		return true;
	}

	// Public virtual method(s)
	Matrix4f Actor::getTransform()
	{
		return hasComponent<Transform2DComponent>() ? getComponent<Transform2DComponent>()->getTransform() : Matrix4f::identity();
	}

	// Protected method(s)
	void Actor::updateZOrdering(int zIndex)
	{
		// Set the z index provided to this node
		Transform2DComponent* const transform = getComponent<Transform2DComponent>();
		if (transform) {
			const Vector3f& pos = transform->getPosition();
			transform->setPosition(Vector2f(pos.xy), zIndex);
		}

		// Set superior z index to all children nodes
		const int NEXT_INDEX = zIndex + 1;
		for (auto& child : mChildren) {
			child->updateZOrdering(NEXT_INDEX);
		}
	}

	void Actor::handleEventSelf(Event* const event)
	{
	}

	void Actor::updateSelf(const Time& dt)
	{
		// Update transform-related properties
		Transform2DComponent* const transform = getComponent<Transform2DComponent>();
		if (transform) {
			transform->updateProperties();
			if (transform->isDirty()) {
				// Update z-ordering
				int zIndex = static_cast<int>(transform->getPosition().z);
				updateZOrdering(zIndex);

				// Update global transform
				mGlobalTransform = Matrix4f::identity();
				for (Actor* node = this; node != nullptr; node = node->mParent) {
					mGlobalTransform = node->getTransform() * mGlobalTransform;
				}
				for (auto& child : mChildren) {
					Transform2DComponent* const childTranform = child->getComponent<Transform2DComponent>();
					if (childTranform) {
						childTranform->setDirty(true);
					}
				}

				transform->setDirty(false);
			}
		}
	}

	void Actor::renderSelf(RenderStates states) const
	{
	}

	// Private method(s)
	void Actor::cleanupFinishedAnimations()
	{
		while (!mAnimations.empty()) {
			if (!mAnimations.front()) {
				mAnimations.pop();
				continue;
			}

			if (!mAnimations.front()->isOver()) {
				break;
			}

			detachChild(*mAnimations.front());
			mAnimations.pop();
		}
	}
}