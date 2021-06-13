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

#include <AEON/Graphics/CameraFPS.h>

#include <AEON/Window/Mouse.h>

namespace ae
{
	// Public constructor(s)
	CameraFPS::CameraFPS(float nearPlane, float farPlane, float fov, float sensitivity)
		: Camera3D(nearPlane, farPlane, fov)
		, mLastMousePos()
		, mSensitivity(sensitivity)
	{
		// Hide and lock the mouse cursor to the active window
		Mouse::grabMouse(true);
	}

	// Public method(s)
	void CameraFPS::setSensitivity(float sensitivity) noexcept
	{
		mSensitivity = sensitivity;
	}

	float CameraFPS::getSensitivity() const noexcept
	{
		return mSensitivity;
	}

	// Public virtual method(s)
	const Quaternion& CameraFPS::getRotation()
	{
		// Only modify the rotation if the mouse is locked to the window
		if (Mouse::isMouseGrabbed()) {
			// Retrieve the current mouse position
			const Vector2f& STORED_FRAME_SIZE = updateInternalFrameSize();
			const Vector2f MOUSE_POS = static_cast<Vector2f>(Mouse::getPosition()) - (STORED_FRAME_SIZE / 2.f);

			// Update the rotation if the mouse cursor is no longer in the same position
			if (mLastMousePos != MOUSE_POS) {
				mLastMousePos = MOUSE_POS;

				const float YAW = MOUSE_POS.x * mSensitivity;
				const float PITCH = MOUSE_POS.y * mSensitivity;

				mRotation = (Quaternion::rotationY(-YAW) * Quaternion::rotationX(-PITCH)).normalize();
				mUpdateViewMatrix = true;
			}
		}

		return mRotation;
	}

	const Matrix4f& CameraFPS::getViewMatrix()
	{
		// Update the stored view matrix if necessary
		if (mUpdateViewMatrix) {
			mViewMatrix = Matrix4f::rotate(getRotation()) * Matrix4f::translate(-getPosition());
			mUpdateInvViewMatrix = std::exchange(mUpdateViewMatrix, false);
		}

		return mViewMatrix;
	}
}