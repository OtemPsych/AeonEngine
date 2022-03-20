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

#include <AEON/Graphics/Transform2DComponent.h>

#include <AEON/Graphics/Actor.h>
#include <AEON/Graphics/Collider2DComponent.h>

namespace ae
{
	// Public constructor(s)
	Transform2DComponent::Transform2DComponent(Actor& associatedActor) noexcept
		: Component(associatedActor)
		, mTransform(Matrix4f::identity())
		, mInvTransform(Matrix4f::identity())
		, mAlignment({ Vector2f(0.f, 0.f), OriginFlag::Left | OriginFlag::Top, false })
		, mTranslationOrigin({ Vector2f(0.f, 0.f), OriginFlag::Left | OriginFlag::Top })
		, mRotationOrigin({ Vector2f(), OriginFlag::Center })
		, mScaleOrigin({ Vector2f(), OriginFlag::Center })
		, mPosition(0.f, 0.f, 0.f)
		, mScale(1.f, 1.f)
		, mRotation(0.f)
		, mTransformDirty(false)
		, mInvTransformDirty(false)
		, mHasFixedZIndex(false)
	{
	}

	// Public method(s)
	void Transform2DComponent::setPosition(const Vector2f& position, int zIndex, bool zIndexFixed) noexcept
	{
		mPosition.xy = position;
		mTransformDirty = true;

		if (zIndex != INT_MAX && (!mHasFixedZIndex || zIndexFixed)) {
			mPosition.z = static_cast<float>(zIndex);
			mHasFixedZIndex = zIndexFixed;
		}
	}

	void Transform2DComponent::setPosition(float posX, float posY, int zIndex, bool zIndexFixed) noexcept
	{
		mPosition.x = posX;
		mPosition.y = posY;
		mTransformDirty = true;

		if (zIndex != INT_MAX && (!mHasFixedZIndex || zIndexFixed)) {
			mPosition.z = static_cast<float>(zIndex);
			mHasFixedZIndex = zIndexFixed;
		}
	}

	void Transform2DComponent::setRotation(float angle) noexcept
	{
		mRotation = angle;
		mTransformDirty = true;
	}

	void Transform2DComponent::setScale(const Vector2f& scale) noexcept
	{
		mScale = scale;
		mTransformDirty = true;
	}

	void Transform2DComponent::setScale(float scaleX, float scaleY) noexcept
	{
		mScale.x = scaleX;
		mScale.y = (scaleY == 0.f) ? scaleX : scaleY;
		mTransformDirty = true;
	}

	void Transform2DComponent::setOrigin(uint32_t typeFlags, const Vector2f& origin) noexcept
	{
		updateOrigins(origin, typeFlags, OriginFlag::Custom);
	}

	void Transform2DComponent::setOriginFlags(uint32_t typeFlags, uint32_t originFlags)
	{
		Collider2DComponent* const collider = mAssociatedActor.getComponent<Collider2DComponent>();
		if (collider) {
			const Box2f& modelBounds = collider->getModelBounds();
			Vector2f origin = modelBounds.min;
			if (originFlags & OriginFlag::CenterX) {
				origin.x += modelBounds.max.x / 2.f;
			}
			else if (originFlags & OriginFlag::Right) {
				origin.x += modelBounds.max.x;
			}

			if (originFlags & OriginFlag::CenterY) {
				origin.y += modelBounds.max.y / 2.f;
			}
			else if (originFlags & OriginFlag::Bottom) {
				origin.y += modelBounds.max.y;
			}

			// Update the origin and the origin flags
			updateOrigins(origin, typeFlags, originFlags);
		}
	}

	void Transform2DComponent::setRelativeAlignment(uint32_t flags)
	{
		// Find the closest ancestor with a collider component
		Collider2DComponent* ancestorCollider = nullptr;
		for (Actor* ancestor = mAssociatedActor.getParent(); ancestor != nullptr; ancestor = ancestor->getParent()) {
			ancestorCollider = ancestor->getComponent<Collider2DComponent>();
			if (ancestorCollider) {
				break;
			}
		}
		if (!ancestorCollider) {
			return;
		}

		// Align the actor based on the alignment flags
		const Box2f& ancestorBounds = ancestorCollider->getModelBounds();
		Vector2f alignedPos = ancestorBounds.min;;
		if (flags & OriginFlag::CenterX) {
			alignedPos.x += ancestorBounds.max.x / 2.f;
		}
		else if (flags & OriginFlag::Right) {
			alignedPos.x += ancestorBounds.max.x;
		}

		if (flags & OriginFlag::CenterY) {
			alignedPos.y += ancestorBounds.max.y / 2.f;
		}
		else if (flags & OriginFlag::Bottom) {
			alignedPos.y += ancestorBounds.max.y;
		}

		mAlignment.isAligned = true;
		mAlignment.flags = flags;
		if (mAlignment.position != alignedPos) {
			mAlignment.position = alignedPos;
			mTransformDirty = true;
		}
	}

