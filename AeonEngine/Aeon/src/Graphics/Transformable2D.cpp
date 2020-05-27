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

#include <AEON/Graphics/Transformable2D.h>

namespace ae
{
	// Public constructor(s)
	Transformable2D::~Transformable2D()
	{
	}

	// Public method(s)
	void Transformable2D::setPosition(const Vector2f& position) noexcept
	{
		mPosition = position;
		mUpdateTransform = true;
	}

	void Transformable2D::setPosition(float posX, float posY) noexcept
	{
		mPosition.x = posX;
		mPosition.y = posY;
		mUpdateTransform = true;
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
	}

	void Transformable2D::move(const Vector2f& offset) noexcept
	{
		mPosition += offset;
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
		const Vector2f deltaPos = focus - mPosition;
		setRotation(Math::toDegrees(Math::atan2(deltaPos.y, deltaPos.x)));
	}

	const Matrix4f& Transformable2D::getTransform()
	{
		// Update the model transform if necessary
		if (mUpdateTransform) {
			// Calculate the rotation based on the origin
			Matrix4f rotation = Matrix4f::rotate(Math::toRadians(mRotation), Vector3f::ZAxis);
			if (mOrigin != Vector2f()) {
				rotation = Matrix4f::translate(Vector3f(mOrigin)) * rotation * Matrix4f::translate(Vector3f(-mOrigin));
			}

			// Calculate the model transform
			mTransform = Matrix4f::translate(Vector3f(mPosition)) * rotation * Matrix4f::scale(Vector3f(mScale));
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

	const Vector2f& Transformable2D::getPosition() const noexcept
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

	// Public virtual method(s)
	void Transformable2D::setOrigin(const Vector2f& origin) noexcept
	{
		mOrigin = origin;
		mUpdateTransform = true;
	}

	// Protected constructor(s)
	Transformable2D::Transformable2D() noexcept
		: mTransform(1.f)
		, mInvTransform(1.f)
		, mPosition(0.f, 0.f)
		, mScale(1.f, 1.f)
		, mOrigin(0.f, 0.f)
		, mRotation(0.f)
		, mUpdateTransform(false)
		, mUpdateInvTransform(false)
	{
	}
}