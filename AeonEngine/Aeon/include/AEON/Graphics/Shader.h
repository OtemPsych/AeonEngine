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

#ifndef Aeon_Graphics_Shader_H_
#define Aeon_Graphics_Shader_H_

#include <map>

#include <AEON/Math/Vector2.h>
#include <AEON/Math/Vector3.h>
#include <AEON/Math/Vector4.h>
#include <AEON/Math/Matrix.h>
#include <AEON/Graphics/internal/GLResource.h>

namespace ae
{
	// Forward declaration(s)
	class UniformBuffer;

	/*!
	 \brief The class representing an OpenGL shader program to which several shader stages can be attached.
	 \details The API user can find several pre-compiled shaders in the ae::GLResourceFactory singleton class.
	*/
	class _NODISCARD AEON_API Shader : public GLResource
	{
	public:
		/*!
		 \brief The enumeration of the possible link types of a shader program.
		 \details A monolithic link forces the user to provide all desired shader stages into a single shader program. On the other hand, a separable link permits the user to only attach a single Vertex shader to a shader program and the remaining shader stages to another.\n
		 This approach can be used to have a standard Vertex shader in one shader program that can be used with several different shader stages.
		*/
		enum class LinkType
		{
			Monolithic, //!< Represents the entire OpenGL pipeline stage
			Separable   //!< Represents a separable part of the OpenGL pipeline stage
		};
		/*!
		 \brief The enumeration of the different shader stage types.
		*/
		enum StageType
		{
			None           = 0x0000, //!< GL_NONE
			Fragment       = 0x8B30, //!< GL_FRAGMENT_SHADER
			Vertex         = 0x8B31, //!< GL_VERTEX_SHADER
			Geometry       = 0x8DD9, //!< GL_GEOMETRY_SHADER
			TessEvaluation = 0x8E87, //!< GL_TESS_EVALUATION_SHADER
			TessControl    = 0x8E88, //!< GL_TESS_CONTROL_SHADER
			Compute        = 0x91B9  //!< GL_COMPUTE_SHADER
		};

	private:
		/*!
		 \brief The struct representing a shader stage's data.
		*/
		struct Stage
		{
			std::string  source; //!< The source code of the shader stage
			unsigned int handle; //!< The OpenGL identifier of the shader stage
		};

