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

#include <AEON/Graphics/BatchRenderer2D.h>

#include <GL/glew.h>

#include <AEON/Graphics/Shader.h>
#include <AEON/Graphics/Texture2D.h>
#include <AEON/Graphics/internal/GLCommon.h>
#include <AEON/Graphics/internal/RenderTarget.h>
#include <AEON/Graphics/internal/VertexArray.h>
#include <AEON/Graphics/internal/IndexBuffer.h>

namespace ae
{
	// Public virtual method(s)
	void BatchRenderer2D::endScene()
	{
		mRenderTarget->activate();
		mVAO->bind();

		flush();
		mSubmissions.clear();

		Renderer2D::endScene();
	}

	void BatchRenderer2D::submit(const std::vector<Vertex2D>& vertices, const std::vector<unsigned int>& indices, const RenderStates& states)
	{
		if (vertices.empty() || indices.empty()) {
			return;
		}

		mSubmissions.emplace_back(SubmissionData{ states, &vertices, &indices });
	}

	void BatchRenderer2D::submit(const Render2DComponent& component, const RenderStates& states)
	{
		submit(component.getVertices(), component.getIndices(), states);
	}

	// Public static method(s)
	BatchRenderer2D& BatchRenderer2D::getInstance()
	{
		static BatchRenderer2D instance;
		return instance;
	}

	// Private constructor(s)
	BatchRenderer2D::BatchRenderer2D()
		: Renderer2D()
		, mSubmissions()
		, mIndexOffset(0)
	{
	}

	// Private method(s)
	void BatchRenderer2D::flush()
	{
		RenderStates currentStates;

		for (size_t i = 0; i < mSubmissions.size(); ++i) {
			RenderStates& states = mSubmissions[i].states;

			// Bind the shader
			if (currentStates.shader != states.shader) {
				states.shader->bind();
				currentStates.shader = states.shader;
			}

			// Set the blending
			if (currentStates.blendMode != states.blendMode) {
				if (states.blendMode != BlendMode::BlendNone) {
					GLCall(glEnable(GL_BLEND));
					GLCall(glBlendEquationSeparate(states.blendMode.colorEquation, states.blendMode.alphaEquation));
					GLCall(glBlendFuncSeparate(states.blendMode.colorSrcFactor, states.blendMode.colorDstFactor,
					                           states.blendMode.alphaSrcFactor, states.blendMode.alphaDstFactor));
				}
				else {
					GLCall(glDisable(GL_BLEND));
				}
				currentStates.blendMode = states.blendMode;
			}

			// Bind texture
			if (currentStates.texture != states.texture) {
				const Texture* const texture = (!states.texture) ? mWhiteTexture.get() : states.texture;
				texture->bind();
				currentStates.texture = states.texture;
			}

			// Batch submission
			std::vector<Vertex2D> batchVertices;
			std::vector<uint32_t> batchIndices;
			mIndexOffset = 0;
			batchSubmission(mSubmissions[i], batchVertices, batchIndices);

			// Attempt to batch more submissions
			for (size_t j = i + 1; j < mSubmissions.size(); ++j) {
				if (isSubmissionBatchable(states, mSubmissions[j].states)) {
					batchSubmission(mSubmissions[j], batchVertices, batchIndices);
					++i;
				}
				else {
					break;
				}
			}

			// Upload the vertices
			VertexBuffer* const vbo = mVAO->getVBO(0);
			vbo->setData(static_cast<int64_t>(sizeof(Vertex2D) * batchVertices.size()), batchVertices.data());

			// Upload the indices
			IndexBuffer* const ibo = mVAO->getIBO();
			ibo->setData(static_cast<int64_t>(sizeof(GLuint) * batchIndices.size()), batchIndices.data());

			// Render the geometry
			GLCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(ibo->getCount()), GL_UNSIGNED_INT, nullptr));
		}
	}

	bool BatchRenderer2D::isSubmissionBatchable(const RenderStates& currentStates, const RenderStates& newStates)
	{
		return currentStates.shader == newStates.shader && currentStates.blendMode == newStates.blendMode && currentStates.texture == newStates.texture;
	}

	void BatchRenderer2D::batchSubmission(const SubmissionData& submission, std::vector<Vertex2D>& vertices, std::vector<uint32_t>& indices)
	{
		// Apply the transform to the vertices
		vertices.reserve(vertices.size() + submission.vertices->size());
		for (const Vertex2D& vertex : *submission.vertices) {
			vertices.emplace_back(
				Vertex2D {
					Vector3f((submission.states.transform * Vector4f(vertex.position, 1.f)).xy, vertex.position.z),
					vertex.color,
					vertex.uv
				}
			);
		}

		// Add in the indices
		indices.reserve(indices.size() + submission.indices->size());
		uint32_t maxIndex = 0;
		for (uint32_t index : *submission.indices) {
			indices.emplace_back(index + mIndexOffset);
			if (index > maxIndex) {
				maxIndex = index;
			}
		}
		mIndexOffset += maxIndex + 1;
	}
}