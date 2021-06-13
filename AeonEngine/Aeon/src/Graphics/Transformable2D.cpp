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

#include <AEON/Graphics/Transformable2D.h>

namespace ae
{
	// Public method(s)
	void Transformable2D::setPosition(const Vector2f& position, int zIndex) noexcept
	{
		mPosition.xy = position;
		mUpdateTransform = true;

		// Only modify the z position if it was manually set
		if (zIndex != INT_MAX) {
			mPosition.z = static_cast<float>(zIndex);
		}
	}

	void Transformable2D::setPosition(float posX, float posY, int zIndex) noexcept
	{
		mPosition.x = posX;
		mPosition.y = posY;
		mUpdateTransform = true;

		// Only modify the z position if it was manually set
		if (zIndex != INT_MAX) {
			mPosition.z = static_cast<float>(zIndex);
		}
	}

	void Transformable2D::setRotation(float angle) noexcept
	{
		mRotation = angle;
		mUpdateTransform = true;
	}

	void Transformable2D::setScale(const Vector2f& scale) noexcept
	{
		mScale = scale;
		mUpdateTransform = true;
	}

	void Transformable2D::setScale(float scaleX, float scaleY) noexcept
	{
		mScale.x = scaleX;
		mScale.y = scaleY;
		mUpdateTransform = true;
	}

	void Transformable2D::setOriginFlags(uint32_t flags)
	{
		const Box2f BOUNDS = getModelBounds();
		if (flags == OriginFlag::Center) {
			setOrigin(BOUNDS.min + BOUNDS.max / 2.f);
		}
		else {
			Vector2f newOrigin(BOUNDS.min);
			if (flags & OriginFlag::CenterX) {
				newOrigin.x += BOUNDS.max.x / 2.f;
			}
			else if (flags & OriginFlag::Right) {
				newOrigin.x += BOUNDS.max.x;
			}

			if (flags & OriginFlag::CenterY) {
				newOrigin.y += BOUNDS.max.y / 2.f;
			}
			else if (flags & OriginFlag::Bottom) {
				newOrigin.y += BOUNDS.max.y;
			}

			setOrigin(newOrigin);
		}

		mOriginFlags = flags;
	}

	void Transformable2D::move(const Vector2f& offset) noexcept
	{
		mPosition.xy += offset;
		mUpdateTransform = true;
	}

	void Transformable2D::move(float offsetX, float offsetY) noexcept
	{
		mPosition.x += offsetX;
		mPosition.y += offsetY;
		mUpdateTransform = true;
	}

	void Transformable2D::rotate(float angle) noexcept
	{
		mRotation += angle;
		mUpdateTransform = true;
	}

	void Transformable2D::scale(const Vector2f& scale) noexcept
	{
		mScale *= scale;
		mUpdateTransform = true;
	}

	void Transformable2D::scale(float scaleX, float scaleY) noexcept
	{
		mScale.x *= scaleX;
		mScale.y *= scaleY;
		mUpdateTransform = true;
	}

	void Transformable2D::lookat(const Vector2f& focus)
	{
		const Vector2f deltaPos = focus - mPosition.xy;
		setRotation(Math::toDegrees(Math::atan2(deltaPos.y, deltaPos.x)));
	}

	void Transformable2D::setOrigin(const Vector2f& origin) noexcept
	{
		mOrigin = origin;
		mUpdateTransform = true;
	}

	const Matrix4f& Transformable2D::getTransform()
	{
		// Update the model transform if necessary
		if (mUpdateTransform) {
			// Calculate the base rotation and scale
			Matrix4f rotation = Matrix4f::rotate(Math::toRadians(mRotation), Vector3f::ZAxis);
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
			mUpdateInvTransform = std::exchange(mUpdateTransform, false);
		}

		return mTransform;
	}

	const Matrix4f& Transformable2D::getInverseTransform()
	{
		// Update the inverse model transform if necessary
		if (mUpdateTransform || mUpdateInvTransform) {
			mInvTransform = getTransform().invert();
			mUpdateInvTransform = false;
		}

		return mInvTransform;
	}

	const Vector3f& Transformable2D::getPosition() const noexcept
	{
		return mPosition;
	}

	float Transformable2D::getRotation() const noexcept
	{
		return mRotation;
	}

	const Vector2f& Transformable2D::getScale() const noexcept
	{
		return mScale;
	}

	const Vector2f& Transformable2D::getOrigin() const noexcept
	{
		return mOrigin;
	}

	Box2f Transformable2D::getWorldBounds()
	{
		const Box2f MODEL_BOUNDS = getModelBounds();
		const Matrix4f& transform = getTransform();

		return Box2f(Vector2f(transform * Vector3f(MODEL_BOUNDS.min)), Vector2f(transform * Vector3f(MODEL_BOUNDS.max)));
	}

	// Protected constructor(s)
	Transformable2D::Transformable2D() noexcept
		: mTransform(1.f)
		, mInvTransform(1.f)
		, mPosition(0.f, 0.f, 0.f)
		, mScale(1.f, 1.f)
		, mOrigin(0.f, 0.f)
		, mRotation(0.f)
		, mOriginFlags(OriginFlag::Left | OriginFlag::Top)
		, mUpdateTransform(false)
		, mUpdateInvTransform(false)
	{
	}

	// Protected virtual method(s)
	void Transformable2D::correctProperties()
	{
		setOriginFlags(mOriginFlags);
	}
}