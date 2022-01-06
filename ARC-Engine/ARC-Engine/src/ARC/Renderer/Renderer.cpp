#include "arc_pch.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "ARC\Objects\CameraBase.h"
#include "Shader.h"
#include "VertexArray.h"

namespace ARC {
	CRenderer::SSceneData* CRenderer::m_SceneData = new CRenderer::SSceneData;

	void CRenderer::BeginScene(COrthographicCameraBase& _Cam)
	{
		// Submit View Projection Matrix
		m_SceneData->ViewProjectionMatrix = _Cam.GetViewProjectionMatrix();
	}

	void CRenderer::EndScene()
	{

	}

	void CRenderer::Submit(const std::shared_ptr<CShader>& _Shader, const std::shared_ptr<CVertexArray>& _VertexArray)
	{
		_Shader->Bind();
		_Shader->UploadUniform4Mat("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		_VertexArray->Bind();
		CRenderCommand::DrawIndexed(_VertexArray);
	}

}