#include "arc_pch.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "ARC\Objects\CameraBase.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Platform\OpenGl\OpenGLShader.h"

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

	void CRenderer::Submit(const TRef<CShader>& _Shader, const TRef<CVertexArray>& _VertexArray, const glm::mat4& _Transform)
	{
		_Shader->Bind();
		std::dynamic_pointer_cast<COpenGLShader>(_Shader)->UploadUniform<glm::mat4>("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<COpenGLShader>(_Shader)->UploadUniform<glm::mat4>("u_Transform", _Transform);
		_VertexArray->Bind();
		CRenderCommand::DrawIndexed(_VertexArray);
	}

}