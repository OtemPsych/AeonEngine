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

#ifndef Aeon_Graphics_Material_H_
#define Aeon_Graphics_Material_H_

#include <vector>
#include <memory>
#include <string>

#include <AEON/Config.h>

namespace ae
{
	// Forward declaration(s)
	class Shader;
	class Texture;
	class UniformBuffer;

	/*!
	 \brief The class representing a material used to define properties determining how light affects an object.
	*/
	class AEON_API Material
	{
	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::Material by providing a name and an ae::Shader program to use.

		 \param[in] name A name to assign to this ae::Material
		 \param[in] shader The ae::Shader to use

		 \par Example:
		 \code
		 auto shader = ae::GLResourceFactory::getInstance().create<ae::Shader>();
		 ...
		 ae::Material material("GoldMaterial", shader);
		 \endcode

		 \since v0.4.0
		*/
		Material(const std::string& name, const std::shared_ptr<Shader>& shader);
		/*!
		 \brief Deleted copy constructor.

		 \since v0.4.0
		*/
		Material(const Material&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.4.0
		*/
		Material(Material&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.4.0
		*/
		Material& operator=(const Material&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.4.0
		*/
		Material& operator=(Material&&) = delete;
	public:
		// Public method(s)
		/*!
		 \brief Sets the uniform associated to \a name the \a data provided.
		 \details Valid uniform names:
		 \li "ambient" (ae::Vector3f)
		 \li "diffuse" (ae::Vector3f)
		 \li "specular" (ae::Vector3f)
		 \li "shininess" (float)

		 \param[in] name A string containing the name of one of the valid uniform names
		 \param[in] data The pointer to the uniform's new data
		 \param[in] size The size of the uniform's new \a data in bytes (sizeof() of the data's type)

		 \par Example:
		 \code
		 auto shader = ae::GLResourceFactory::getInstance().create<ae::Shader>();
		 ...
		 ae::Material material("GoldMaterial", shader);

		 // Set the material's ambient light value
		 ae::Vector3f materialAmbient(0.0215f, 0.1745f, 0.0215f);
		 material.setUniform("ambient", materialAmbient.elements.data(), sizeof(ae::Vector3f));
		 \endcode

		 \since v0.4.0
		*/
		void setUniform(const std::string& name, const void* data, size_t size);
		/*!
		 \brief 
		*/
		void setTexture(const std::shared_ptr<Texture>& texture);
		/*!
		 \brief Retrieves the ae::Material's assigned name.

		 \return A string containing the ae::Material's name

		 \par Example:
		 \code
		 auto shader = ae::GLResourceFactory::getInstance().create<ae::Shader>();
		 ...
		 ae::Material material("GoldMaterial", shader);
		 ...
		 const std::string& materialName = material.getName();
		 \endcode

		 \since v0.4.0
		*/
		_NODISCARD const std::string& getName() const noexcept;
		/*!
		 \brief Retrieves the ae::Material's assigned ae::Shader.

		 \return The ae::Material's ae::Shader program

		 \par Example:
		 \code
		 auto shader = ae::GLResourceFactory::getInstance().create<ae::Shader>();
		 ...
		 ae::Material material("GoldMaterial", shader);
		 ...
		 const ae::Shader& materialShader = material.getShader();
		 \endcode

		 \since v0.4.0
		*/
		_NODISCARD const Shader& getShader() const noexcept;

	private:
		// Private member(s)
		const std::string                     mName;     //!< The material's assigned name
		std::vector<std::shared_ptr<Texture>> mTextures; //!< The list of textures
		std::shared_ptr<Shader>               mShader;   //!< The shader program to use
		std::shared_ptr<UniformBuffer>        mUBO;      //!< The UBO containing the light reaction properties
	};
}
#endif // Aeon_Graphics_Material_H_

/*!
 \class ae::Material
 \ingroup graphics

 The ae::Material class contains several properties that determine how a mesh
 reacts to light, as well as one or more textures that be wrapped on said mesh.

 \author Filippos Gleglakos
 \version v0.4.0
 \date 2020.01.07
 \copyright MIT License
*/