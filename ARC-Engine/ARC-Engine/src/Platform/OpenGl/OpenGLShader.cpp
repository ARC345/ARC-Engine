#include "arc_pch.h"
#include "ARC\Core\Core.h"
#include "OpenGLShader.h"
#include "glad\glad.h"
#include "ARC\Core\Log.h"
#include "ARC\Helpers\Helpers.h"
#include "ARC\Renderer\Color.h"

namespace ARC {
	std::string COpenGLShader::s_Seperator = "#type";

	COpenGLShader::COpenGLShader(const std::string& _Name, const std::string& _VertexSrc, const std::string& _FragmentSrc) :
		m_Name(_Name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = _VertexSrc;
		sources[GL_FRAGMENT_SHADER] = _FragmentSrc;
		Compile(sources);
	}

	COpenGLShader::COpenGLShader(const std::string& _Path)
	{
		std::string source = HPR::IO::ReadFile(_Path);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		m_Name = HPR::IO::ExtractFileNameFromPath(_Path);
	}

	COpenGLShader::~COpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void COpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void COpenGLShader::UnBind() const
	{
		glUseProgram(0);
	}

	unsigned int COpenGLShader::ShaderTypeFromString(std::string& _Type)
	{
		if (_Type == "vertex")
			return GL_VERTEX_SHADER;
		if (_Type == "fragment" || _Type == "pixel")
			return GL_FRAGMENT_SHADER;
		
		ARC_CORE_ASSERT(0, "Invalid shader type specifier");
		return 0;
	}

	std::string COpenGLShader::StringFromShaderType(unsigned int _Type)
	{
		switch (_Type)
		{
			case GL_VERTEX_SHADER:
				return "vertex";
			case GL_FRAGMENT_SHADER:
				return "fragment";
			default:
				ARC_CORE_ASSERT(0, "Invalid shader type specifier");
				return 0;
		}
	}

	std::unordered_map<GLenum, std::string> COpenGLShader::PreProcess(const std::string& _Source)
	{
		ARC_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> shaderSources;
		size_t pos = _Source.find(s_Seperator.c_str(), 0);

		while (pos != std::string::npos)
		{
			size_t eol = _Source.find_first_of("\r\n", pos); // finds first newline position
			ARC_CORE_ASSERT(eol != std::string::npos, "Syntax Error");
			size_t begin = pos + s_Seperator.length() + 1; // find first s_Seperator
			std::string type = _Source.substr(begin, eol - begin);
			
			size_t nextLinePos = _Source.find_first_not_of("\r\n", eol);
			ARC_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax error");
			pos = _Source.find(s_Seperator, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = _Source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? _Source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}

	void COpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& _ShaderSources)
	{
		ARC_PROFILE_FUNCTION();
		// Get a program object.
		GLuint program = glCreateProgram();

		ARC_CORE_ASSERT(_ShaderSources.size() <= 2, "Only 2 shaders are supported currently");
		std::array<GLenum, 2> shaderHandles;
		size_t shaderHandleIndex = 0;

		for(auto&&[key, value]: _ShaderSources)
		{
			// Create an empty shader handle
			GLuint shader = glCreateShader(key);

			// Send the shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
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
				ARC_CORE_ASSERT(false, "{0} Shader compilation failiure!", StringFromShaderType(key));
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

		m_RendererID = program;

		// Always detach shaders after a successful link.
		for (auto& shaderHandle : shaderHandles)
		{	
			glDetachShader(program, shaderHandle);
			glDeleteShader(shaderHandle);
		}
	}

	void COpenGLShader::SetMat4(const std::string& _Name, const float* _Value)
	{
		GLint location = glGetUniformLocation(m_RendererID, _Name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, _Value);
	}

	void COpenGLShader::SetMat3(const std::string& _Name, const float* _Value)
	{
		GLint location = glGetUniformLocation(m_RendererID, _Name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, _Value);
	}

	void COpenGLShader::SetInt(const std::string& _Name, const int* _Value)
	{
		GLint location = glGetUniformLocation(m_RendererID, _Name.c_str());
		glUniform1iv(location, 1, _Value);
	}

	void COpenGLShader::SetInt2(const std::string& _Name, const int* _Value)
	{
		GLint location = glGetUniformLocation(m_RendererID, _Name.c_str());
		glUniform2iv(location, 1, _Value);
	}

	void COpenGLShader::SetInt3(const std::string& _Name, const int* _Value)
	{
		GLint location = glGetUniformLocation(m_RendererID, _Name.c_str());
		glUniform3iv(location, 1, _Value);
	}

	void COpenGLShader::SetInt4(const std::string& _Name, const int* _Value)
	{
		GLint location = glGetUniformLocation(m_RendererID, _Name.c_str());
		glUniform4iv(location, 1, _Value);
	}
	void COpenGLShader::SetFloat(const std::string& _Name, const float* _Value)
	{
		GLint location = glGetUniformLocation(m_RendererID, _Name.c_str());
		glUniform1fv(location, 1, _Value);
	}

	void COpenGLShader::SetFloat2(const std::string& _Name, const float* _Value)
	{
		GLint location = glGetUniformLocation(m_RendererID, _Name.c_str());
		glUniform2fv(location, 1, _Value);
	}

	void COpenGLShader::SetFloat3(const std::string& _Name, const float* _Value)
	{
		GLint location = glGetUniformLocation(m_RendererID, _Name.c_str());
		glUniform3fv(location, 1, _Value);
	}

	void COpenGLShader::SetFloat4(const std::string& _Name, const float* _Value)
	{
		GLint location = glGetUniformLocation(m_RendererID, _Name.c_str());
		glUniform4fv(location, 1, _Value);
	}

	void COpenGLShader::SetIntArray(const std::string& _Name, const int* _Values, uint32_t _Count)
	{
		GLint location = glGetUniformLocation(m_RendererID, _Name.c_str());
		glUniform1iv(location, _Count, _Values);
	}

	void COpenGLShader::SetFloatArray(const std::string& _Name, const float* _Values, uint32_t _Count)
	{
		GLint location = glGetUniformLocation(m_RendererID, _Name.c_str());
		glUniform1fv(location, _Count, _Values);
	}
}