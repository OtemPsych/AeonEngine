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

#include <AEON/Graphics/Camera2D.h>

#include <AEON/Window/Window.h>

namespace ae
{
	// Public constructor(s)
	Camera2D::Camera2D(float nearPlane, float farPlane)
		: Camera(nearPlane, farPlane)
		, mZoomFactor(1.f)
	{
	}

	// Public method(s)
	void Camera2D::zoom(float factor)
	{
		mZoomFactor *= factor;
		mUpdateProjectionMatrix = true;
	}

	void Camera2D::zoomAt(const Vector2f& pixel, float factor)
	{
		// Check that a camera has been assigned to a render target (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (!mTarget) {
				AEON_LOG_ERROR("Unassigned camera", "The camera instance hasn't been assigned to a render target.");
				return;
			}
		}

		// Retrieve the corresponding world coordinates of the pixel prior to performing the zoom
		const ae::Vector2f BEFORE_COORD = mTarget->mapPixelToCoords(pixel);

		// Perform the zoom
		zoom(factor);

		// Move the camera based on the prior and current world coordinates of the zoom
		move(Vector3f(BEFORE_COORD - mTarget->mapPixelToCoords(pixel), 0.f));
	}

	// Public virtual method(s)
	const Matrix4f& Camera2D::getProjectionMatrix()
	{
		// Update the orthographic projection matrix is necessary
		if (mUpdateProjectionMatrix) {
			// Retrieve the render target's size and the near and far plane distances
			const Vector2f FRAME_SIZE = static_cast<Vector2f>(mTarget->getFramebufferSize()) * mZoomFactor;
			float nearPlane, farPlane;
			getFrustum(nearPlane, farPlane);

			// Update the projection matrix
			mProjectionMatrix = Matrix4f::orthographic(0.f, FRAME_SIZE.x, FRAME_SIZE.y, 0.f, nearPlane, farPlane);
			mUpdateProjectionMatrix = false;
		}

		return mProjectionMatrix;
	}
}