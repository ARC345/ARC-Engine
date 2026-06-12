#include "arc_pch.h"
#include "ARC\Core\Core.h"
#include "OpenGLShader.h"
#include "glad\glad.h"
#include "ARC\Core\Log.h"
#include "ARC\Helpers\Helpers.h"
#include "ARC/Types/Color.h"

namespace arc {
	string opengl_shader::seperator = "#type";

	opengl_shader::opengl_shader(const string& name, const string& vertex_src, const string& fragment_src) :
		name_(name)
	{
		std::unordered_map<GLenum, string> sources;
		sources[GL_VERTEX_SHADER] = vertex_src;
		sources[GL_FRAGMENT_SHADER] = fragment_src;
		Compile(sources);
	}

	opengl_shader::opengl_shader(const string& path)
	{
		string source = shpr::ReadFile(path);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		name_ = shpr::ExtractFileNameFromPath(path);
	}

	opengl_shader::~opengl_shader()
	{
		glDeleteProgram(renderer_id_);
	}

	void opengl_shader::bind() const
	{
		glUseProgram(renderer_id_);
	}

	void opengl_shader::UnBind() const
	{
		glUseProgram(0);
	}

	unsigned int opengl_shader::ShaderTypeFromString(string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;
		
		ARC_CORE_ASSERT(0, "Invalid shader type specifier");
		return 0;
	}

	string opengl_shader::StringFromShaderType(unsigned int type)
	{
		switch (type)
		{
			case GL_VERTEX_SHADER:
				return "vertex";
			case GL_FRAGMENT_SHADER:
				return "fragment";
			default:
				ARC_CORE_ASSERT(0, "Invalid shader type specifier");
				return "";
		}
	}

	std::unordered_map<GLenum, string> opengl_shader::PreProcess(const string& source)
	{
		ARC_PROFILE_FUNCTION();
		std::unordered_map<GLenum, string> shaderSources;
		size_t pos = source.find(seperator.c_str(), 0);

		while (pos != string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos); // Finds first newline position
			ARC_CORE_ASSERT(eol != string::npos, "Syntax Error");
			size_t begin = pos + seperator.length() + 1; // Find first seperator
			string type = source.substr(begin, eol - begin);
			
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			ARC_CORE_ASSERT(nextLinePos != string::npos, "Syntax error");
			pos = source.find(seperator, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == string::npos ? source.length() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void opengl_shader::Compile(const std::unordered_map<GLenum, string>& shader_sources)
	{
		ARC_PROFILE_FUNCTION();
		// Get a program object.
		GLuint program = glCreateProgram();

		ARC_CORE_ASSERT(shader_sources.size() <= 2, "Only 2 shaders are supported currently");
		std::array<GLenum, 2> shaderHandles;
		size_t shaderHandleIndex = 0;

		for(auto&&[key, value]: shader_sources)
		{
			// create an empty shader handle
			GLuint shader = glCreateShader(key);

			// Send the shader source code to GL
			// Note that string's .CStr is NULL character terminated.
			const GLchar* source = value.c_str();
			glShaderSource(shader, 1, &source, 0);
		
			// Compile the shader
			glCompileShader(shader);
			GLint isCompiled = 0;
		
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				ARC_CORE_ERROR("{0}", infoLog.data());
				ARC_CORE_ASSERT(false, "{0} Shader compilation failiure!", StringFromShaderType(key).data());
				return;
			}

			// Vertex and fragment shaders are successfully compiled.
			// Now time to link them together into a program.
			// Attach our shaders to our program
			glAttachShader(program, shader);
			shaderHandles[shaderHandleIndex++] = shader;
		}

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.

			for(auto& shaderHandle : shaderHandles)
				glDeleteShader(shaderHandle);

			ARC_CORE_ERROR("{0}", infoLog.data());
			ARC_CORE_ASSERT(false, "Shader link failiure!");

			return;
		}

		renderer_id_ = program;

		// Always detach shaders after a successful link.
		for (auto& shaderHandle : shaderHandles)
		{	
			glDetachShader(program, shaderHandle);
			glDeleteShader(shaderHandle);
		}
	}

	void opengl_shader::set_mat4(const string& name, const float* value)
	{
		GLint location = glGetUniformLocation(renderer_id_, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, value);
	}

	void opengl_shader::set_mat3(const string& name, const float* value)
	{
		GLint location = glGetUniformLocation(renderer_id_, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, value);
	}

	void opengl_shader::set_int(const string& name, const int* value)
	{
		GLint location = glGetUniformLocation(renderer_id_, name.c_str());
		glUniform1iv(location, 1, value);
	}

	void opengl_shader::set_int2(const string& name, const int* value)
	{
		GLint location = glGetUniformLocation(renderer_id_, name.c_str());
		glUniform2iv(location, 1, value);
	}

	void opengl_shader::set_int3(const string& name, const int* value)
	{
		GLint location = glGetUniformLocation(renderer_id_, name.c_str());
		glUniform3iv(location, 1, value);
	}

	void opengl_shader::set_int4(const string& name, const int* value)
	{
		GLint location = glGetUniformLocation(renderer_id_, name.c_str());
		glUniform4iv(location, 1, value);
	}
	void opengl_shader::set_float(const string& name, const float* value)
	{
		GLint location = glGetUniformLocation(renderer_id_, name.c_str());
		glUniform1fv(location, 1, value);
	}

	void opengl_shader::set_float2(const string& name, const float* value)
	{
		GLint location = glGetUniformLocation(renderer_id_, name.c_str());
		glUniform2fv(location, 1, value);
	}

	void opengl_shader::set_float3(const string& name, const float* value)
	{
		GLint location = glGetUniformLocation(renderer_id_, name.c_str());
		glUniform3fv(location, 1, value);
	}

	void opengl_shader::set_float4(const string& name, const float* value)
	{
		GLint location = glGetUniformLocation(renderer_id_, name.c_str());
		glUniform4fv(location, 1, value);
	}

	void opengl_shader::set_int_array(const string& name, const int* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(renderer_id_, name.c_str());
		glUniform1iv(location, count, values);
	}

	void opengl_shader::set_float_array(const string& name, const float* values, uint32_t count)
	{
		GLint location = glGetUniformLocation(renderer_id_, name.c_str());
		glUniform1fv(location, count, values);
	}
}