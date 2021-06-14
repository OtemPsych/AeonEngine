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

#include <AEON/Graphics/Transform2DComponent.h>

namespace ae
{
	// Public constructor(s)
	Transform2DComponent::Transform2DComponent() noexcept
		: mTransform(Matrix4f::identity())
		, mInvTransform(Matrix4f::identity())
		, mPosition(0.f, 0.f, 0.f)
		, mScale(1.f, 1.f)
		, mOrigin(0.f, 0.f)
		, mRotation(0.f)
		, mTransformDirty(false)
		, mInvTransformDirty(false)
	{
	}

	// Public method(s)
	void Transform2DComponent::setPosition(const Vector2f& position, int zIndex) noexcept
	{
		mPosition.xy = position;
		mTransformDirty = true;

		if (zIndex != INT_MAX) {
			mPosition.z = static_cast<float>(zIndex);
		}
	}

	void Transform2DComponent::setPosition(float posX, float posY, int zIndex) noexcept
	{
		mPosition.x = posX;
		mPosition.y = posY;
		mTransformDirty = true;

		if (zIndex != INT_MAX) {
			mPosition.z = static_cast<float>(zIndex);
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

	void Transform2DComponent::setOrigin(const Vector2f& origin) noexcept
	{
		mOrigin = origin;
		mTransformDirty = true;
	}

	const Matrix4f& Transform2DComponent::getTransform()
	{
		if (mTransformDirty) {
			// Calculate the base rotation and scale
			Matrix4f rotation = Matrix4f::rotate(mRotation, Vector3f::ZAxis);
			Matrix4f scale = Matrix4f::scale(Vector3f(mScale));

			// Calculate the final rotation and scale based on the origin
			if (mOrigin.x != 0.f && mOrigin.y != 0.f) {
				const Matrix4f ORIGIN_POS_OFFSET = Matrix4f::translate(Vector3f(mOrigin));
				const Matrix4f ORIGIN_NEG_OFFSET = Matrix4f::translate(Vector3f(-mOrigin));

				rotation = ORIGIN_POS_OFFSET * rotation * ORIGIN_NEG_OFFSET;
				scale = ORIGIN_POS_OFFSET * scale * ORIGIN_NEG_OFFSET;
			}

			// Calculate the model transform
			mTransform = Matrix4f::translate(mPosition - Vector3f(mOrigin)) * rotation * scale;
			mInvTransformDirty = std::exchange(mTransformDirty, false);
		}

		return mTransform;
	}

	const Matrix4f& Transform2DComponent::getInverseTransform()
	{
		if (mTransformDirty || mInvTransformDirty) {
			mInvTransform = getTransform().invert();
			mInvTransformDirty = false;
		}
		
		return mInvTransform;
	}
}