#pragma once

#include "ARC/Renderer/Shader.h"
#include <unordered_map>

#include "glm/glm.hpp"
namespace ARC {
	class COpenGLShader :
		public CShader
	{
	public:
		COpenGLShader(const TString& _Name, const TString& _VertexSrc, const TString& _FragmentSrc);
		COpenGLShader(const TString& _Path);

		virtual ~COpenGLShader();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual inline const TString& GetName() const override { return m_Name; };

	private:
		static unsigned int ShaderTypeFromString(TString& _Type);
		static TString StringFromShaderType(unsigned int _Type);

		std::unordered_map<unsigned int, TString> PreProcess(const TString& _Source);
		void Compile(const std::unordered_map<unsigned int, TString>& _ShaderSources);
	private:
		static TString s_Seperator;
		TString m_Name;
		uint32_t m_RendererID;
	protected:
		virtual void SetInt(const TString& _Name, const int* _Value) override;
		virtual void SetInt2(const TString& _Name, const int* _Value) override;
		virtual void SetInt3(const TString& _Name, const int* _Value) override;
		virtual void SetInt4(const TString& _Name, const int* _Value) override;
		virtual void SetMat4(const TString& _Name, const float* _Value) override;
		virtual void SetMat3(const TString& _Name, const float* _Value) override;
		virtual void SetFloat(const TString& _Name, const float* _Value) override;
		virtual void SetFloat2(const TString& _Name, const float* _Value) override;
		virtual void SetFloat3(const TString& _Name, const  float*  _Value) override;
		virtual void SetFloat4(const TString& _Name, const  float* _Value) override;

		virtual void SetIntArray(const TString& _Name, const int* _Values, uint32_t _Count) override;
		virtual void SetFloatArray(const TString& _Name, const float* _Values, uint32_t _Count) override;

	};
}
