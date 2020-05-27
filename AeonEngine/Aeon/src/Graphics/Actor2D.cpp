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

#include <AEON/Graphics/Actor2D.h>

namespace ae
{
	// Public constructor(s)
	Actor2D::Actor2D()
		: Transformable2D()
		, Renderable2D()
		, mParent(nullptr)
		, mChildren()
		, mFuncs()
	{
		// Initialize the associate functionalities and targets
		mFuncs = {
			{ Func::EventHandle, { { Target::Self, true }, { Target::Children, true } } },
			{ Func::Update,      { { Target::Self, true }, { Target::Children, true } } },
			{ Func::Render,      { { Target::Self, true }, { Target::Children, true } } }
		};
	}

	Actor2D::Actor2D(const Actor2D& copy)
		: Transformable2D(copy)
		, Renderable2D(copy)
		, mParent(copy.mParent)
		, mChildren()
		, mFuncs(copy.mFuncs)
	{
		// Create copies of the children and add them in
		for (const auto& child : copy.mChildren) {
			mChildren.emplace_back(std::make_unique<Actor2D>(*child));
		}
	}

	Actor2D::~Actor2D()
	{
	}

	// Public method(s)
	void Actor2D::attachChild(std::unique_ptr<Actor2D> child)
	{
		child->mParent = this;
		mChildren.push_back(std::move(child));
	}

	std::unique_ptr<Actor2D> Actor2D::detachChild(const Actor2D& child)
	{
		// Retrieve the stored child and check if it was found
		auto found = std::find_if(mChildren.begin(), mChildren.end(), [&child](std::unique_ptr<Actor2D>& p) {
			return p.get() == &child;
		});
		if (found == mChildren.end()) {
			AEON_LOG_ERROR("Invalid child", "The child provided wasn't stored in the list. Returning null.");
			return nullptr;
		}

		// Nullify the child's parent, remove it from the list and return it
		std::unique_ptr<Actor2D> result = std::move(*found);
		result->mParent = nullptr;
		mChildren.erase(found);

		return std::move(result);
	}

	bool Actor2D::handleEvent(Event* const event)
	{
		bool allow = true;
		if (mFuncs[Func::EventHandle][Target::Self]) {
			allow = handleEventSelf(event);
		}
		if (allow && mFuncs[Func::EventHandle][Target::Children]) {
			allow = handleEventChildren(event);
		}

		return allow;
	}

	bool Actor2D::update(const Time& dt)
	{
		removeChildrenMarkedForRemoval();

		bool allow = true;
		if (mFuncs[Func::Update][Target::Self]) {
			allow = updateSelf(dt);
		}
		if (allow && mFuncs[Func::Update][Target::Children]) {
			allow = updateChildren(dt);
		}

		return allow;
	}

	void Actor2D::activateFunctionality(uint32_t func, uint32_t target, bool flag)
	{
		for (auto& funcItr : mFuncs) {
			for (auto& targetItr : funcItr.second) {
				if (func & funcItr.first && target & targetItr.first) {
					targetItr.second = flag;
				}
			}
		}
	}

	Matrix4f Actor2D::getGlobalTransform()
	{
		Matrix4f globalTransform(1.f);
		for (Actor2D* node = this; node != nullptr; node = mParent) {
			globalTransform = node->getTransform() * globalTransform;
		}

		return globalTransform;
	}

	Box2f Actor2D::getGlobalBounds()
	{
		const Box2f MODEL_BOUNDS = getModelBounds();
		const Matrix4f GLOBAL_TRANSFORM = getGlobalTransform();

		return Box2f(Vector2f(GLOBAL_TRANSFORM * Vector3f(MODEL_BOUNDS.min)), Vector2f(GLOBAL_TRANSFORM * Vector3f(MODEL_BOUNDS.max)));
	}

	Vector2f Actor2D::getGlobalPosition()
	{
		return Vector2f(getGlobalTransform() * Vector3f(getPosition()));
	}

	// Public virtual method(s)
	bool Actor2D::isMarkedForRemoval() const
	{
		return isDestroyed();
	}

	bool Actor2D::isDestroyed() const
	{
		return false;
	}

	bool Actor2D::render(RenderStates states)
	{
		states.transform *= getTransform();

		bool allow = true;
		if (mFuncs[Func::Render][Target::Self]) {
			allow = renderSelf(states);
		}
		if (allow && mFuncs[Func::Render][Target::Children]) {
			allow = renderChildren(states);
		}

		return allow;
	}

	Box2f Actor2D::getModelBounds() const
	{
		return Box2f();
	}

	// Private method(s)
	void Actor2D::removeChildrenMarkedForRemoval()
	{
		mChildren.erase(std::remove_if(mChildren.begin(), mChildren.end(), [](std::unique_ptr<Actor2D>& child) {
			return child->isMarkedForRemoval();
		}));
	}

	bool Actor2D::handleEventChildren(Event* const event)
	{
		for (auto& child : mChildren) {
			if (!child->handleEvent(event)) {
				return false;
			}
		}

		return true;
	}

	bool Actor2D::updateChildren(const Time& dt)
	{
		for (auto& child : mChildren) {
			if (!child->update(dt)) {
				return false;
			}
		}

		return true;
	}

	bool Actor2D::renderChildren(RenderStates states)
	{
		for (auto& child : mChildren) {
			if (!child->render(states)) {
				return false;
			}
		}

		return true;
	}

	// Private virtual method(s)
	bool Actor2D::handleEventSelf(Event* const event)
	{
		return true;
	}

	bool Actor2D::updateSelf(const Time& dt)
	{
		return true;
	}

	bool Actor2D::renderSelf(RenderStates states)
	{
		return true;
	}
}