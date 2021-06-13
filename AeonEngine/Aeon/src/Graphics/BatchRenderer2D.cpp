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

#include <AEON/Graphics/BatchRenderer2D.h>

#include <GL/glew.h>

#include <AEON/Graphics/internal/GLCommon.h>
#include <AEON/Graphics/internal/RenderTarget.h>
#include <AEON/Graphics/BlendMode.h>
#include <AEON/Graphics/Camera.h>
#include <AEON/Graphics/GLResourceFactory.h>
#include <AEON/Graphics/Renderable2D.h>
#include <AEON/Graphics/RenderStates.h>

namespace ae
{
	// Public virtual method(s)
	void BatchRenderer2D::endScene()
	{
		GLCall(glEnable(GL_DEPTH_TEST));
		mRenderTarget->activate();
		mVAO->bind();

		// Render opaque entities front-to-back
		flush(mOpaqueCalls, true);

		// Render transparent entities back-to-front
		flush(mTransparentCalls, false);

		// Unbind the VAO, disable depth-testing and invalidate scene-specific pointers
		Renderer2D::endScene();
	}

	void BatchRenderer2D::submit(const std::vector<Vertex2D>& vertices, const std::vector<unsigned int>& indices, const RenderStates& states)
	{
		// Check if the submission is opaque or transparent
		ShaderPasses& drawcalls = (vertices.front().color.w < 1.f) ? mTransparentCalls : mOpaqueCalls;

		// Find an existing shader pass or create one
		auto shaderItr = drawcalls.find(states.shader);
		if (shaderItr == drawcalls.end()) {
			shaderItr = drawcalls.emplace(states.shader, BlendPasses()).first;
		}

		// Find an existing blend pass or create one
		BlendPasses& blendPasses = shaderItr->second;
		auto blendItr = blendPasses.find(states.blendMode);
		if (blendItr == blendPasses.end()) {
			blendItr = blendPasses.emplace(states.blendMode, TexturePasses()).first;
		}

		// Find an existing texture pass or create one
		TexturePasses& texturePasses = blendItr->second;
		const Texture* const texture = (!states.texture) ? mWhiteTexture.get() : states.texture;
		auto textureItr = texturePasses.find(texture);
		if (textureItr == texturePasses.end()) {
			textureItr = texturePasses.emplace(texture, RenderData()).first;
		}

		// Check if submission is cached
		auto submissionItr = textureItr->second.lookup.find(&vertices);
		if (submissionItr != textureItr->second.lookup.end()) {
			if (states.dirty || submissionItr->second->transform != states.transform) {
				submissionItr->second->transform = states.transform;
				submissionItr->second->vertexList = &vertices;
				submissionItr->second->indexList = &indices;
				submissionItr->second->dirty = true;
			}
			else {
				submissionItr->second->resubmitted = true;
			}
		}
		else {
			// Create the submission
			textureItr->second.submissions.emplace_back(
				SubmissionData{
					states.transform, // transform
					&vertices,        // vertexList
					&indices,         // indexList
					true,             // resubmitted
					false             // dirty
				}
			);
		}
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
		, mOpaqueCalls()
		, mTransparentCalls()
	{
	}

	// Private method(s)
	void BatchRenderer2D::flush(ShaderPasses& drawcalls, bool frontToBack)
	{
		for (auto& shaderPass : drawcalls)
		{
			// Bind the shader
			shaderPass.first->bind();

			for (auto& blendPass : shaderPass.second)
			{
				// Set the appropriate blending
				if (blendPass.first != BlendMode::BlendNone) {
					GLCall(glEnable(GL_BLEND));
					GLCall(glBlendEquationSeparate(blendPass.first.colorEquation, blendPass.first.alphaEquation));
					GLCall(glBlendFuncSeparate(blendPass.first.colorSrcFactor, blendPass.first.colorDstFactor,
					                           blendPass.first.alphaSrcFactor, blendPass.first.alphaDstFactor));
				}
				else {
					GLCall(glDisable(GL_BLEND));
				}

				for (auto& texturePass : blendPass.second)
				{
					// Bind the texture
					texturePass.first->bind();

					// Sort the submissions
					sortSubmissions(texturePass.second, frontToBack);

					// Upload the vertices
					VertexBuffer* const vboPtr = mVAO->getVBO(0);
					vboPtr->setData(static_cast<int>(sizeof(Vertex2D) * texturePass.second.vertices.size()), texturePass.second.vertices.data());

					// Upload the indices
					IndexBuffer* const iboPtr = mVAO->getIBO();
					iboPtr->setData(texturePass.second.indices.size() * sizeof(GLuint), texturePass.second.indices.data());

					// Draw the indices
					GLCall(glDrawElements(GL_TRIANGLES, iboPtr->getCount(), GL_UNSIGNED_INT, nullptr));

					// Unbind the texture
					texturePass.first->unbind();

					// Reset the submissions' resubmission flags
					resetSubmissions(texturePass.second);
				}
			}

			// Unbind the shader
			shaderPass.first->unbind();
		}
	}

	void BatchRenderer2D::sortSubmissions(RenderData& data, bool frontToBack)
	{
		bool dirtySubmissionFound = false;

		// Retrieve the vertex and index count of all submissions, and check if any submission is dirty
		size_t vertexCount = 0, indexCount = 0;
		for (const auto& submission : data.submissions) {
			vertexCount += submission.vertexList->size();
			indexCount += submission.indexList->size();

			if (submission.dirty) {
				dirtySubmissionFound = true;
			}
		}

		// Check if the submissions need to be sorted
		if (!dirtySubmissionFound && data.vertices.size() == vertexCount && data.indices.size() == indexCount) {
			return;
		}

		// Sort the submissions
		if (frontToBack) {
			std::sort(data.submissions.begin(), data.submissions.end(), [](SubmissionData& data1, SubmissionData& data2) {
				return data1.vertexList->front().position.z > data2.vertexList->front().position.z;
			});
		}
		else {
			std::sort(data.submissions.begin(), data.submissions.end(), [](SubmissionData& data1, SubmissionData& data2) {
				return data1.vertexList->front().position.z < data2.vertexList->front().position.z;
			});
		}

		// Clear the stored vertices and indices, and reset the index offset
		data.vertices.clear();
		data.indices.clear();
		data.lookup.clear();
		data.indexOffset = 0;

		// Store the submissions' vertices and indices
		data.vertices.reserve(vertexCount);
		data.indices.reserve(indexCount);
		for (auto& submission : data.submissions)
		{
			// Store the vertices
			for (const Vertex2D& vertex : *submission.vertexList) {
				data.vertices.emplace_back(
					Vertex2D{
						Vector3f((submission.transform * Vector3f(vertex.position.xy)).xy, vertex.position.z),
						vertex.color,
						vertex.uv
					}
				);
			}

			// Store the indices
			for (const unsigned int& index : *submission.indexList) {
				data.indices.emplace_back(index + data.indexOffset);
			}

			// Update the index offset by retrieving the max element of the submission's indices
			auto maxIndex = std::max_element(submission.indexList->begin(), submission.indexList->end());
			data.indexOffset += *maxIndex + 1;

			// Update the lookup table to detect if a submission was resubmitted faster
			data.lookup.try_emplace(submission.vertexList, &submission);
		}
	}

	void BatchRenderer2D::resetSubmissions(RenderData& data)
	{
		// Delete the submissions if at least one wasn't resubmitted this frame
		for (const auto& submission : data.submissions) {
			if (!submission.resubmitted) {
				data = RenderData();
				break;
			}
		}

		// Reset the submissions' flags
		for (auto& submission : data.submissions) {
			submission.resubmitted = false;
			submission.dirty = false;
		}
	}
}