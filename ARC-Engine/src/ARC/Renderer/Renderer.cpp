#include "arc_pch.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "ARC\Objects\CameraBase.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Platform\OpenGl\OpenGLShader.h"
#include "ARC\Types\vector.h"
#include "Renderer2D.h"

namespace ARC {
	CRenderer::SSceneData* CRenderer::m_SceneData = new CRenderer::SSceneData;

	void CRenderer::Init()
	{
		ARC_PROFILE_FUNCTION();

		CRenderCommand::Init();
		CRenderer2D::Init();
	}

	void CRenderer::OnWindowResize(TVec2<uint32_t> _Dimentions)
	{
		CRenderCommand::SetViewport(TVec2<uint32_t>::ZeroVector, _Dimentions);
	}

	void CRenderer::BeginScene(COrthographicCameraBase& _Cam)
	{
		// Submit View Projection Matrix
		m_SceneData->ViewProjectionMatrix = _Cam.GetViewProjectionMatrix();
	}

	void CRenderer::EndScene()
	{

	}

	void CRenderer::Submit(const TRef<CShader>& _Shader, const TRef<CVertexArray>& _VertexArray, const glm::mat4& _Transform)
	{
		_Shader->Bind();
		_Shader->Set<glm::mat4>("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		_Shader->Set<glm::mat4>("u_Transform", _Transform);
		_VertexArray->Bind();
		CRenderCommand::DrawIndexed(_VertexArray);
	}

}