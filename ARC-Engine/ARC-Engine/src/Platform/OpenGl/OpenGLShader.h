#pragma once

#include "ARC/Renderer/Shader.h"
#include <unordered_map>
#include <string>

#include "glm/glm.hpp"
namespace ARC {
	class COpenGLShader :
		public CShader
	{
	public:
		COpenGLShader(const std::string& _Name, const std::string& _VertexSrc, const std::string& _FragmentSrc);
		COpenGLShader(const std::string& _Path);

		virtual ~COpenGLShader();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual inline const std::string& GetName() const override { return m_Name; };

	private:
		static unsigned int ShaderTypeFromString(std::string& _Type);
		static std::string StringFromShaderType(unsigned int _Type);

		std::unordered_map<unsigned int, std::string> PreProcess(const std::string& _Source);
		void Compile(const std::unordered_map<unsigned int, std::string>& _ShaderSources);
	private:
		static std::string s_Seperator;
		std::string m_Name;
		uint32_t m_RendererID;
	protected:
		virtual void SetInt(const std::string& _Name, const int* _Value) override;
		virtual void SetInt2(const std::string& _Name, const int* _Value) override;
		virtual void SetInt3(const std::string& _Name, const int* _Value) override;
		virtual void SetInt4(const std::string& _Name, const int* _Value) override;
		virtual void SetMat4(const std::string& _Name, const float* _Value) override;
		virtual void SetMat3(const std::string& _Name, const float* _Value) override;
		virtual void SetFloat(const std::string& _Name, const float* _Value) override;
		virtual void SetFloat2(const std::string& _Name, const float* _Value) override;
		virtual void SetFloat3(const std::string& _Name, const  float*  _Value) override;
		virtual void SetFloat4(const std::string& _Name, const  float* _Value) override;

		virtual void SetIntArray(const std::string& _Name, const int* _Values, uint32_t _Count) override;
		virtual void SetFloatArray(const std::string& _Name, const float* _Values, uint32_t _Count) override;

	};
}
