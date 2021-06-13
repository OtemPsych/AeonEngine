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

#include <AEON/Graphics/Actor2D.h>

namespace ae
{
	// Public constructor(s)
	Actor2D::Actor2D()
		: Transformable2D()
		, Renderable2D()
		, mGlobalTransform(1.f)
		, mParent(nullptr)
		, mChildren()
		, mFuncs({
			{ Func::EventHandle, { { Target::Self, true }, { Target::Children, true } } },
			{ Func::Update,      { { Target::Self, true }, { Target::Children, true } } },
			{ Func::Render,      { { Target::Self, true }, { Target::Children, true } } }})
		, mAlignment(std::make_pair(false, std::make_pair(OriginFlag::Top | OriginFlag::Left, Vector2f(0.f))))
	{
	}

	Actor2D::Actor2D(const Actor2D& copy)
		: Transformable2D(copy)
		, Renderable2D(copy)
		, mParent(copy.mParent)
		, mGlobalTransform(1.f)
		, mChildren()
		, mFuncs(copy.mFuncs)
		, mAlignment(copy.mAlignment)
	{
	}

	Actor2D::Actor2D(Actor2D&& rvalue) noexcept
		: Transformable2D(std::move(rvalue))
		, Renderable2D(std::move(rvalue))
		, mParent(rvalue.mParent)
		, mGlobalTransform(std::move(rvalue.mGlobalTransform))
		, mChildren(std::move(rvalue.mChildren))
		, mFuncs(std::move(rvalue.mFuncs))
		, mAlignment(std::move(rvalue.mAlignment))
	{
	}

