#include "arc_pch.h"
#include "Renderer.h"
#include "Platform\OpenGl\OpenGLShader.h"
#include "ARC\Core\Core.h"

namespace ARC {
	TRef<CShader> CShader::Create(const std::string& _Name, const std::string& _VertexSrc, const std::string& _FragmentSrc)
	{
		switch (CRenderer::GetCurrentAPI())
		{
		case CRendererAPI::ERendererAPI::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (ERendererAPI::None)");
			return nullptr;
		case CRendererAPI::ERendererAPI::OpenGL:
			return std::make_shared<COpenGLShader>(_Name, _VertexSrc, _FragmentSrc);
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
			return std::make_shared<COpenGLShader>(_Path);
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
}