	public:
		// Public constructor(s)
		/*!
		 \brief Constructs the ae::Shader by providing an optional link type.
		 \details The shader stages will have to be loaded in after the ae::Shader's construction.
		 \note The link type can't be modified following the ae::Shader's construction.

		 \param[in] linkType The ae::Shader::LinkType to use, ae::Shader::LinkType::Monolithic by default

		 \par Example:
		 \code
		 // Retrieve the single instance of the GLResourceFactory
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();

		 // Create a monolithic and a separable shader
		 std::shared_ptr<ae::Shader> monolithicShader = glResourceFactory.createShader();
		 std::shared_ptr<ae::Shader> separableShader = glResourceFactory.createShader(ae::Shader::LinkType::Separable);
		 \endcode

		 \since v0.4.0
		*/
		explicit Shader(LinkType linkType = LinkType::Monolithic);
		/*!
		 \brief Deleted copy constructor.

		 \since v0.4.0
		*/
		Shader(const Shader&) = delete;
		/*!
		 \brief Deleted move constructor.

		 \since v0.4.0
		*/
		Shader(Shader&&) = delete;
	public:
		// Public operator(s)
		/*!
		 \brief Deleted assignment operator.

		 \since v0.4.0
		*/
		Shader& operator=(const Shader&) = delete;
		/*!
		 \brief Deleted move assignment operator.

		 \since v0.4.0
		*/
		Shader& operator=(Shader&&) = delete;
	public:
		// Public method(s)
		/*!
		 \brief Loads in and attaches a shader stage that will be created by providing the \a type of the shader stage and its \a source code.
		 \details A shader stage representing the same stage as an already attached one will be refused.
		 
		 \param[in] type The ae::Shader::StageType indicating the type of the shader stage to create and attach
		 \param[in] source A string containing the source code of the shader stage

		 \par Example:
		 \code
		 // The string containing the source code
		 std::string vertShaderSource =
			"#version 450 core                          \n"
			"                                           \n"
			"void main()                                \n"
			"{                                          \n"
			"	gl_Position = vec4(0.0, 0.0, 0.5, 1.0); \n"
			"}";

		 // Retrieve the single instance of the GLResourceFactory
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 
		 // Create the shader program and attach the shader stage that will be created within the shader program
		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->loadFromSource(ae::Shader::StageType::Vertex, vertShaderSource);
		 \endcode

		 \sa loadFromFile()

		 \since v0.4.0
		*/
		void loadFromSource(StageType type, const std::string& source);
		/*!
		 \brief Loads in and attaches the shader stage that will be created by providing the \a type of the shader stage and the path of the file containing the source code.
		 \details A shader stage representing the same stage as an already attached one will be refused.

		 \param[in] type The ae::Shader::StageType indicating the type of the shader stage to create and attach
		 \param[in] filename A string containing the path of the file containing the source code

		 \par Example:
		 \code
		 // Retrieve the single instance of the GLResourceFactory
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();

		 // Create the shader program and attach the shader stage that will be created within the shader program
		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->loadFromFile(ae::Shader::StageType::Vertex, "Shaders/vertShader.vs");
		 \endcode

		 \sa loadFromSource()

		 \since v0.4.0
		*/
		void loadFromFile(StageType type, const std::string& filename);
		/*!
		 \brief Links together all the attached shader stages.
		 \note This method should only be called after having attached all the necessary shader stages.\n
		 A GL_INVALID_VALUE error will be generated regarding a detached shader if a complete vertex shader and a complete fragment shader haven't been attached.

		 \par Example:
		 \code
		 // Retrieve the single instance of the GLResourceFactory
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();

		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->loadFromFile(ae::Shader::StageType::Vertex, "Shaders/vertShader.vs");
		 shader->loadFromFile(ae::Shader::StageType::Fragment, "Shaders/fragShader.fs");
		 ...
		 shader->link();
		 \endcode

		 \sa loadFromSource(), loadFromFile()

		 \since v0.4.0
		*/
		void link() const;
		/*!
		 \brief Attempts to reload the ae::Shader using the currently attached shader stages and their current source code.
		 \details The 'destroy()' method should be called before calling this method as a new OpenGL identifier will be created for the new shader program and for every attached shader stage before linking them together.
		 \note This method is automatically called when the window's OpenGL context is destroyed.

		 \par Example:
		 \code
		 // The string containing the source code
		 std::string vertShaderSource =
			"#version 450 core                          \n"
			"                                           \n"
			"void main()                                \n"
			"{                                          \n"
			"	gl_Position = vec4(0.0, 0.0, 0.5, 1.0); \n"
			"}";
		 
		 // Create the shader program and load one shader from source and one from a file
		 ae::Shader shader;
		 shader.loadFromSource(ae::Shader::StageType::Vertex, vertShaderSource);
		 shader.loadFromFile(ae::Shader::StageType::Fragment, "Shaders/fragShader.fs");

		 ...

		 // The destroy() and reload() methods are called by the ae::GLResourceFactory instance if the shader was created through it
		 shader.destroy(); // Destroy the shader program and its shader stages
		 shader.reload();  // Reload the shader program and its shader stages, and link them together
		 \endcode

		 \since v0.4.0
		*/
		void reload();
		/*!
		 \brief Checks if the ae::Shader is currently bound.
		 \details If this method returns false, it could mean that either another shader program is bound or that no shader programs are bound.

		 \return True if it's currently bound, false otherwise

		 \par Example:
		 \code
		 // Retrieve the single instance of the GLResourceFactory
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();

		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->loadFromFile(ae::Shader::StageType::Vertex, "Shaders/vertShader.vs");
		 shader->loadFromFile(ae::Shader::StageType::Fragment, "Shaders/fragShader.fs");
		 shader->link();

		 shader->bind();
		 ...
		 if (shader->isBound()) {
			...
		 }
		 ...
		 shader->unbind();
		 \endcode

		 \sa bind()

		 \since v0.4.0
		*/
		_NODISCARD bool isBound() const;

		/*!
		 \brief Retrieves the ae::Shader's uniform block's index and assigns the ae::UniformBuffer's binding point to the uniform block's index.
		 
		 \param[in] ubo A pointer to an ae::UniformBuffer from which the uniform block data will be retrieved

		 \par Example:
		 \code
		 // Retrieve the single instance of the GLResourceFactory
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();

		 // Create the shader program and attach the shader stages that will be created within the shader program
		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->loadFromFile(ae::Shader::StageType::Vertex, "Shaders/vertexShader.vs");
		 shader->loadFromFile(ae::Shader::StageType::Fragment, "Shaders/fragmentShader.fs");
		 shader->link();

		 // Create a UBO and query the previously-created shader's layout to retrieve information about the uniform block
		 std::unique_ptr<ae::UniformBuffer> ubo;
		 ubo->queryLayout(*shader, "uTransformBlock", { "model", "view", "projection" });

		 // Add the UBO to the shader so that the shader automatically retrieves data from the UBO
		 shader->addUniformBuffer(ubo.get());
		 \endcode

		 \since v0.4.0
		*/
		void addUniformBuffer(const UniformBuffer* const ubo);

