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

#include <AEON/Graphics/Renderer2D.h>

#include <GL/glew.h>

#include <AEON/Graphics/internal/GLCommon.h>
#include <AEON/Graphics/Camera.h>

#include <iostream>

namespace ae
{
	void Renderer2D::beginScene(RenderTarget& target)
	{
		mRenderTarget = &target;
		Camera* const camera = mRenderTarget->getCamera();
		const Matrix4f& viewMatrix = camera->getViewMatrix();
		const Matrix4f& projMatrix = camera->getProjectionMatrix();

		mTransformUBO->queueUniformUpload("view", viewMatrix.elements.data(), sizeof(Matrix4f));
		mTransformUBO->queueUniformUpload("projection", projMatrix.elements.data(), sizeof(Matrix4f));
		mTransformUBO->queueUniformUpload("viewProjection", (projMatrix * viewMatrix).elements.data(), sizeof(Matrix4f));
		mTransformUBO->uploadQueuedUniforms();
	}

	void Renderer2D::endScene()
	{
		glEnable(GL_DEPTH_TEST);

		mRenderTarget->activate();

		mSpriteVAO->bind();

		for (auto& shaderPass : mDrawcalls)
		{
			shaderPass.first->bind();

			BlendPass& blendPasses = shaderPass.second;
			for (auto& blendPass : blendPasses)
			{
				if (blendPass.first != BlendMode::BlendNone) {
					GLCall(glEnable(GL_BLEND));
					GLCall(glBlendEquationSeparate(blendPass.first.colorEquation, blendPass.first.alphaEquation));
					GLCall(glBlendFuncSeparate(blendPass.first.colorSrcFactor, blendPass.first.colorDstFactor,
					                           blendPass.first.alphaSrcFactor, blendPass.first.alphaDstFactor));
				}
				else {
					GLCall(glDisable(GL_BLEND));
				}

				TexturePass& texturePasses = blendPass.second;
				for (auto& texturePass : texturePasses)
				{
					texturePass.first->bind();

					// Upload vertices
					VertexBuffer* const vboPtr = mSpriteVAO->getVBO(0);
					vboPtr->setData(static_cast<int>(sizeof(Vertex2D) * texturePass.second.vertices.size()), texturePass.second.vertices.data());

					// Upload indices
					std::vector<unsigned int>& indices = texturePass.second.indices;
					IndexBuffer* const iboPtr = mSpriteVAO->getIBO();
					iboPtr->setData(indices.size() * sizeof(GLuint), indices.data());

					// Draw indices
					GLCall(glDrawElements(GL_TRIANGLES, iboPtr->getCount(), GL_UNSIGNED_INT, nullptr));

					texturePass.first->unbind();

					// Delete the stored submissions that weren't resubmitted this frame
					deleteSubmissions(texturePass.second);
					// Reset the cached submissions flag
					for (auto& submission : texturePass.second.subData) {
						submission.second.resubmitted = false;
					}
				}
			}

			shaderPass.first->unbind();
		}

		mSpriteVAO->unbind();
	}

	void Renderer2D::submit(const Renderable2D& renderable, const RenderStates& states, bool dirty)
	{
		submit(renderable.getVertices(), renderable.getIndices(), states, dirty);
	}

	void Renderer2D::submit(const std::vector<Vertex2D>& vertices, const std::vector<unsigned int>& indices, const RenderStates& states, bool dirty)
	{
		// Find existing shader pass or create a new one
		auto shaderItr = mDrawcalls.find(states.shader);
		if (shaderItr == mDrawcalls.end()) {
			shaderItr = mDrawcalls.emplace(states.shader, BlendPass()).first;
		}

		// Find existing blend pass or create a new one
		BlendPass& blendPasses = shaderItr->second;
		auto blendItr = blendPasses.find(states.blendMode);
		if (blendItr == blendPasses.end()) {
			blendItr = blendPasses.emplace(states.blendMode, TexturePass()).first;
		}

		// Find existing texture pass or create a new one
		TexturePass& texturePasses = blendItr->second;
		const Texture* const texture = (!states.texture) ? mWhiteTexture.get() : states.texture;
		auto textureItr = texturePasses.find(texture);
		if (textureItr == texturePasses.end()) {
			textureItr = texturePasses.emplace(texture, RenderData()).first;
		}

		// Check if submission is cached
		auto subFound = textureItr->second.subData.find(&vertices);
		if (subFound != textureItr->second.subData.end()) {
			subFound->second.resubmitted = true;
			if (dirty || textureItr->second.transform != states.transform) {
				if (subFound->second.indexCount == indices.size()) {
					auto cachedVertexItr = textureItr->second.vertices.begin() + subFound->second.vertexOffset;
					for (auto vertexItr = vertices.begin(); vertexItr != vertices.end(); ++vertexItr, ++cachedVertexItr) {
						*cachedVertexItr = Vertex2D{ Vector3f(Vector2f(states.transform * Vector3f(vertexItr->position.xy)), vertexItr->position.z), vertexItr->color, vertexItr->uv };
					}
					textureItr->second.transform = states.transform;
				}
				else {
					subFound->second.resubmitted = false;
				}
			}
		}
		else {
			// Retrieve the list of vertices and store them
			std::vector<Vertex2D>& vertexList = textureItr->second.vertices;
			vertexList.reserve(vertices.size());
			for (const Vertex2D& vertex : vertices) {
				vertexList.emplace_back(Vertex2D{ Vector3f(Vector2f(states.transform * Vector3f(vertex.position.xy)), vertex.position.z), vertex.color, vertex.uv });
			}

			// Retrieve the list of indices and store them
			std::vector<unsigned int>& indexList = textureItr->second.indices;
			indexList.reserve(indices.size());
			for (const unsigned int& index : indices) {
				indexList.emplace_back(index + textureItr->second.indexOffset);
			}
			auto minMaxIndices = std::minmax_element(indices.begin(), indices.end());
			textureItr->second.indexOffset += *minMaxIndices.second + 1;

			// Store the transform
			textureItr->second.transform = states.transform;

			// Create submission
			textureItr->second.subData.emplace(&vertices, SubmissionData{ vertexList.size() - vertices.size(), indices.size(), true });
		}

	}

	// Public static method(s)
	Renderer2D& Renderer2D::getInstance()
	{
		static Renderer2D instance;
		return instance;
	}

	// Private constructor(s)
	Renderer2D::Renderer2D()
		: mTransformUBO(GLResourceFactory::getInstance().get<UniformBuffer>("_AEON_TransformUBO"))
		, mSpriteVAO(GLResourceFactory::getInstance().get<VertexArray>("_AEON_VAO"))
		, mRenderTarget(nullptr)
		, mWhiteTexture(GLResourceFactory::getInstance().get<Texture2D>("_AEON_WhiteTexture"))
		, mDrawcalls()
	{
	}

	void Renderer2D::deleteSubmissions(RenderData& renderData)
	{
		std::vector<unsigned int>& indices = renderData.indices;
		std::vector<Vertex2D>& vertices = renderData.vertices;

		// Remove render data if at least one submission wasn't resubmitted this frame
		for (const auto& sub : renderData.subData) {
			if (!sub.second.resubmitted) {
				renderData = RenderData();
				break;
			}
		}
	}
}