	void Transform2DComponent::move(const Vector2f& offset) noexcept
	{
		mPosition.xy += offset;
		mTransformDirty = true;
	}

	void Transform2DComponent::move(float offsetX, float offsetY) noexcept
	{
		mPosition.x += offsetX;
		mPosition.y += offsetY;
		mTransformDirty = true;
	}

	void Transform2DComponent::rotate(float angle) noexcept
	{
		mRotation += angle;
		mTransformDirty = true;
	}

	void Transform2DComponent::scale(const Vector2f& scale) noexcept
	{
		mScale *= scale;
		mTransformDirty = true;
	}

	void Transform2DComponent::scale(float scaleX, float scaleY) noexcept
	{
		mScale.x *= scaleX;
		mScale.y *= (scaleY == 0.f) ? scaleX : scaleY;
		mTransformDirty = true;
	}

	void Transform2DComponent::lookat(const Vector2f& focus)
	{
		const Vector2f DELTA_POS = focus - mPosition.xy;
		setRotation(Math::atan2(DELTA_POS.y, DELTA_POS.y));
	}

	void Transform2DComponent::updateProperties()
	{
		setOriginFlags(OriginType::Translation, mTranslationOrigin.flags);
		setOriginFlags(OriginType::Rotation, mRotationOrigin.flags);
		setOriginFlags(OriginType::Scale, mScaleOrigin.flags);
		if (mAlignment.isAligned) {
			setRelativeAlignment(mAlignment.flags);
		}
	}

	const Matrix4f& Transform2DComponent::getTransform()
	{
		if (mTransformDirty || isDirty()) {
			// Calculate individual transformations
			Matrix4f rotation = Matrix4f::rotate(mRotation, Vector3f::ZAxis);
			if (mRotationOrigin.position.x != 0.f && mRotationOrigin.position.y != 0.f) {
				const Matrix4f POS_OFFSET = Matrix4f::translate(Vector3f(mRotationOrigin.position));
				const Matrix4f NEG_OFFSET = Matrix4f::translate(Vector3f(-mRotationOrigin.position));
				rotation = POS_OFFSET * rotation * NEG_OFFSET;
			}

			Matrix4f scale = Matrix4f::scale(Vector3f(mScale));
			if (mScaleOrigin.position.x != 0.f && mScaleOrigin.position.y != 0.f) {
				const Matrix4f POS_OFFSET = Matrix4f::translate(Vector3f(mScaleOrigin.position));
				const Matrix4f NEG_OFFSET = Matrix4f::translate(Vector3f(-mScaleOrigin.position));
				scale = POS_OFFSET * scale * NEG_OFFSET;
			}

			Matrix4f translation = Matrix4f::translate(mPosition + Vector3f(mAlignment.position) - Vector3f(mTranslationOrigin.position));

			// Calculate the model transform
			mTransform = translation * rotation * scale;
			mInvTransformDirty = std::exchange(mTransformDirty, false);

			// Raise the dirty flag indicating that the transform has been changed
			setDirty(true);
		}

		return mTransform;
	}

	const Matrix4f& Transform2DComponent::getInverseTransform()
	{
		if (mTransformDirty || mInvTransformDirty || isDirty()) {
			mInvTransform = getTransform().invert();
			mInvTransformDirty = false;
		}
		
		return mInvTransform;
	}

	const Vector2f& Transform2DComponent::getOrigin(uint32_t originType) const noexcept
	{
		switch (originType)
		{
		case OriginType::Rotation:
			return mRotationOrigin.position;
		case OriginType::Scale:
			return mScaleOrigin.position;
		default:
			return mTranslationOrigin.position;
		}
	}

	// Private method(s)
	void Transform2DComponent::updateOrigins(const Vector2f& pos, uint32_t typeFlags, uint32_t originFlags) noexcept
	{
		if (typeFlags & OriginType::Translation) {
			mTranslationOrigin.flags = originFlags;
			if (mTranslationOrigin.position != pos) {
				mTranslationOrigin.position = pos;
				mTransformDirty = true;
			}
		}
		if (typeFlags & OriginType::Rotation) {
			mRotationOrigin.flags = originFlags;
			if (mRotationOrigin.position != pos) {
				mRotationOrigin.position = pos;
				mTransformDirty = true;
			}
		}
		if (typeFlags & OriginType::Scale) {
			mScaleOrigin.flags = originFlags;
			if (mScaleOrigin.position != pos) {
				mScaleOrigin.position = pos;
				mTransformDirty = true;
			}
		}
	}
}