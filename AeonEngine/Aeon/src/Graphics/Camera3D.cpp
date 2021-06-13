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

#include <AEON/Graphics/Camera3D.h>

#include <AEON/Window/Window.h>

namespace ae
{
	// Public constructor(s)
	Camera3D::Camera3D(float nearPlane, float farPlane, float fov)
		: Camera(nearPlane, farPlane)
		, mTargetSize()
		, mFOV(fov)
	{
	}

	Camera3D::~Camera3D()
	{
	}

	// Public method(s)
	void Camera3D::setFOV(float fov) noexcept
	{
		mFOV = fov;
		mUpdateProjectionMatrix = true;
	}

	float Camera3D::getFOV() const noexcept
	{
		return mFOV;
	}

	// Public virtual method(s)
	const Matrix4f& Camera3D::getProjectionMatrix()
	{
		// Update the internally-saved framebuffer size in case it's been modified
		updateInternalFrameSize();

		// Update the perspective projection matrix if necessary
		if (mUpdateProjectionMatrix) {
			// Calculate the half of the framebuffer's size and retrieve the near and far plane distances
			const Vector2f HALF_FRAME_SIZE = mTargetSize / 2.f;
			float nearPlane, farPlane;
			getFrustum(nearPlane, farPlane);

			// Update the projection matrix
			mProjectionMatrix = Matrix4f::perspective(mFOV, HALF_FRAME_SIZE.x / HALF_FRAME_SIZE.y, nearPlane, farPlane);
			mUpdateInvProjectionMatrix = std::exchange(mUpdateProjectionMatrix, false);
		}

		return mProjectionMatrix;
	}

	// Protected method(s)
	const Vector2f& Camera3D::updateInternalFrameSize() noexcept
	{
		const Vector2f FRAME_SIZE = (mTarget) ? static_cast<Vector2f>(mTarget->getFramebufferSize()) : Vector2f();
		if (mTargetSize != FRAME_SIZE) {
			mTargetSize = FRAME_SIZE;
			mUpdateProjectionMatrix = true;
		}

		return mTargetSize;
	}
}