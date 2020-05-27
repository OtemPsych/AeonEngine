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

namespace ae
{
	// Public method(s)
	template <class T, typename... Args>
	_NODISCARD std::enable_if_t<std::is_base_of_v<GLResource, T>, std::shared_ptr<T>> GLResourceFactory::create(const std::string& name, Args&&... args)
	{
		// Retrieve the appropriate hashmap
		ResourceMap* const resourceMap = getResourceMap<T>();

		// Create the resource and add it into the appropriate hashmap
		if (resourceMap) {
			auto resource = std::make_shared<T>(std::forward<Args>(args)...);
			const std::string NAME = (name.empty()) ? std::to_string(reinterpret_cast<uint64_t>(resource.get())) + std::to_string(resource->getHandle()) : name;

			return std::dynamic_pointer_cast<T>(resourceMap->try_emplace(NAME, std::move(resource)).first->second);
		}
		return nullptr;
	}

	template <class T>
	_NODISCARD std::shared_ptr<T> GLResourceFactory::get(const std::string& name)
	{
		// Retrieve the appropriate hashmap
		ResourceMap* const resourceMap = getResourceMap<T>();

		// Retrieve the resource associated with the name provided
		if (resourceMap) {
			auto found = resourceMap->find(name);
			if (found != resourceMap->end()) {
				return std::dynamic_pointer_cast<T>(found->second);
			}
		}
		return nullptr;
	}

	// Private method(s)
	template <class T>
	_NODISCARD GLResourceFactory::ResourceMap* const GLResourceFactory::getResourceMap()
	{
		ResourceMap* resourceMap = nullptr;
		if (std::is_same_v<VertexBuffer, T>) {
			resourceMap = &mResourceMaps[ResourceType::VBO];
		}
		else if (std::is_same_v<Texture, T> || std::is_base_of_v<Texture, T>) {
			resourceMap = &mResourceMaps[ResourceType::Texture];
		}
		else if (std::is_same_v<Shader, T>) {
			resourceMap = &mResourceMaps[ResourceType::Shader];
		}
		else if (std::is_same_v<VertexArray, T>) {
			resourceMap = &mResourceMaps[ResourceType::VAO];
		}
		else if (std::is_same_v<IndexBuffer, T>) {
			resourceMap = &mResourceMaps[ResourceType::IBO];
		}
		else if (std::is_same_v<UniformBuffer, T>) {
			resourceMap = &mResourceMaps[ResourceType::UBO];
		}
		else if (std::is_same_v<Framebuffer, T>) {
			resourceMap = &mResourceMaps[ResourceType::Framebuffer];
		}

		return resourceMap;
	}
}