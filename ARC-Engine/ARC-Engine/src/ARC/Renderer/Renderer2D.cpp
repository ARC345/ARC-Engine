#include "arc_pch.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "Platform\OpenGl\OpenGLShader.h"
#include "RenderCommand.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "ARC\Objects\CameraBase.h"
#include "glm\ext\matrix_transform.hpp"
#include "glm\gtx\rotate_vector.inl"
#include "Texture.h"

namespace ARC {
	static SRenderer2DStorage* s_Data;

	void CRenderer2D::Init()
	{
		s_Data = new SRenderer2DStorage();

		//--------------------------------+[Code for square]+--------------------------------//
		TRef<CVertexBuffer> square_vertex_buffer;
		TRef<CIndexBuffer> square_index_buffer;

		float square_verts[5 * 4] = {
			- 0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		unsigned int square_indices[] = { 0, 1, 2, 2, 3, 0 };
		
		s_Data->QuadVertexArray = CVertexArray::Create();
		s_Data->FlatColorShader = CShader::Create("assets/shaders/FlatColorShader.glsl");
		s_Data->TextureShader	= CShader::Create("assets/shaders/Texture.glsl");
		square_index_buffer		= CIndexBuffer::Create(square_indices, sizeof(square_indices) / sizeof(uint32_t));
		square_vertex_buffer	= CVertexBuffer::Create(square_verts, sizeof(square_verts));


		CBufferLayout square_layout = {
			 { EShaderDataType::Float3, "a_Position" }, 
			 { EShaderDataType::Float2, "a_TexCoord" }
		};

		square_vertex_buffer->SetLayout(square_layout);

		s_Data->QuadVertexArray->AddVertexBuffer(square_vertex_buffer);
		s_Data->QuadVertexArray->SetIndexBuffer(square_index_buffer);

		s_Data->TextureShader->Bind();
		s_Data->TextureShader->Set<int>("u_Texture", 0);
		//--------------------------------~[Code for square]~--------------------------------//

	}

	void CRenderer2D::Shutdown()
	{
		delete s_Data;
	}

	void CRenderer2D::BeginScene(const COrthographicCameraBase& _Camera)
	{
		s_Data->FlatColorShader->Bind();
		s_Data->FlatColorShader->Set<glm::mat4>("u_ViewProjection", _Camera.GetViewProjectionMatrix());
		
		s_Data->TextureShader->Bind();
		s_Data->TextureShader->Set<glm::mat4>("u_ViewProjection", _Camera.GetViewProjectionMatrix());
	}

	void CRenderer2D::EndScene()
	{
	}

	void CRenderer2D::DrawQuad(const TVec2<> _Position, const float _Rotation, const TVec2<> _Size, int _ZOrder, CColor _Color)
	{
		s_Data->FlatColorShader->Bind();

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), glm::vec3(_Position.x, _Position.y, (float)_ZOrder)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(_Rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(_Size.x, _Size.y, 1.0f));
		s_Data->FlatColorShader->Set<glm::mat4>("u_Transform", transform);
		s_Data->FlatColorShader->Set<CColor>("u_Color", _Color);

		s_Data->QuadVertexArray->Bind();
		CRenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}

	void CRenderer2D::DrawQuad(const TVec2<> _Position, const float _Rotation, const TVec2<> _Size, int _ZOrder, TRef<CTexture2D> _Tex)
	{
		s_Data->TextureShader->Bind();

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), glm::vec3(_Position.x, _Position.y, (float)_ZOrder)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(_Rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(_Size.x, _Size.y, 1.0f));
		s_Data->TextureShader->Set<glm::mat4>("u_Transform", transform);

		_Tex->Bind();

		s_Data->QuadVertexArray->Bind();
		CRenderCommand::DrawIndexed(s_Data->QuadVertexArray);
	}
}