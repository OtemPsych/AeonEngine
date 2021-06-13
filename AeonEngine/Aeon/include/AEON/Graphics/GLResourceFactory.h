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

#ifndef Aeon_Graphics_GLResourceFactory_H_
#define Aeon_Graphics_GLResourceFactory_H_

#include <memory>
#include <unordered_map>

#include <AEON/Graphics/internal/GLResource.h>
#include <AEON/Graphics/internal/VertexArray.h>
#include <AEON/Graphics/internal/UniformBuffer.h>
#include <AEON/Graphics/internal/VertexBuffer.h>
#include <AEON/Graphics/internal/IndexBuffer.h>
#include <AEON/Graphics/internal/Framebuffer.h>
#include <AEON/Graphics/Texture.h>
#include <AEON/Graphics/Texture2D.h>
#include <AEON/Graphics/Shader.h>

namespace ae
{
	/*!
	 \brief The singleton class used to create and manage the lifetime of OpenGL resources.
	 \details It also contains several pre-compiled shaders that can be used by the API user.
	*/
	class AEON_API GLResourceFactory
	{
	private:
		// Private enumeration(s)
		/*!
		 \brief The enumeration of all ae::GLResource types available.
		*/
		enum class ResourceType
		{
			Shader,     //!< The shader program type
			VAO,        //!< The vertex array object type
			VBO,        //!< The vertex buffer object type
			IBO,        //!< The index buffer object type
			UBO,        //!< The uniform buffer object type
			Texture,    //!< The texture object type
			Framebuffer //!< The framebuffer object type
		};
	private:
		// Private typedef(s)
		using ResourceMap = std::unordered_map<std::string, std::shared_ptr<GLResource>>; //!< A hashmap containing all GLResource objects of a certain type with an associated name

