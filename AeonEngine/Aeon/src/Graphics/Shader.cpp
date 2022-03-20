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

#include <AEON/Graphics/Shader.h>

#include <GL/glew.h>

#include <AEON/System/FileSystem.h>
#include <AEON/Graphics/internal/GLCommon.h>
#include <AEON/Graphics/internal/UniformBuffer.h>

namespace ae
{
	// Public constructor(s)
	Shader::Shader(LinkType linkType)
		: GLResource()
		, mStages()
		, mUniforms()
		, mDataLayout()
		, mLinkType(linkType)
	{
		// Create the shader program object
		mHandle = GLCall(glCreateProgram());
	}

	// Public method(s)
	void Shader::loadFromSource(StageType type, const std::string& source)
	{
		// Check if a shader stage of the same type has already been attached and that the source provided is valid (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (mStages.find(type) != mStages.end()) {
				AEON_LOG_ERROR("Attempt to insert shader stage of the same type", "The shader already possesses a shader stage of the type provided.\nAborting operation.");
				return;
			}
			if (source.empty()) {
				AEON_LOG_ERROR("The source code provided is invalid", "The string containing the source code is empty.\nAborting operation.");
				return;
			}
		}

		// Create the shader object and compile it
		unsigned int stageHandle = GLCall(glCreateShader(type));
		auto stage = mStages.emplace(type, Stage({ source, stageHandle })).first;
		compileShader(stage->second);

		// Attach the shader stage to the shader program
		GLCall(glAttachShader(mHandle, stageHandle));
	}

	void Shader::loadFromFile(StageType type, const std::string& filename)
	{
		// Read in the contents of the file
		const std::string SOURCE = FileSystem::readFile(filename);

		// Check that the contents of the file are valid (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (SOURCE.empty()) {
				AEON_LOG_ERROR("The source code retrieved is invalid", "The filepath of the source code is either invalid or its contents are empty.\nAborting operation.");
				return;
			}
		}

		// Create and attach the shader object from the source code retrieved
		loadFromSource(type, SOURCE);
	}

	void Shader::link() const
	{
		// Verify that at least two shader stages have been attached (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (mStages.size() < 2) {
				AEON_LOG_ERROR("Incomplete shader program", "At least two shader stages have to be attached before linking a program, a vertex shader and a fragment shader.\nAborting operation.");
				return;
			}
		}

		// Indicate to OpenGL that we intend to use a separable shader program
		if (mLinkType == LinkType::Separable) {
			GLCall(glProgramParameteri(mHandle, GL_PROGRAM_SEPARABLE, GL_TRUE));
		}

		// Link together all the attached shader stages
		GLCall(glLinkProgram(mHandle));
		checkProgramStatus(GL_LINK_STATUS);
		GLCall(glValidateProgram(mHandle));
		checkProgramStatus(GL_VALIDATE_STATUS);

		// Raise the shader stages' deletion flag and detach all shader stages as they're no longer needed
		for (const auto& stage : mStages) {
			GLCall(glDeleteShader(stage.second.handle));
			checkShaderStatus(stage.second.handle, GL_DELETE_STATUS);
			GLCall(glDetachShader(mHandle, stage.second.handle));
		}
	}

	void Shader::reload()
	{
		// Create a new OpenGL shader program
		mHandle = GLCall(glCreateProgram());

		// Reload and reattach the shader stages
		for (auto& stage : mStages) {
			stage.second.handle = GLCall(glCreateShader(stage.first));
			compileShader(stage.second);
			GLCall(glAttachShader(mHandle, stage.second.handle));
		}

		// Link together the shader program
		link();
	}

	bool Shader::isBound() const
	{
		// Retrieve the identifier of the currently bound shader program
		GLint boundProgram;
		GLCall(glGetIntegerv(GL_CURRENT_PROGRAM, &boundProgram));

		return boundProgram == mHandle;
	}

	void Shader::addUniformBuffer(const UniformBuffer& ubo)
	{
		// Retrieve the uniform block's index from the shader and assign a buffer binding to the uniform block
		GLuint blockIndex = GLCall(glGetUniformBlockIndex(mHandle, ubo.getBlockName().c_str()));
		GLCall(glUniformBlockBinding(mHandle, blockIndex, ubo.getBindingPoint()));
	}

	void Shader::setUniform(const std::string& name, float value)
	{
		GLint location = cacheUniformLocation(name);
		if (location != -1) {
			GLCall(glProgramUniform1f(mHandle, location, value));
		}
	}

	void Shader::setUniform(const std::string& name, int value)
	{
		GLint location = cacheUniformLocation(name);
		if (location != -1) {
			GLCall(glProgramUniform1i(mHandle, location, value));
		}
	}

	void Shader::setUniform(const std::string& name, unsigned int value)
	{
		GLint location = cacheUniformLocation(name);
		if (location != -1) {
			GLCall(glProgramUniform1ui(mHandle, location, value));
		}
	}

	void Shader::setUniform(const std::string& name, const Vector2f& vec)
	{
		GLint location = cacheUniformLocation(name);
		if (location != -1) {
			GLCall(glProgramUniform2fv(mHandle, location, 1, vec.elements.data()));
		}
	}

	void Shader::setUniform(const std::string& name, const Vector3f& vec)
	{
		GLint location = cacheUniformLocation(name);
		if (location != -1) {
			GLCall(glProgramUniform3fv(mHandle, location, 1, vec.elements.data()));
		}
	}

	void Shader::setUniform(const std::string& name, const Vector4f& vec)
	{
		GLint location = cacheUniformLocation(name);
		if (location != -1) {
			GLCall(glProgramUniform4fv(mHandle, location, 1, vec.elements.data()));
		}
	}

	void Shader::setUniform(const std::string& name, const Vector2i& vec)
	{
		GLint location = cacheUniformLocation(name);
		if (location != -1) {
			GLCall(glProgramUniform2iv(mHandle, location, 1, vec.elements.data()));
		}
	}

	void Shader::setUniform(const std::string& name, const Vector3i& vec)
	{
		GLint location = cacheUniformLocation(name);
		if (location != -1) {
			GLCall(glProgramUniform3iv(mHandle, location, 1, vec.elements.data()));
		}
	}

	void Shader::setUniform(const std::string& name, const Vector4i& vec)
	{
		GLint location = cacheUniformLocation(name);
		if (location != -1) {
			GLCall(glProgramUniform4iv(mHandle, location, 1, vec.elements.data()));
		}
	}

	void Shader::setUniform(const std::string& name, const Matrix4f& mat)
	{
		GLint location = cacheUniformLocation(name);
		if (location != -1) {
			GLCall(glProgramUniformMatrix4fv(mHandle, location, 1, GL_FALSE, mat.getElements().data()));
		}
	}

	// Public virtual method(s)
	void Shader::destroy() const
	{
		// Check if the OpenGL identifier is valid (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (!mHandle) {
				AEON_LOG_ERROR("Attempt to delete invalid shader program", "The shader program's OpenGL identifier is invalid.\nAborting operation.");
				return;
			}
		}

		// Delete the OpenGL identifier
		GLCall(glDeleteProgram(mHandle));

		// Check if it was successfully deleted only if it was currently bound (ignored in Release mode)
		if _CONSTEXPR_IF (AEON_DEBUG) {
			if (isBound()) {
				checkProgramStatus(GL_DELETE_STATUS);
			}
		}
	}

	void Shader::bind() const
	{
		// Check that the OpenGL identifier is valid (ignored in Release mode)
		if _CONSTEXPR_IF(AEON_DEBUG) {
			if (!mHandle) {
				AEON_LOG_ERROR("Attempt to bind invalid shader program", "The shader program doesn't possess a valid OpenGL identifier.\nAborting operation.");
				return;
			}
		}

		GLCall(glUseProgram(mHandle));
	}

	void Shader::unbind() const
	{
		// Check that the currently-bound shader program is the caller (ignored in Release mode)
		if _CONSTEXPR_IF(AEON_DEBUG) {
			if (!isBound()) {
				AEON_LOG_WARNING("Unbinding different shader program", "A shader program was unbound through another shader program.");
			}
		}

		GLCall(glUseProgram(0));
	}

	// Private method(s)
	void Shader::compileShader(const Stage& stage) const
	{
		// Load the source code
		const GLchar* const sourceContent[] = { stage.source.c_str() };
		GLCall(glShaderSource(stage.handle, 1, sourceContent, nullptr));

		// Compile the shader and check if it successfully compiled
		GLCall(glCompileShader(stage.handle));
		checkShaderStatus(stage.handle, GL_COMPILE_STATUS);
	}

	int Shader::cacheUniformLocation(const std::string& name)
	{
		// Return the cached uniform's location or retrieve it if it's not known
		auto uniformItr = mUniforms.find(name);
		if (uniformItr == mUniforms.end()) {
			GLint uniformLocation = GLCall(glGetUniformLocation(mHandle, name.c_str()));
			if (uniformLocation != -1) {
				mUniforms.emplace(name, uniformLocation);
			}
			return uniformLocation;
		}

		return uniformItr->second;
	}

	void Shader::checkProgramStatus(unsigned int pname) const
	{
		// Find the name associated to the operation that was performed
		std::string statusName = "";
		switch (pname)
		{
		case GL_LINK_STATUS:
			statusName = "link";
			break;
		case GL_VALIDATE_STATUS:
			statusName = "validate";
			break;
		case GL_DELETE_STATUS:
			statusName = "delete";
			break;
		default:
			AEON_LOG_ERROR("Invalid OpenGL value provided", "The GLenum provided is invalid.\nStatus wasn't checked.");
			return;
		}

		// Retrieve the result of the operation
		GLint status;
		GLCall(glGetProgramiv(mHandle, pname, &status));
		if (status == GL_FALSE) {
			GLint length;
			GLCall(glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length));

			// Log an error message
			std::vector<GLchar> error(length);
			GLCall(glGetProgramInfoLog(mHandle, length, &length, &error[0]));
			AEON_LOG_ERROR("Failed to " + statusName + " shader program", &error[0]);
		}
	}

	void Shader::checkShaderStatus(unsigned int handle, unsigned int pname) const
	{
		// Find the name associated to the operation that was performed
		std::string statusName = "";
		switch (pname)
		{
		case GL_COMPILE_STATUS:
			statusName = "compile";
			break;
		case GL_DELETE_STATUS:
			statusName = "delete";
			break;
		default:
			AEON_LOG_ERROR("Invalid OpenGL value provided", "The GLenum provided is invalid.\nStatus wasn't checked.");
			return;
		}

		// Retrieve the result of the operation
		GLint status;
		GLCall(glGetShaderiv(handle, pname, &status));
		if (status == GL_FALSE) {
			GLint length;
			GLCall(glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &length));

			// Log an error message
			std::vector<GLchar> error(length);
			GLCall(glGetShaderInfoLog(handle, length, &length, &error[0]));
			AEON_LOG_ERROR("Failed to " + statusName + " shader stage", &error[0]);
		}
	}
}