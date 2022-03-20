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

#include <AEON/Graphics/internal/Renderer2D.h>

#include <GL/glew.h>

#include <AEON/Graphics/internal/GLCommon.h>
#include <AEON/Graphics/internal/RenderTarget.h>
#include <AEON/Graphics/Camera.h>
#include <AEON/Graphics/GLResourceFactory.h>
#include <AEON/Graphics/Render2DComponent.h>

namespace ae
{
	// Private static member(s)
	Renderer2D* Renderer2D::activeInstance = nullptr;

	// Public method(s)
	void Renderer2D::submit(const Render2DComponent& renderComponent, const RenderStates& states)
	{
		// Check if an inactive renderer received a submission (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (this != activeInstance) {
				AEON_LOG_WARNING("Invalid render submission", "A submission was received by a renderer other than the active one.\nAborting submission.");
				return;
			}
		}

		submit(renderComponent.getVertices(), renderComponent.getIndices(), states);
	}

	// Public virtual method(s)
	void Renderer2D::beginScene(RenderTarget& target)
	{
		// Check if there is already an active renderer (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (activeInstance) {
				AEON_LOG_WARNING("Attempt to overwrite active renderer", "A renderer can't begin a scene when another is currently active.\nAborting operation.");
				return;
			}
		}

		// Set the caller renderer as the active renderer
		activeInstance = this;

		// Assign the new render target for this scene
		mRenderTarget = &target;

		// Retrieve the camera's matrices
		Camera* const camera = mRenderTarget->getCamera();
		const Matrix4f& viewMatrix = camera->getViewMatrix();
		const Matrix4f& projMatrix = camera->getProjectionMatrix();

		// Upload the camera's properties to the UBO
		mTransformUBO->queueUniformUpload("view", viewMatrix.getElements().data(), sizeof(viewMatrix));
		mTransformUBO->queueUniformUpload("projection", projMatrix.getElements().data(), sizeof(projMatrix));
		mTransformUBO->queueUniformUpload("viewProjection", (projMatrix * viewMatrix).getElements().data(), sizeof(Matrix4f));
		mTransformUBO->uploadQueuedUniforms();
	}

	void Renderer2D::endScene()
	{
		// Check if the caller renderer isn't the active renderer (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (this != activeInstance) {
				AEON_LOG_WARNING("Invalid scene termination", "The scene can only be terminated by the renderer that started it.\nAborting operation.");
				return;
			}
		}

		// Unbinds the VAO used for the drawcalls, and disables depth-testing and blending
		mVAO->unbind();
		GLCall(glDisable(GL_DEPTH_TEST));
		GLCall(glDisable(GL_BLEND));

		// Invalidate the pointer to the render target and to the active renderer
		mRenderTarget = nullptr;
		activeInstance = nullptr;
	}

	// Public static method(s)
	Renderer2D* const Renderer2D::getActiveInstance() noexcept
	{
		return activeInstance;
	}

	// Protected constructor(s)
	Renderer2D::Renderer2D()
		: mWhiteTexture(GLResourceFactory::getInstance().get<Texture2D>("_AEON_WhiteTexture"))
		, mVAO(GLResourceFactory::getInstance().get<VertexArray>("_AEON_VAO"))
		, mRenderTarget(nullptr)
		, mTransformUBO(GLResourceFactory::getInstance().get<UniformBuffer>("_AEON_TransformUBO"))
	{
	}
}