	public:
		// Public constructor(s)
		/*!
		 \brief Deleted copy constructor.

		 \since v0.4.0
		*/
		GLResourceFactory(const GLResourceFactory&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.4.0
		*/
		GLResourceFactory(GLResourceFactory&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.4.0
		*/
		GLResourceFactory& operator=(const GLResourceFactory&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.4.0
		*/
		GLResourceFactory& operator=(GLResourceFactory&&) = delete;
	public:
		// Public method(s)
		/*!
		 \brief Creates and returns a shared_ptr to the ae::GLResource derived type that will be stored in the ae::GLResourceFactory instance.
		 \details The ae::GLResourceFactory will be responsible for its lifetime and for destroying the OpenGL-allocated resources.\n
		 The resource created can be retrieved at a later date by providing the resource's unique name or made anonymous by providing an empty string.
		 \note Only classes that derive from ae::GLResource may be created using this method.

		 \param[in] name The string containing the name that will be associated to the resource created, the name must be unique amongst its resources of the same type, an id will be provided if the string is empty
		 \param[in] args The various arguments of the specific resource type that will be forwarded to its appropriate constructor, it's up to the API user to provide valid arguments

		 \return The newly-created shared_ptr to the ae::GLResource derived type

		 \par Example:
		 \code
		 // Retrieve the ae::GLResourceFactory's single instance
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();

		 // Create an ae::VertexArray with the name "vao"
		 auto vao = glResourceFactory.create<ae::VertexArray>("vao");

		 // Attempt to create an ae::VertexArray with the name "vao" that was previously created
		 auto vao2 = glResourceFactory.create<ae::VertexArray>("vao"); // error!

		 // Create an ae::Shader with the name "vao" (this is allowed as its type is different)
		 auto shader = glResourceFactory.create<ae::Shader>("vao", ae::Shader::LinkType::Monolithic);

		 // Create an anonymous ae::Shader by not providing a name
		 auto shader2 = glResourceFactory.create<ae::Shder>("");
		 \endcode

		 \sa get()

		 \since v0.4.0
		*/
		template <class T, typename... Args>
		_NODISCARD std::enable_if_t<std::is_base_of_v<GLResource, T>, std::shared_ptr<T>> create(const std::string& name, Args&&... args);
		/*!
		 \brief Retrieves a previously-created shared_ptr to a ae::GLResource derived type that's stored in the ae::GLResourceFactory instance.
		 \note If the same \a name is associated with two resources of a different type, the template type should be provided to ensure the correct resource is retrieved.

		 \param[in] name The name associated with the resource created

		 \return A shared_ptr to the ae::GLResource derived type associated to the \a name provided

		 \par Example:
		 \code
		 // Retrieve the ae::GLResourceFactory's single instance
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();

		 // Create an ae::VertexArray with the name "vao"
		 auto vao = glResourceFactory.create<ae::VertexArray>("vao");

		 // Create an ae::Shader with the name "vao" (this is allowed as its type is different)
		 auto shader = glResourceFactory.create<ae::Shader>("vao", ae::Shader::LinkType::Monolithic);

		 // Retrieve the ae::VertexArray with the name "vao"
		 auto sameVao = glResourceFactory.get<ae::VertexArray>("vao");

		 // Retrieve the ae::Shader with the name "vao"
		 auto sameShader = glResourceFactory.get<ae::Shader>("vao");
		 \endcode

		 \sa create()

		 \since v0.4.0
		*/
		template <class T>
		_NODISCARD std::shared_ptr<T> get(const std::string& name);

		/*!
		 \brief Destroys all unused OpenGL resources.
		 \note Should be used whenever the application's state changes.

		 \since v0.4.0
		*/
		void destroyUnused();
		/*!
		 \brief Destroys all stored OpenGL resources.
		 \details Typically used at the termination of the application.

		 \since v0.4.0
		*/
		void destroy() const;
		/*!
		 \brief Attempts to reload all stored OpenGL resources that need to be reloaded after an OpenGL context change.
		 \note This method is automatically called when the window's OpenGL context is destroyed.

		 \since v0.4.0
		*/
		void reload();

		// Public static method(s)
		/*!
		 \brief Retrieves the single instance of the ae::GLResourceFactory.

		 \return The single instance of the ae::GLResourceFactory

		 \par Example:
		 \code
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 \endcode

		 \since v0.4.0
		*/
		_NODISCARD static GLResourceFactory& getInstance() noexcept;
	private:
		// Private constructor(s)
		/*!
		 \brief Default constructor.
		 \details Initializes all the maps that will contain the OpenGL resources.

		 \since v0.4.0
		*/
		GLResourceFactory();
	private:
		// Private method(s)
		/*!
		 \brief Retrieves the source code of the pre-compiled shaders and creates the ae::Shader objects.

		 \since v0.4.0
		*/
		void createPrecompiledShaders();
		/*!
		 \brief Retrieves a pointer to the resource hashmap associated with the type T provided.

		 \return A pointer to the resource hashmap associated to the type T

		 \since v0.4.0
		*/
		template <class T>
		_NODISCARD ResourceMap* const getResourceMap();

	private:
		// Private member(s)
		std::unordered_map<ResourceType, ResourceMap> mResourceMaps; //!< The hashmap containing the hashmaps of all GLResource objects of a certain type
	};
}
#include <AEON/Graphics/GLResourceFactory.inl>
#endif // Aeon_Graphics_GLResourceFactory_H_

/*!
 \class ae::GLResourceFactory
 \ingroup graphics

 The ae::GLResourceFactory singleton class is used to create and manage the
 lifetime of all ae::GLResources derived objects, such as: ae::VertexArray
 objects, ae::Shader objects, ae::UniformBuffer objects, etc.

 It contains several pre-compiled ae::Shader objects that represent the most
 common shaders that can be used by the API user.

 \author Filippos Gleglakos
 \version v0.4.0
 \date 2020.05.18
 \copyright MIT License
*/