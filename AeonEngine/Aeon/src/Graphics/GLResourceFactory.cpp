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

#include <AEON/Graphics/GLResourceFactory.h>

#include <string>

#include <GL/glew.h>

#include <AEON/System/DebugLogger.h>

namespace ae
{
	// Public method(s)
	void GLResourceFactory::destroyUnused()
	{
		// Destroy all unused textures in the hashmaps
		ResourceMap& textureMap = mResourceMaps[ResourceType::Texture];
		for (auto texture = textureMap.begin(); texture != textureMap.end(); ++texture) {
			if (texture->second.use_count() == 1) {
				AEON_LOG_INFO("Texture deallocation", "Texture \"" + texture->first + "\" was freed from memory.");
				texture->second->destroy();
				textureMap.erase(texture);
				texture = textureMap.begin();
			}
		}

		/*for (auto& resourceMap : mResourceMaps) {
			for (auto itr = resourceMap.second.begin(); itr != resourceMap.second.end(); ++itr) {
				if (itr->second.use_count() == 1) {
					itr->second->destroy();
					resourceMap.second.erase(itr);
					itr = resourceMap.second.begin();
				}
			}
		}*/
	}

	void GLResourceFactory::destroy() const
	{
		// Destroy all resources in the hashmaps
		for (const auto& resourceMap : mResourceMaps) {
			for (const auto& resource : resourceMap.second) {
				resource.second->destroy();
			}
		}
	}

	void GLResourceFactory::reload()
	{
		// Destroy and reload all shaders
		ResourceMap& shaderMap = mResourceMaps[ResourceType::Shader];
		for (const auto& shaderResource : shaderMap) {
			auto shaderPtr = std::dynamic_pointer_cast<Shader>(shaderResource.second);
			shaderPtr->destroy();
			shaderPtr->reload();
		}
	}

	// Public static method(s)
	GLResourceFactory& GLResourceFactory::getInstance() noexcept
	{
		static GLResourceFactory instance;
		return instance;
	}
	
	// Private constructor(s)
	GLResourceFactory::GLResourceFactory()
		: mResourceMaps()
	{
		createPrecompiledShaders();
	}

	// Private method(s)
	void GLResourceFactory::createPrecompiledShaders()
	{
		// Shaders
			// Retrieve the shader sources
				// Basic2D Shader
		std::string basic2DShaderVertSource =
		#include <AEON/Shaders/Basic2D.vs>
		;
		std::string basic2DShaderFragSource =
		#include <AEON/Shaders/Basic2D.fs>
		;

				// Text2D Shader
		std::string text2DShaderVertSource =
		#include <AEON/Shaders/Text2D.vs>
		;
		std::string text2DShaderFragSource =
		#include <AEON/Shaders/Text2D.fs>
		;

			// Create the shaders
				// Basic2D Shader
		std::shared_ptr<Shader> basic2DShader = create<Shader>("_AEON_Basic2D");
		basic2DShader->loadFromSource(Shader::StageType::Vertex, basic2DShaderVertSource);
		basic2DShader->loadFromSource(Shader::StageType::Fragment, basic2DShaderFragSource);
		basic2DShader->link();

				// Text2D Shader
		std::shared_ptr<Shader> text2DShader = create<Shader>("_AEON_Text2D");
		text2DShader->loadFromSource(Shader::StageType::Vertex, text2DShaderVertSource);
		text2DShader->loadFromSource(Shader::StageType::Fragment, text2DShaderFragSource);
		text2DShader->link();

			// Set the shaders' data layouts
				// Basic2D Shader
		VertexBuffer::Layout& basic2DShaderLayout = basic2DShader->getDataLayout();
		basic2DShaderLayout.addElement(GL_FLOAT, 3, GL_FALSE);
		basic2DShaderLayout.addElement(GL_FLOAT, 4, GL_FALSE);
		basic2DShaderLayout.addElement(GL_FLOAT, 2, GL_FALSE);

				// Text2D Shader
		VertexBuffer::Layout& text2DShaderLayout = text2DShader->getDataLayout();
		text2DShaderLayout.addElement(GL_FLOAT, 3, GL_FALSE);
		text2DShaderLayout.addElement(GL_FLOAT, 4, GL_FALSE);
		text2DShaderLayout.addElement(GL_FLOAT, 2, GL_FALSE);

			// Create the UBOs and attach them to the shaders
				// Transform UBO
		auto transformUBO = create<UniformBuffer>("_AEON_TransformUBO");
		transformUBO->queryLayout(*basic2DShader, "uTransformBlock", { "model", "view", "projection", "viewProjection", "mvp" });
		basic2DShader->addUniformBuffer(*transformUBO);
		text2DShader->addUniformBuffer(*transformUBO);

		// VAOs
			// Create the IBOs
		auto ibo = std::make_unique<IndexBuffer>(GL_DYNAMIC_DRAW);

			// Create the VBOs
		auto vbo = std::make_unique<VertexBuffer>(GL_DYNAMIC_DRAW);
		vbo->getLayout().addElement(GL_FLOAT, 3, GL_FALSE); // position
		vbo->getLayout().addElement(GL_FLOAT, 4, GL_FALSE); // color
		vbo->getLayout().addElement(GL_FLOAT, 2, GL_FALSE); // uv coordinates

			// Create the VAO and attach the VBOs and IBOs
		auto vao = create<VertexArray>("_AEON_VAO");
		vao->addVBO(std::move(vbo));
		vao->addIBO(std::move(ibo));

		// Textures
			// White Texture
		uint32_t hexWhite = 0xffffffff;
		auto whiteTexture = create<Texture2D>("_AEON_WhiteTexture", Texture2D::Filter::Nearest, Texture2D::Wrap::Repeat, Texture2D::InternalFormat::RGBA8);
		whiteTexture->create(1, 1, &hexWhite);
	}
}