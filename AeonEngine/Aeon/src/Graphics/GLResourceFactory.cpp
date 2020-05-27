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

#include <AEON/Graphics/GLResourceFactory.h>

#include <string>

#include <AEON/System/DebugLogger.h>

namespace ae
{
	// Public method(s)
	void GLResourceFactory::destroyUnused()
	{
		// Destroy all unused resources in the hashmaps
		for (auto& resourceMap : mResourceMaps) {
			for (auto itr = resourceMap.second.begin(); itr != resourceMap.second.end(); ++itr) {
				if (itr->second.use_count() == 1) {
					itr->second->destroy();
					resourceMap.second.erase(itr);
				}
			}
		}
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
		// Retrieve the shader sources
			// Sprite Shader
		std::string spriteShaderVertSource =
		#include <AEON/Shaders/Sprite.vs>
		;
		std::string spriteShaderFragSource =
		#include <AEON/Shaders/Sprite.fs>
		;

		// Create the shaders
			// Sprite Shader
		auto spriteShader = create<Shader>("Sprite");
		spriteShader->loadFromSource(Shader::StageType::Vertex, spriteShaderVertSource);
		spriteShader->loadFromSource(Shader::StageType::Fragment, spriteShaderFragSource);
		spriteShader->link();
	}
}