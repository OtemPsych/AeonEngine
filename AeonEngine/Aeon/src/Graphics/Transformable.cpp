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

#include <AEON/Graphics/Transformable.h>

namespace ae
{
	// Public constructor(s)
	Transformable::~Transformable()
	{
	}

	// Public method(s)
	void Transformable::setPosition(const Vector3f& position) noexcept
	{
		mPosition = position;
		mUpdateTransform = true;
	}

	void Transformable::setRotation(float angle, const Vector3f& axes) noexcept
	{
		mRotation = Quaternion::rotation(angle, axes);
		mUpdateTransform = true;
	}

	void Transformable::setScale(const Vector3f& scale) noexcept
	{
		mScale = scale;
		mUpdateTransform = true;
	}

	void Transformable::setOrigin(uint32_t flags) noexcept
	{
		const Box3f BOUNDS = getModelBounds();
		if (flags == OriginFlag::Center) {
			setOrigin(BOUNDS.min + BOUNDS.max / 2.f);
		}
		else {
			Vector3f newOrigin(BOUNDS.min);
			if (flags & OriginFlag::CenterX) {
				newOrigin.x = BOUNDS.min.x + BOUNDS.max.x / 2.f;
			}
			else if (flags & OriginFlag::Right) {
				newOrigin.x = BOUNDS.min.x + BOUNDS.max.x;
			}

			if (flags & OriginFlag::CenterY) {
				newOrigin.y = BOUNDS.min.y + BOUNDS.max.y / 2.f;
			}
			else if ((flags & OriginFlag::Bottom && BOUNDS.max.z == 0.f) || (flags & OriginFlag::Top && BOUNDS.max.z != 0.f)) {
				newOrigin.y = BOUNDS.min.y + BOUNDS.max.y;
			}

			if (flags & OriginFlag::CenterZ) {
				newOrigin.z = BOUNDS.min.z + BOUNDS.max.z / 2.f;
			}
			else if (flags & OriginFlag::Back) {
				newOrigin.z = BOUNDS.min.z + BOUNDS.max.z;
			}

			setOrigin(newOrigin);
		}
	}

	void Transformable::move(const Vector3f& offset) noexcept
	{
		mPosition += offset;
		mUpdateTransform = true;
	}

	void Transformable::rotate(float angle, const Vector3f& axes) noexcept
	{
		mRotation = Quaternion::rotation(angle, axes) * mRotation;
		mUpdateTransform = true;
	}

	void Transformable::scale(const Vector3f& scale) noexcept
	{
		mScale *= scale;
		mUpdateTransform = true;
	}

	void Transformable::lookat(const Vector3f& focus)
	{
		mTransform = Matrix4f::lookat(mPosition, focus, Vector3f::Up);
		mUpdateTransform = false;
		mUpdateInvTransform = true;
	}

	const Matrix4f& Transformable::getTransform()
	{
		// Update the model transform if necessary
		if (mUpdateTransform) {
			const Matrix4f ORIGIN_ROTATION = (mOrigin == Vector3f(0.f)) ? Matrix4f::rotate(mRotation)
			                                                            : Matrix4f::translate(mOrigin) * Matrix4f::rotate(mRotation) * Matrix4f::translate(-mOrigin);
			mTransform = Matrix4f::translate(mPosition) * ORIGIN_ROTATION * Matrix4f::scale(mScale);
			mUpdateTransform = false;
			mUpdateInvTransform = true;
		}

		return mTransform;
	}

	const Matrix4f& Transformable::getInverseTransform()
	{
		// Update the inverse model transform if necessary
		if (mUpdateTransform || mUpdateInvTransform) {
			mInvTransform = getTransform().invert();
			mUpdateInvTransform = false;
		}

		return mInvTransform;
	}

	const Vector3f& Transformable::getPosition() const noexcept
	{
		return mPosition;
	}

	const Quaternion& Transformable::getRotation() const noexcept
	{
		return mRotation;
	}

	const Vector3f& Transformable::getScale() const noexcept
	{
		return mScale;
	}

	const Vector3f& Transformable::getOrigin() const noexcept
	{
		return mOrigin;
	}

	Box3f Transformable::getWorldBounds()
	{
		const Box3f MODEL_BOUNDS = getModelBounds();
		const Matrix4f& worldTransform = getTransform();

		return Box3f(worldTransform * MODEL_BOUNDS.min, worldTransform * MODEL_BOUNDS.max);
	}

	// Public virtual method(s)
	void Transformable::setOrigin(const Vector3f& origin) noexcept
	{
		mOrigin = origin;
		mUpdateTransform = true;
	}

	// Protected constructor(s)
	Transformable::Transformable() noexcept
		: mTransform(1.f)
		, mInvTransform(1.f)
		, mRotation()
		, mPosition()
		, mScale(1.f, 1.f, 1.f)
		, mOrigin(0.f, 0.f, 0.f)
		, mUpdateTransform(false)
		, mUpdateInvTransform(false)
	{
	}
}