	// Public operator(s)
	Actor2D& Actor2D::operator=(const Actor2D& other)
	{
		// Check if the caller is being assigned to itself (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (this == &other) {
				AEON_LOG_WARNING("Invalid assignment", "The caller is being assigned to itself.\nAborting assignment.");
				return *this;
			}
		}

		// Copy the other's data except its children
		Transformable2D::operator=(other);
		Renderable2D::operator=(other);
		mGlobalTransform = Matrix4f::identity();
		mParent = other.mParent;
		mFuncs = other.mFuncs;
		mAlignment = other.mAlignment;

		return *this;
	}

	Actor2D& Actor2D::operator=(Actor2D&& rvalue) noexcept
	{
		// Copy the trival data and move the rest
		Transformable2D::operator=(std::move(rvalue));
		Renderable2D::operator=(std::move(rvalue));
		mParent = rvalue.mParent;
		mGlobalTransform = std::move(rvalue.mGlobalTransform);
		mChildren = std::move(rvalue.mChildren);
		mFuncs = std::move(rvalue.mFuncs);
		mAlignment = std::move(rvalue.mAlignment);

		return *this;
	}

	// Public method(s)
	void Actor2D::attachChild(std::unique_ptr<Actor2D> child)
	{
		child->mParent = this;
		mChildren.push_back(std::move(child));

		// Update Z-ordering
		const int Z_INDEX = (mParent) ? static_cast<int>(getPosition().z) : 0;
		updateZOrdering(Z_INDEX);
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
		result->updateZOrdering(0);
		mChildren.erase(found);

		return result;
	}

	void Actor2D::setRelativeAlignment(uint32_t alignmentFlags, const Vector2f& padding)
	{
		// Check if the caller has a parent
		if (!mParent) {
			AEON_LOG_WARNING("Invalid alignment", "Attempt to relatively align parentless actor.\nAborting operation.");
			return;
		}

		// Align the caller to its parent based on the alignment flags
		const Box2f PARENT_BOUNDS = mParent->getModelBounds();
		if (alignmentFlags == OriginFlag::Center) {
			setPosition(PARENT_BOUNDS.min + PARENT_BOUNDS.max / 2.f);
		}
		else {
			Vector2f newPos(PARENT_BOUNDS.min + padding);
			if (alignmentFlags & OriginFlag::CenterX) {
				newPos.x = PARENT_BOUNDS.min.x + PARENT_BOUNDS.max.x / 2.f;
			}
			else if (alignmentFlags & OriginFlag::Right) {
				newPos.x = PARENT_BOUNDS.min.x + PARENT_BOUNDS.max.x - padding.x;
			}

			if (alignmentFlags & OriginFlag::CenterY) {
				newPos.y = PARENT_BOUNDS.min.y + PARENT_BOUNDS.max.y / 2.f;
			}
			else if (alignmentFlags & OriginFlag::Bottom) {
				newPos.y = PARENT_BOUNDS.min.y + PARENT_BOUNDS.max.y - padding.y;
			}

			setPosition(newPos);
		}

		// Store the new alignment properties
		mAlignment = std::make_pair(true, std::make_pair(alignmentFlags, padding));
	}

	void Actor2D::handleEvent(Event* const event)
	{
		if (mFuncs[Func::EventHandle][Target::Children]) {
			handleEventChildren(event);
		}
		if (mFuncs[Func::EventHandle][Target::Self]) {
			handleEventSelf(event);
		}
	}

	void Actor2D::update(const Time& dt)
	{
		removeChildrenMarkedForRemoval();

		if (mFuncs[Func::Update][Target::Self]) {
			updateSelf(dt);
		}
		if (mFuncs[Func::Update][Target::Children]) {
			updateChildren(dt);
		}
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
		//if (mUpdateGlobalTransform) {
			//mUpdateGlobalTransform = false;
			mGlobalTransform = Matrix4f::identity();
			for (Actor2D* node = this; node != nullptr; node = node->mParent) {
				mGlobalTransform = node->getTransform() * mGlobalTransform;
			}
	//	}

		return mGlobalTransform;
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

	void Actor2D::correctProperties()
	{
		// Correct caller node's properties
		Transformable2D::correctProperties();
		if (mAlignment.first) {
			setRelativeAlignment(mAlignment.second.first, mAlignment.second.second);
		}

		// Correct the children nodes' properties
		for (const auto& child : mChildren) {
			child->correctProperties();
		}
	}

	void Actor2D::render(RenderStates states)
	{
		states.transform *= getTransform();

		if (mFuncs[Func::Render][Target::Self]) {
			renderSelf(states);
		}
		if (mFuncs[Func::Render][Target::Children]) {
			renderChildren(states);
		}
	}

	Box2f Actor2D::getModelBounds() const
	{
		return Box2f();
	}

	// Protected method(s)
	void Actor2D::updateZOrdering(int zIndex)
	{
		// Set the z index provided to this node and raise flag to update global transform
		const Vector3f& pos = getPosition();
		setPosition(pos.x, pos.y, zIndex);

		// Set superior z index to all children nodes, and they'll pass the next z index to their children
		const int NEXT_ZINDEX = zIndex + 1;
		for (auto& child : mChildren) {
			child->updateZOrdering(NEXT_ZINDEX);
		}
	}

	// Private method(s)
	void Actor2D::removeChildrenMarkedForRemoval()
	{
		mChildren.erase(std::remove_if(mChildren.begin(), mChildren.end(), [](const std::unique_ptr<Actor2D>& child) {
			return child->isMarkedForRemoval();
		}), mChildren.end());
	}

	void Actor2D::handleEventChildren(Event* const event)
	{
		for (auto& child : mChildren) {
			child->handleEvent(event);
		}
	}

	void Actor2D::updateChildren(const Time& dt)
	{
		for (auto& child : mChildren) {
			child->update(dt);
		}
	}

	void Actor2D::renderChildren(RenderStates states) const
	{
		for (const auto& child : mChildren) {
			child->render(states);
		}
	}

	// Private virtual method(s)
	void Actor2D::handleEventSelf(Event* const event)
	{
	}

	void Actor2D::updateSelf(const Time& dt)
	{
	}

	void Actor2D::renderSelf(RenderStates states) const
	{
	}
}