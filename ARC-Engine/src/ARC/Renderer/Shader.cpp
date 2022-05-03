#include "arc_pch.h"
#include "Renderer.h"
#include "Platform\OpenGl\OpenGLShader.h"
#include "ARC\Core\Core.h"
#include "ARC/Types/Color.h"
#include "ARC\Types\Glm.h"
#include "glm\gtx\compatibility.hpp"
#include "glm\gtc\type_ptr.inl"

namespace ARC {
	TRef<CShader> CShader::Create(const std::string& _Name, const std::string& _VertexSrc, const std::string& _FragmentSrc)
	{
		switch (CRenderer::GetCurrentAPI())
		{
		case CRendererAPI::ERendererAPI::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (ERendererAPI::None)");
			return nullptr;
		case CRendererAPI::ERendererAPI::OpenGL:
			return CreateRef<COpenGLShader>(_Name, _VertexSrc, _FragmentSrc);
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}

	TRef<CShader> CShader::Create(const std::string& _Path)
	{
		switch (CRenderer::GetCurrentAPI())
		{
		case CRendererAPI::ERendererAPI::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (ERendererAPI::None)");
			return nullptr;
		case CRendererAPI::ERendererAPI::OpenGL:
			return CreateRef<COpenGLShader>(_Path);
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}

	void CShaderLibrary::Add(const TRef<CShader>& _Shader)
	{
		auto& name = _Shader->GetName();
		Add(name, _Shader);
	}

	void CShaderLibrary::Add(const std::string& _Name, const TRef<CShader>& _Shader)
	{
		ARC_CORE_ASSERT(!Exists(_Name), "Shader Already Exists");
		m_Shaders[_Name] = _Shader;
	}

	TRef<CShader> CShaderLibrary::Load(const std::string& _Path, const std::string& _Name /*= ""*/)
	{
		ARC_PROFILE_FUNCTION();
		auto shader = CShader::Create(_Path);
		if(_Name == "") Add(shader);
		else Add(_Name, shader);
		return shader;
	}

	TRef<CShader> CShaderLibrary::GetShader(const std::string& _Name)
	{
		ARC_CORE_ASSERT(Exists(_Name), "Shader Not Found");
		return m_Shaders[_Name];
	}

	bool CShaderLibrary::Exists(const std::string& _Name) const
	{
		return m_Shaders.find(_Name) != m_Shaders.end();
	}

	template<typename T>
	void CShader::Set(const std::string& _Name, const T& _Value)
	{
		ARC_CORE_WARN("Uploading {0} to Shader is currently not supported", typeid(T).name());
	}

	template<>
	void CShader::Set<FVec4>(const std::string& _Name, const FVec4& _Value)
	{
		SetFloat4(_Name, _Value.Data());
	}
	template<>
	void CShader::Set<FVec3>(const std::string& _Name, const FVec3& _Value)
	{
		SetFloat3(_Name, _Value.Data());
	}
	template<>
	void CShader::Set<FVec2>(const std::string& _Name, const FVec2& _Value)
	{
		SetFloat2(_Name, _Value.Data());
	}
	template<>
	void CShader::Set<CColor>(const std::string& _Name, const CColor& _Value)
	{
		SetFloat4(_Name, _Value.Data());
	}
	template<>
	void CShader::Set<FGVec4>(const std::string& _Name, const glm::vec4& _Value)
	{
		SetFloat4(_Name, glm::value_ptr(_Value));
	}
	template<>
	void CShader::Set<FGMat4>(const std::string& _Name, const glm::mat4& _Value)
	{
		SetMat4(_Name, glm::value_ptr(_Value));
	}
	template<>
	void CShader::Set<FGMat3>(const std::string& _Name, const glm::mat3& _Value)
	{
		SetMat3(_Name, glm::value_ptr(_Value));
	}
	template<>
	void CShader::Set<float>(const std::string& _Name, const float& _Value)
	{
		SetFloat(_Name, &_Value);
	}
	template<>
	void CShader::Set<glm::float2>(const std::string& _Name, const glm::float2& _Value)
	{
		SetFloat2(_Name, glm::value_ptr(_Value));
	}
	template<>
	void CShader::Set<glm::float3>(const std::string& _Name, const glm::float3& _Value)
	{
		SetFloat3(_Name, glm::value_ptr(_Value));
	}
	template<>
	void CShader::Set<int>(const std::string& _Name, const int& _Value)
	{
		SetInt(_Name, &_Value);
	}

	template<typename T>
	void CShader::SetArray(const std::string& _Name, const T* _Value, uint32_t _Count)
	{
		ARC_CORE_WARN("Uploading {0} Array to Shader is currently not supported", typeid(T).name());
	}

	template<>
	void CShader::SetArray<int>(const std::string& _Name, const int* _Value, uint32_t _Count)
	{
		SetIntArray(_Name, _Value, _Count);
	}
	template<>
	void CShader::SetArray<float>(const std::string& _Name, const float* _Value, uint32_t _Count)
	{
		SetFloatArray( _Name, _Value, _Count);
	}
}