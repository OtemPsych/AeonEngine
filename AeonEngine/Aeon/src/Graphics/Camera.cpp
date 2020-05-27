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

#include <AEON/Graphics/Camera.h>

#include <AEON/Window/Window.h>
#include <AEON/Window/Application.h>

namespace ae
{
	// Public constructor(s)
	Camera::~Camera()
	{
	}

	// Public method(s)
	void Camera::setPosition(const Vector3f& position) noexcept
	{
		mPosition = position;
		mUpdateViewMatrix = true;
	}

	void Camera::setPosition(float posX, float posY, float posZ) noexcept
	{
		mPosition = Vector3f(posX, posY, posZ);
		mUpdateViewMatrix = true;
	}

	void Camera::setRotation(float angle, const Vector3f& axes) noexcept
	{
		mRotation = Quaternion::rotation(angle, axes);
		mUpdateViewMatrix = true;
	}

	void Camera::setFrustum(float nearPlane, float farPlane) noexcept
	{
		// Check if the nearPlane value is greater than the farPlane value or vice versa (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (nearPlane > farPlane && farPlane < nearPlane) {
				AEON_LOG_WARNING("Inverted plane frustum values", "The near plane value (\"" + std::to_string(nearPlane) + "\") and the far plane value (\"" + std::to_string(farPlane) + "\") may be inverted.");
			}
		}

		mNearPlane = nearPlane;
		mFarPlane = farPlane;
		mUpdateProjectionMatrix = true;
	}

	void Camera::setViewport(const Box2f& viewport) noexcept
	{
		// Check if the viewport coordinates are valid (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (viewport.min[0] < 0.f || viewport.min[1] < 0.f || viewport.max[0] < 0.f || viewport.max[1] < 0.f) {
				AEON_LOG_ERROR("Invalid viewport values", "One or more of the viewport values provided are less than 0.");
				return;
			}
			if (viewport.min[0] > 1.f || viewport.min[1] > 1.f || viewport.max[0] > 1.f || viewport.max[1] > 1.f) {
				AEON_LOG_ERROR("Invalid viewport values", "One or more of the viewport values provided are greater than 1.");
				return;
			}
			if (viewport.min[0] == viewport.max[0] || viewport.min[1] == viewport.max[1]) {
				AEON_LOG_ERROR("Invalid viewport values", "One or more minimum and maximum viewport values provided are equal.");
				return;
			}
		}

		mViewport = viewport;
	}

	void Camera::move(const Vector3f& offset) noexcept
	{
		mPosition += offset;
		mUpdateViewMatrix = true;
	}

	void Camera::move(float offsetX, float offsetY, float offsetZ) noexcept
	{
		mPosition += Vector3f(offsetX, offsetY, offsetZ);
		mUpdateViewMatrix = true;
	}

	void Camera::rotate(float angle, const Vector3f& axes) noexcept
	{
		// The 'getRotation()' method is used as derived classes calculate it differently
		mRotation = Quaternion::rotation(angle, axes) * getRotation();
		mUpdateViewMatrix = true;
	}

	void Camera::lookAt(const Vector3f& focus) noexcept
	{
		mViewMatrix = Matrix4f::lookat(mPosition, focus, Vector3f::Up);
		mUpdateViewMatrix = false;
		mUpdateInvViewMatrix = true;
	}

	void Camera::setTarget(const RenderTarget* const target) noexcept
	{
		mTarget = target;
	}

	Vector3f Camera::getLocalRight()
	{
		// The 'getRotation()' method is used as derived classes calculate it differently
		return getRotation().rotate(Vector3f::Right);
	}

	Vector3f Camera::getLocalUp()
	{
		// The 'getRotation()' method is used as derived classes calculate it differently
		return getRotation().rotate(Vector3f::Up);
	}

	Vector3f Camera::getLocalForward()
	{
		// The 'getRotation()' method is used as derived classes calculate it differently
		return getRotation().rotate(Vector3f::Forward);
	}
	
	const Vector3f& Camera::getPosition() const noexcept
	{
		return mPosition;
	}

	void Camera::getFrustum(float& nearPlane, float& farPlane) const noexcept
	{
		nearPlane = mNearPlane;
		farPlane = mFarPlane;
	}

	const Box2f& Camera::getViewport() const noexcept
	{
		return mViewport;
	}

	const Matrix4f& Camera::getInverseViewMatrix()
	{
		// Update the stored inverse view matrix if necessary
		if (mUpdateViewMatrix || mUpdateInvViewMatrix) {
			mInvViewMatrix = getViewMatrix().invert();
			mUpdateInvViewMatrix = false;
		}

		return mInvViewMatrix;
	}

	// Public virtual method(s)
	const Quaternion& Camera::getRotation()
	{
		return mRotation;
	}

	const Matrix4f& Camera::getViewMatrix()
	{
		// Update the stored view matrix if necessary
		if (mUpdateViewMatrix) {
			// Recalculate the local vectors pointing forwards, rightwards and upwards
			const Vector3f FORWARD = getLocalForward().normalize();
			const Vector3f RIGHT = cross(FORWARD, getLocalUp().normalize());
			const Vector3f UP = cross(RIGHT, FORWARD);

			// Update the view matrix
			mViewMatrix = Matrix4f::lookat(mPosition, mPosition + FORWARD, UP);
			mUpdateViewMatrix = false;
			mUpdateInvViewMatrix = true;
		}

		return mViewMatrix;
	}

	// Protected constructor(s)
	Camera::Camera(float nearPlane, float farPlane)
		: mViewMatrix()
		, mProjectionMatrix()
		, mRotation()
		, mUpdateViewMatrix(true)
		, mUpdateInvViewMatrix(true)
		, mUpdateProjectionMatrix(true)
		, mTarget(nullptr)
		, mInvViewMatrix()
		, mViewport(0.f, 0.f, 1.f, 1.f)
		, mPosition()
		, mNearPlane(nearPlane)
		, mFarPlane(farPlane)
	{
	}
}