		/*!
		 \brief Sets the \a value provided to the uniform \a name.
		 
		 \param[in] name A string containing the name of the uniform
		 \param[in] value A float containing the uniform's new value

		 \par Example:
		 \code
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->setUniform("uFloatValue", 5.f);
		 \endcode

		 \since v0.4.0
		*/
		void setUniform(const std::string& name, float value);
		/*!
		 \brief Sets the \a value provided to the uniform \a name.

		 \param[in] name A string containing the name of the uniform
		 \param[in] value An int containing the uniform's new value

		 \par Example:
		 \code
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->setUniform("uIntValue", 5);
		 \endcode

		 \since v0.4.0
		*/
		void setUniform(const std::string& name, int value);
		/*!
		 \brief Sets the \a value provided to the uniform \a name.

		 \param[in] name A string containing the name of the uniform
		 \param[in] value An unsigned int containing the uniform's new value

		 \par Example:
		 \code
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->setUniform("uUnsignedIntValue", 5u);
		 \endcode

		 \since v0.4.0
		*/
		void setUniform(const std::string& name, unsigned int value);
		/*!
		 \brief Sets the \a vec provided to the uniform \a name.

		 \param[in] name A string containing the name of the uniform
		 \param[in] vec An ae::Vector2f containing the uniform's new values

		 \par Example:
		 \code
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->setUniform("uVec2f", ae::Vector2f(1.f, 0.f));
		 \endcode

		 \since v0.4.0
		*/
		void setUniform(const std::string& name, const Vector2f& vec);
		/*!
		 \brief Sets the \a vec provided to the uniform \a name.

		 \param[in] name A string containing the name of the uniform
		 \param[in] vec An ae::Vector3f containing the uniform's new values

		 \par Example:
		 \code
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->setUniform("uVec3f", ae::Vector3f(1.f, 0.f, -0.5f));
		 \endcode

		 \since v0.4.0
		*/
		void setUniform(const std::string& name, const Vector3f& vec);
		/*!
		 \brief Sets the \a vec provided to the uniform \a name.

		 \param[in] name A string containing the name of the uniform
		 \param[in] vec An ae::Vector4f containing the uniform's new values

		 \par Example:
		 \code
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->setUniform("uVec4f", ae::Vector4f(1.f, 0.f, -0.5f, 1.f));
		 \endcode

		 \since v0.4.0
		*/
		void setUniform(const std::string& name, const Vector4f& vec);
		/*!
		 \brief Sets the \a vec provided to the uniform \a name.

		 \param[in] name A string containing the name of the uniform
		 \param[in] vec An ae::Vector2i containing the uniform's new values

		 \par Example:
		 \code
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->setUniform("uVec2i", ae::Vector2i(1, 0));
		 \endcode

		 \since v0.4.0
		*/
		void setUniform(const std::string& name, const Vector2i& vec);
		/*!
		 \brief Sets the \a vec provided to the uniform \a name.

		 \param[in] name A string containing the name of the uniform
		 \param[in] vec An ae::Vector3i containing the uniform's new values

		 \par Example:
		 \code
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->setUniform("uVec3i", ae::Vector3i(1, 0, -5));
		 \endcode

		 \since v0.4.0
		*/
		void setUniform(const std::string& name, const Vector3i& vec);
		/*!
		 \brief Sets the \a vec provided to the uniform \a name.

		 \param[in] name A string containing the name of the uniform
		 \param[in] vec An ae::Vector4i containing the uniform's new values

		 \par Example:
		 \code
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->setUniform("uVec4i", ae::Vector4i(1, 0, -5, 1));
		 \endcode

		 \since v0.4.0
		*/
		void setUniform(const std::string& name, const Vector4i& vec);
		/*!
		 \brief Sets the \a mat provided to the uniform \a name.

		 \param[in] name A string containing the name of the uniform
		 \param[in] mat An ae::Matrix4f containing the uniform's new values

		 \par Example:
		 \code
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();
		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->setUniform("uMat4f", ae::Matrix4f::identity());
		 \endcode

		 \since v0.4.0
		*/
		void setUniform(const std::string& name, const Matrix4f& mat);

