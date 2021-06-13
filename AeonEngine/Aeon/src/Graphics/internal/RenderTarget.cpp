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

#include <AEON/Graphics/internal/RenderTarget.h>

#include <GL/glew.h>

#include <AEON/Window/Application.h>
#include <AEON/Graphics/internal/GLCommon.h>
#include <AEON/Graphics/Color.h>

namespace ae
{
	// Private static member(s)
	const RenderTarget* RenderTarget::activeTarget = nullptr;
	float RenderTarget::depthValue = 1.f;

	// Public constructor(s)
	RenderTarget::~RenderTarget()
	{
	}

	// Public method(s)
	void RenderTarget::clear()
	{
		// Check if the caller is the active render target (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (activeTarget != this) {
				AEON_LOG_WARNING("Invalid render target", "The currently active render target isn't the caller render target.");
			}
		}

		// Clear the color and depth buffers
		unsigned int fboHandle = getFramebufferHandle();
		//GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
		GLCall(glClearNamedFramebufferfv(fboHandle, GL_COLOR, 0, mClearColor.elements.data()));
		GLCall(glClearNamedFramebufferfi(fboHandle, GL_DEPTH_STENCIL, 0, depthValue, 0));
	}

	void RenderTarget::activate()
	{
		if (activeTarget != this) {
			activeTarget = this;

			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, getFramebufferHandle()));
			GLCall(glViewport(0, 0, mFramebufferSize.x, mFramebufferSize.y));
		}
		clear();
	}

	void RenderTarget::setClearColor(const Color& color)
	{
		mClearColor = color.normalize();
	}

	Vector2f RenderTarget::mapPixelToCoords(const Vector2f& pixel) const
	{
		// Check that a camera has been assigned to the render target (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (!mCamera) {
				AEON_LOG_ERROR("Unassigned camera", "No camera instance has been assigned to this render target.");
				return Vector2f();
			}
		}

		// Convert the pixel to homogeneous coordinates
		const Box2i VIEWPORT = getViewport();
		const Vector2f NDC(-1.f + 2.f * (pixel.x - VIEWPORT.min.x) / VIEWPORT.max.x,
		                    1.f - 2.f * (pixel.y - VIEWPORT.min.y) / VIEWPORT.max.y);

		// Transform the homogeneous coordinates by the inverse of the view and the inverse of the projection matrices
		return Vector2f(mCamera->getInverseViewMatrix() * mCamera->getInverseProjectionMatrix() * Vector3f(NDC));
	}

	Vector2f RenderTarget::mapCoordsToPixel(const Vector2f& point) const
	{
		// Check that a camera has been assigned to the render target (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (!mCamera) {
				AEON_LOG_ERROR("Unassigned camera", "No camera instance has been assigned to this render target.");
				return Vector2f();
			}
		}

		// Convert the point to target coordinates
		const Vector2f TRANS_POINT(mCamera->getProjectionMatrix() * Vector3f(point));
		const Box2f VIEWPORT = getViewport();

		return Vector2f((TRANS_POINT.x + 1.f) / 2.f * VIEWPORT.max.x + VIEWPORT.min.x,
		                (-TRANS_POINT.y + 1.f) / 2.f * VIEWPORT.max.y + VIEWPORT.min.y);
	}

	Box2f RenderTarget::getViewport() const
	{
		// Check that a camera has been assigned to the render target (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (!mCamera) {
				AEON_LOG_ERROR("Unassigned camera", "No camera instance has been assigned to this render target.");
				return Box2f();
			}
		}

		// Calculate the viewport applied to the render target
		const Box2f& viewport = mCamera->getViewport();
		const Vector2f SIZE = Vector2f(mFramebufferSize);

		return Box2f(SIZE * viewport.min + 0.5f, SIZE * viewport.max + 0.5f);
	}

	Camera* RenderTarget::getCamera()
	{
		// Checks if the camera pointer is valid (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (!mCamera) {
				AEON_LOG_ERROR("Invalid camera", "The render target's camera hasn't been set.");
				return nullptr;
			}
		}

		return (mCamera) ? mCamera.get() : nullptr;
	}

	const Vector2i& RenderTarget::getFramebufferSize() const noexcept
	{
		return mFramebufferSize;
	}

	// Public virtual method(s)
	unsigned int RenderTarget::getFramebufferHandle() const noexcept
	{
		return 0;
	}

	// Protected constructor(s)
	RenderTarget::RenderTarget() noexcept
		: mFramebufferSize(0, 0)
		, mClearColor(Color::Black.normalize())
		, mCamera(nullptr)
	{
	}

	RenderTarget::RenderTarget(RenderTarget&& rvalue) noexcept
		: mFramebufferSize(std::move(rvalue.mFramebufferSize))
		, mClearColor(std::move(rvalue.mClearColor))
		, mCamera(std::move(rvalue.mCamera))
	{
	}

	// Protected operator(s)
	RenderTarget& RenderTarget::operator=(RenderTarget&& rvalue) noexcept
	{
		// Move the rvalue's data
		mFramebufferSize = std::move(rvalue.mFramebufferSize);
		mClearColor = std::move(rvalue.mClearColor);
		mCamera = std::move(rvalue.mCamera);

		return *this;
	}
}