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
	struct SQuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		// TODO: texid
	};

	struct SRenderer2DData {
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		TRef<CVertexArray> QuadVertexArray;
		TRef<CTexture2D> WhiteTexture;

		TRef<CVertexBuffer> QuadVertexBuffer;

		TRef<CShader> TextureShader;

		uint32_t QuadIndexCount=0;

		SQuadVertex* QuadVertexBufferBase=nullptr;
		SQuadVertex* QuadVertexBufferPtr=nullptr;
	};
	static SRenderer2DData s_Data;

	void CRenderer2D::Init()
	{
		ARC_PROFILE_FUNCTION();

		//--------------------------------+[Code for square]+--------------------------------//

		//float square_verts[5 * 4] = {
		//	- 0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		//	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		//	 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		//	-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		//};
		//unsigned int quad_indices[] = { 0, 1, 2, 2, 3, 0 };
		
		s_Data.QuadVertexArray = CVertexArray::Create();
		s_Data.QuadVertexBuffer = CVertexBuffer::Create(s_Data.MaxVertices * sizeof(SQuadVertex));
		s_Data.QuadVertexBuffer->SetLayout({
			{ EShaderDataType::Float3, "a_Position" },
			{ EShaderDataType::Float4, "a_Color" },
			{ EShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);
		s_Data.QuadVertexBufferBase = new SQuadVertex[s_Data.MaxVertices];
		
		uint32_t* quad_indices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quad_indices[i + 0] = offset + 0;
			quad_indices[i + 1] = offset + 1;
			quad_indices[i + 2] = offset + 2;

			quad_indices[i + 3] = offset + 2;
			quad_indices[i + 4] = offset + 3;
			quad_indices[i + 5] = offset + 0;

			offset += 4;
		}
		TRef<CIndexBuffer> quad_ib = CIndexBuffer::Create(quad_indices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quad_ib);
		
		// @TODO make thread safe
		delete[] quad_indices;

		s_Data.WhiteTexture	= CTexture2D::Create(TVec2<uint32_t>(1, 1));
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));


		s_Data.TextureShader = CShader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->Set<int>("u_Texture", 0);
		//--------------------------------~[Code for square]~--------------------------------//

	}

	void CRenderer2D::Shutdown()
	{
	}

	void CRenderer2D::BeginScene(const COrthographicCameraBase& _Camera)
	{
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->Set<glm::mat4>("u_ViewProjection", _Camera.GetViewProjectionMatrix());
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
	}

	void CRenderer2D::EndScene()
	{
		ARC_PROFILE_FUNCTION();
		auto data_size = uint32_t((uint8_t*)s_Data.QuadVertexBufferPtr- (uint8_t*)s_Data.QuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, data_size);
		
		Flush();
	}

	void CRenderer2D::Flush()
	{
		ARC_PROFILE_FUNCTION();
		CRenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}

	void CRenderer2D::DrawQuad(const FVec2 _Position, const float _Rotation, const FVec2 _Size, float _ZOrder, const CColor _Color)
	{
		ARC_PROFILE_FUNCTION();
		//s_Data.FlatColorShader->Bind();
		//s_Data.FlatColorShader->Set<CColor>("u_Color", _Color);
		s_Data.QuadVertexBufferPtr->Position = { _Position.x(), _Position.y(), 0.f };
		//s_Data.QuadVertexBufferPtr->ZOrder = _ZOrder;
		s_Data.QuadVertexBufferPtr->Color = glm::vec4(_Color.r(), _Color.g(), _Color.b(), _Color.a());
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f, 0.f };
		s_Data.QuadVertexBufferPtr++;
		
		s_Data.QuadVertexBufferPtr->Position = { _Position.x() + _Size.x(), _Position.y(), 0.f };
		//s_Data.QuadVertexBufferPtr->ZOrder = 0.f;
		s_Data.QuadVertexBufferPtr->Color = glm::vec4(_Color.r(), _Color.g(), _Color.b(), _Color.a());
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f, 0.f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { _Position.x() + _Size.x(), _Position.y()+_Size.y(), 0.f };
		//s_Data.QuadVertexBufferPtr->ZOrder = 0.f;
		s_Data.QuadVertexBufferPtr->Color = glm::vec4(_Color.r(), _Color.g(), _Color.b(), _Color.a());
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.f, 1.f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadVertexBufferPtr->Position = { _Position.x(), _Position.y() + _Size.y(), 0.f};
		//s_Data.QuadVertexBufferPtr->ZOrder = 0.f;
		s_Data.QuadVertexBufferPtr->Color = glm::vec4(_Color.r(), _Color.g(), _Color.b(), _Color.a());
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.f, 1.f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount+=6;
		// s_Data.WhiteTexture->Bind();
		// 
		// glm::mat4 transform =
		// 	glm::translate(glm::mat4(1.0f), glm::vec3(_Position.x(), _Position.y(), _ZOrder)) *
		// 	glm::rotate(glm::mat4(1.0f), _Rotation, glm::vec3(0, 0, 1)) *
		// 	glm::scale(glm::mat4(1.0f), glm::vec3(_Size.x(), _Size.y(), 1.0f));
		// s_Data.TextureShader->Set<glm::mat4>("u_Transform", transform);
		// 
		// s_Data.QuadVertexArray->Bind();
		// CRenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}

	void CRenderer2D::DrawQuad(const FVec2 _Position, const float _Rotation, const FVec2 _Size, float _ZOrder, const FVec2 _TilingFactor, const CColor _Color, const TRef<CTexture2D> _Tex)
	{
		ARC_PROFILE_FUNCTION();
		s_Data.TextureShader->Set<CColor>("u_Color", _Color);
		s_Data.TextureShader->Set<FVec2>("u_TextureScaling", _TilingFactor);
		 
		_Tex->Bind();

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), glm::vec3(_Position.x(), _Position.y(), _ZOrder)) *
			//glm::rotate(glm::mat4(1.0f), _Rotation, glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(_Size.x(), _Size.y(), 1.0f));
		s_Data.TextureShader->Set<glm::mat4>("u_Transform", transform);

		s_Data.QuadVertexArray->Bind();
		CRenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}

	void CRenderer2D::DrawQuad(CPrimitive2D& Quad)
	{
		ARC_PROFILE_FUNCTION();
		s_Data.TextureShader->Set<CColor>("u_Color", Quad.Color);

		if (Quad.Texture != nullptr) {
			Quad.Texture->Bind();
			s_Data.TextureShader->Set<FVec2>("u_TextureScaling", Quad.TilingFactor);
		}
		else s_Data.WhiteTexture->Bind();

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), glm::vec3(Quad.GetLocation().x(), Quad.GetLocation().y(), Quad.Transform.ZOrder)) *
			//glm::rotate(glm::mat4(1.0f), Quad.GetRotation(), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(Quad.GetScale().x(), Quad.GetScale().y(), 1.0f));
		s_Data.TextureShader->Set<glm::mat4>("u_Transform", transform);

		s_Data.QuadVertexArray->Bind();
		CRenderCommand::DrawIndexed(s_Data.QuadVertexArray);
	}
}