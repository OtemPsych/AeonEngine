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

#include <AEON/Graphics/BasicRenderer2D.h>

#include <GL/glew.h>

#include <AEON/Graphics/internal/GLCommon.h>
#include <AEON/Graphics/internal/RenderTarget.h>
#include <AEON/Graphics/internal/VertexArray.h>
#include <AEON/Graphics/internal/VertexBuffer.h>
#include <AEON/Graphics/internal/IndexBuffer.h>
#include <AEON/Graphics/RenderStates.h>
#include <AEON/Graphics/Shader.h>
#include <AEON/Graphics/Texture2D.h>
#include <AEON/Graphics/Renderable2D.h>

namespace ae
{
	// Public virtual method(s)
	void BasicRenderer2D::beginScene(RenderTarget& target)
	{
		// Assign the new render target and uploads its camera's properties
		Renderer2D::beginScene(target);

		// Enable depth-testing, activate the render target and bind the VAO used for the drawcalls
		GLCall(glEnable(GL_DEPTH_TEST));
		mRenderTarget->activate();
		mVAO->bind();
	}

	void BasicRenderer2D::submit(const std::vector<Vertex2D>& vertices, const std::vector<unsigned int>& indices, const RenderStates& states)
	{
		// Check if the shader provided is valid (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (!states.shader) {
				AEON_LOG_WARNING("Null shader", "The shader provided is null.\nAborting rendering.");
				return;
			}
		}

		// Bind the shader provided
		states.shader->bind();

		// Set the appropriate blending
		if (states.blendMode != BlendMode::BlendNone) {
			GLCall(glEnable(GL_BLEND));
			GLCall(glBlendEquationSeparate(states.blendMode.colorEquation, states.blendMode.alphaEquation));
			GLCall(glBlendFuncSeparate(states.blendMode.colorSrcFactor, states.blendMode.colorDstFactor,
			                           states.blendMode.alphaSrcFactor, states.blendMode.alphaDstFactor));
		}
		else {
			GLCall(glDisable(GL_BLEND));
		}

		// Bind the texture provided or the 1x1 white texture for untextured geometry
		const Texture* const texture = (!states.texture) ? mWhiteTexture.get() : states.texture;
		texture->bind();

		// Apply the transform to the vertices
		std::vector<Vertex2D> transformVertices;
		transformVertices.reserve(vertices.size());
		for (const Vertex2D& vertex : vertices) {
			transformVertices.emplace_back(
				Vertex2D {
					Vector3f((states.transform * Vector3f(vertex.position.xy)).xy, vertex.position.z),
					vertex.color,
					vertex.uv
				}
			);
		}

		// Upload the vertices
		VertexBuffer* const vbo = mVAO->getVBO(0);
		vbo->setData(static_cast<int>(sizeof(Vertex2D) * transformVertices.size()), transformVertices.data());

		// Upload the indices
		IndexBuffer* const ibo = mVAO->getIBO();
		ibo->setData(sizeof(GLuint) * indices.size(), indices.data());

		// Render the geometry
		GLCall(glDrawElements(GL_TRIANGLES, ibo->getCount(), GL_UNSIGNED_INT, nullptr));

		// Unbind the texture
		texture->unbind();

		// Unbind the shader
		states.shader->unbind();
	}

	// Public static method(s)
	BasicRenderer2D& BasicRenderer2D::getInstance()
	{
		static BasicRenderer2D instance;
		return instance;
	}

	// Private constructor(s)
	BasicRenderer2D::BasicRenderer2D()
		: Renderer2D()
	{
	}
}