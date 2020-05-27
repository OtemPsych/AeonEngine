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

#include <iostream>

#include <GL/glew.h>

#include <AEON/Graphics/internal/GLCommon.h>
#include <AEON/Graphics/Camera.h>

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
		mRenderTarget->activate();

		mSpriteVAO->bind();
		//mTransformUBO->bind();
		mSpriteIBO->bind();
		//mSpriteVBO->bind();

		for (const auto& shaderPass : mDrawcalls)
		{
			shaderPass.first->bind();

			const BlendPass& blendPasses = shaderPass.second;
			for (const auto& blendPass : blendPasses)
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

				const TexturePass& texturePasses = blendPass.second;
				for (const auto& texturePass : texturePasses)
				{
					texturePass.first->bind();

					mSpriteVBO->setData(static_cast<int>(sizeof(Vertex2D) * texturePass.second.vertices.size()), nullptr);
					void* vboMap = mSpriteVBO->map(GL_WRITE_ONLY);

					// Upload vertices
					const std::vector<Vertex2D>& vertices = texturePass.second.vertices;
					uint64_t offset = 0;
					for (const Vertex2D& vertex : vertices)
					{
						// Upload position
						memcpy(static_cast<void*>(static_cast<char*>(vboMap) + offset), vertex.position.elements.data(), sizeof(Vertex2D::position));
						offset += sizeof(Vertex2D::position);

						// Upload color
						memcpy(static_cast<void*>(static_cast<char*>(vboMap) + offset), vertex.color.elements.data(), sizeof(Vertex2D::color));
						offset += sizeof(Vertex2D::color);

						// Upload uv coordinates
						memcpy(static_cast<void*>(static_cast<char*>(vboMap) + offset), vertex.uv.elements.data(), sizeof(Vertex2D::uv));
						offset += sizeof(Vertex2D::uv);
					}
					mSpriteVBO->unmap();

					// Upload indices
					const std::vector<unsigned int>& indices = texturePass.second.indices;
					mSpriteIBO->setData(static_cast<int>(indices.size() * sizeof(GLuint)), indices.data());

					// Draw indices
					GLCall(glDrawElements(GL_TRIANGLES, mSpriteIBO->getCount(), GL_UNSIGNED_INT, nullptr));

					texturePass.first->unbind();
				}
			}

			shaderPass.first->unbind();
		}

		//mSpriteVBO->unbind();
		mSpriteIBO->unbind();
		//mTransformUBO->unbind();
		mSpriteVAO->unbind();
		mDrawcalls.clear();
	}

	void Renderer2D::submit(Renderable2D& renderable, const RenderStates& states)
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

		// Retrieve the list of vertices and store them
		const std::vector<Vertex2D>& vertices = renderable.getVertices();
		std::vector<Vertex2D>& vertexList = textureItr->second.vertices;
		vertexList.reserve(vertices.size());
		for (const Vertex2D& vertex : vertices) {
			vertexList.emplace_back(Vertex2D{ Vector2f(states.transform * Vector3f(vertex.position)), vertex.uv, vertex.color });
		}

		// Retrieve the list of indices and store them
		const std::vector<unsigned int>& indices = renderable.getIndices();
		std::vector<unsigned int>& indexList = textureItr->second.indices;
		indexList.reserve(indices.size());
		for (const unsigned int& index : indices) {
			indexList.emplace_back(index + textureItr->second.indexOffset);
		}
		textureItr->second.indexOffset += *std::max_element(indices.begin(), indices.end()) + 1;
	}

	// Public static method(s)
	Renderer2D& Renderer2D::getInstance()
	{
		static Renderer2D instance;
		return instance;
	}

	// Private constructor(s)
	Renderer2D::Renderer2D()
		: mTransformUBO(GLResourceFactory::getInstance().create<UniformBuffer>("TransformUBO"))
		, mSpriteVAO(GLResourceFactory::getInstance().create<VertexArray>("SpriteVAO"))
		, mSpriteVBO(nullptr)
		, mSpriteIBO(GLResourceFactory::getInstance().create<IndexBuffer>("SpriteIBO", GL_DYNAMIC_DRAW))
		, mSpriteShader(GLResourceFactory::getInstance().get<Shader>("Sprite"))
		, mRenderTarget(nullptr)
		, mWhiteTexture(GLResourceFactory::getInstance().create<Texture2D>("_AEON_WhiteTexture", Texture2D::Filter::Linear, Texture2D::Wrap::Repeat, Texture2D::InternalFormat::RGBA8))
		, mDrawcalls()
	{
		mTransformUBO->queryLayout(*mSpriteShader, "uTransformBlock", { "model", "view", "projection", "viewProjection", "mvp" });
		mSpriteShader->addUniformBuffer(mTransformUBO.get());

		auto vbo = std::make_unique<VertexBuffer>(GL_DYNAMIC_DRAW);
		vbo->getLayout().addElement(GL_FLOAT, 2, GL_FALSE);
		vbo->getLayout().addElement(GL_FLOAT, 4, GL_FALSE);
		vbo->getLayout().addElement(GL_FLOAT, 2, GL_FALSE);
		mSpriteVBO = vbo.get();
		mSpriteVAO->addBuffer(std::move(vbo));

		uint32_t white = 0xffffffff;
		mWhiteTexture->create(2, 2, &white);
	}
}