		// Public virtual method(s)
		/*!
		 \brief Deletes the OpenGL shader program that was created.
		 \note This method should only be used by the ae::Shader's manager.

		 \par Example:
		 \code
		 // Create the shader program and load one shader from source and one from a file
		 ae::Shader shader;
		 shader.loadFromFile(ae::Shader::StageType::Vertex, "Shaders/vertShader.vs");
		 shader.loadFromFile(ae::Shader::StageType::Fragment, "Shaders/fragShader.fs");

		 ...

		 // The destroy() and reload() methods are called by the ae::GLResourceFactory instance if the shader was created through it
		 shader.destroy(); // Destroy the shader program and its shader stages
		 shader.reload();  // Reload the shader program and its shader stages, and link them together
		 \endcode

		 \since v0.4.0
		*/
		virtual void destroy() const override final;
		/*!
		 \brief Binds the ae::Shader to the context indicating to OpenGL that we're about to use it.
		 \details The ae::Shader should be unbound at the end but it's not of paramount importance.
		 \note This method should only be called after having linked together all the attached shader stages.\n
		 A GL_INVALID_OPERATION will be generated if the ae::Shader hasn't been linked.

		 \par Example:
		 \code
		 // Retrieve the single instance of the GLResourceFactory
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();

		 // Create and configure a shader
		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->loadFromFile(ae::Shader::StageType::Vertex, "Shaders/vertShader.vs");
		 shader->loadFromFile(ae::Shader::StageType::Fragment, "Shaders/fragShader.fs");
		 shader->link();

		 shader->bind();
		 ...
		 shader->unbind();
		 \endcode

		 \sa unbind(), isBound(), link()

		 \since v0.4.0
		*/
		virtual void bind() const override final;
		/*!
		 \brief Unbounds the ae::Shader from the context indicating that we've finished using it.
		 \note Make sure that currently-bound shader program is the caller ae::Shader as this method will unbound any shader program.

		 \par Example:
		 \code
		 // Retrieve the single instance of the GLResourceFactory
		 ae::GLResourceFactory& glResourceFactory = ae::GLResourceFactory::getInstance();

		 // Create and configure a shader
		 std::shared_ptr<ae::Shader> shader = glResourceFactory.createShader();
		 shader->loadFromFile(ae::Shader::StageType::Vertex, "Shaders/vertShader.vs");
		 shader->loadFromFile(ae::Shader::StageType::Fragment, "Shaders/fragShader.fs");
		 shader->link();

		 shader->bind();
		 ...
		 shader->unbind();
		 \endcode

		 \sa bind(), isBound()

		 \since v0.4.0
		*/
		virtual void unbind() const override final;
	private:
		// Private method(s)
		/*!
		 \brief Compiles the source code of the shader associated to the \a handle provided.
		 \details The status of the compilation will automatically be checked.

		 \param[in] stage The OpenGL identifier of the shader stage

		 \sa checkShaderStatus()

		 \since v0.4.0
		*/
		void compileShader(const Stage& stage) const;
		/*!
		 \brief Caches and retrieves the uniform's location in the shader.

		 \param[in] name A string containing the name of the uniform in question

		 \return The location of the cached uniform or the one that was just retrieved, or -1 if it wasn't found

		 \since v0.4.0
		*/
		int cacheUniformLocation(const std::string& name);
		/*!
		 \brief Checks if the status of the shader program operation indicated was marked as successful.
		 \details A log will be generated if the operation failed.

		 \param[in] pname A GLenum of the operation to check

		 \sa checkShaderStatus()

		 \since v0.4.0
		*/
		void checkProgramStatus(unsigned int pname) const;
		/*!
		 \brief Checks if the status of the shader operation indicated was marked as successful.
		 \details A log will be generated if the operation failed.

		 \param[in] handle The OpenGL identifier of the shader stage
		 \param[in] pname A GLenum of the operation to check

		 \sa checkProgramStatus()

		 \since v0.4.0
		*/
		void checkShaderStatus(unsigned int handle, unsigned int pname) const;

	private:
		// Private member(s)
		std::map<StageType, Stage> mStages;   //!< The source code of all the loaded shader stages
		std::map<std::string, int> mUniforms; //!< The cached uniform names and locations
		LinkType                   mLinkType; //!< The link type of the shader
	};
}
#endif // Aeon_Graphics_Shader_H_

/*!
 \class ae::Shader
 \ingroup graphics

 The ae::Shader class represents an OpenGL shader program, the reason it wasn't
 called ShaderProgram is because that would would've been far too lengthy to be
 used repeatedly and most people associate a shader with a complete shader
 program even though the terminology is being misused.

 While it is possible for the API user to create their own shaders,
 pre-compiled shaders that are the most common can be found in the
 ae::ShaderManager singleton class that can immediately be used by the API user.

 All ae::Shader instances should be created via the ae::GLResourceFactory
 instance in order for it to be responsible for their lifetime and for the
 proper deallocation of all OpenGL resources. If it isn't created via the
 ae::GLResourceFactory instance, their management must be handled manually,
 meaning that the 'destroy()' method must be called when the resource is no
 longer needed.

 \author Filippos Gleglakos
 \version v0.4.0
 \date 2019.10.26
 \copyright MIT License
*/