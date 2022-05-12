#include "arc_pch.h"
#include "Renderer2D.h"
#include "Shader.h"
#include "Platform/OpenGl/OpenGLShader.h"
#include "RenderCommand.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "ARC/Renderer/OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"
#include "ARC/Objects/Primitive2D.h"
#include "Camera.h"
#include "ARC/Types/Glm.h"
#include "glm/gtx/transform.hpp"

namespace ARC {
	struct SQuadVertex
	{
		FGVec3 Position;
		FGVec4 Color;
		FGVec2 TexCoord;
		FGVec2 TexScaling;
		float TexIndex;
	};

	struct SRenderer2DData {
		//@TODO : Load from ini file
		static constexpr uint32_t MaxQuads = 1000;
		static constexpr uint32_t MaxVertices = MaxQuads * 4;
		static constexpr uint32_t MaxIndices = MaxQuads * 6;
		static constexpr uint32_t MaxTextureSlots = 32;
		TRef<CVertexArray> QuadVertexArray;
		TRef<CTexture2D> WhiteTexture;

		TRef<CVertexBuffer> QuadVertexBuffer;

		TRef<CShader> TextureShader;

		uint32_t QuadIndexCount=0;

		SQuadVertex* QuadVertexBufferBase=nullptr;
		SQuadVertex* QuadVertexBufferPtr=nullptr;

		std::array<TRef<CTexture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		FGVec4 QuadVertexPositions[4];

		CRenderer2D::SStatistics Statistics;

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
			{ EShaderDataType::Float2, "a_TexCoord" },
			{ EShaderDataType::Float2, "a_TexScaling" },
			{ EShaderDataType::Float, "a_TexIndex" },
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

		int32_t samplers[s_Data.MaxTextureSlots];
		for (int32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.TextureShader = CShader::Create("assets/shaders/Texture.glsl");
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->SetArray<int>("u_Textures", samplers, s_Data.MaxTextureSlots);
		//--------------------------------~[Code for square]~--------------------------------//

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;
		s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f,  0.0f,  1.0f};
		s_Data.QuadVertexPositions[1] = {  0.5f, -0.5f,  0.0f,  1.0f};
		s_Data.QuadVertexPositions[2] = {  0.5f,  0.5f,  0.0f,  1.0f};
		s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f,  0.0f,  1.0f};
	}

	void CRenderer2D::Shutdown()
	{
	}

	void CRenderer2D::BeginScene(const COrthographicCamera& _Camera)
	{
		s_Data.TextureShader->Bind();
		s_Data.TextureShader->Set<FGMat4>("u_ViewProjection", _Camera.GetViewProjectionMatrix());
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;
	}

	void CRenderer2D::BeginScene(const CCamera& _Camera, const FTransform2D& _Transform)
	{
		FGMat4 Trans = _Camera.GetProjection() * glm::inverse(_Transform.To<FGMat4>());

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->Set<FGMat4>("u_ViewProjection", Trans);
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;
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
		if (s_Data.QuadIndexCount == 0)
			return; // Nothing to draw
		++s_Data.Statistics.DrawCalls;

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);
		
		CRenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
	}

	void CRenderer2D::DrawQuad(const FVec2& _Position, const float _Rotation, const FVec2& _Size, float _ZOrder, const CColor& _Color, const TRef<CTexture2D>& _Tex, const FVec2& _TextureScaling)
	{
		ARC_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount >= SRenderer2DData::MaxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;

		if (_Tex != nullptr)
		{
			for (size_t i = 1; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i].get() == *_Tex.get())
				{
					textureIndex = (float)i;
					break;
				}
			}
			if (textureIndex == 0.0f)
			{
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = _Tex;
				s_Data.TextureSlotIndex++;
			}
		}

		FGMat4 transform =
			 	glm::translate(FGMat4(1.0f), FGVec3(_Position.x, _Position.y, _ZOrder)) *
			 	glm::rotate(FGMat4(1.0f), _Rotation, FGVec3(0, 0, 1)) *
			 	glm::scale(FGMat4(1.0f), FGVec3(_Size.x, _Size.y, 1.0f));

		for (size_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = FGVec4(_Color.r, _Color.g, _Color.b, _Color.a);
			s_Data.QuadVertexBufferPtr->TexCoord = CTexture2D::TexCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TexScaling = FGVec2(_TextureScaling.x, _TextureScaling.y);
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		++s_Data.Statistics.QuadCount;
	}
	void CRenderer2D::DrawQuad(const FVec2& _Position, const float _Rotation, const FVec2& _Size, float _ZOrder, const CColor& _Color, const TRef<CSubTexture2D>& _SubTex, const FVec2& _TextureScaling)
	{
		if (s_Data.QuadIndexCount >= SRenderer2DData::MaxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;

		if (_SubTex != nullptr)
		{
			for (size_t i = 1; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i].get() == *_SubTex->GetTexture().get())
				{
					textureIndex = (float)i;
					break;
				}
			}
			if (textureIndex == 0.0f)
			{
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = _SubTex->GetTexture();
				s_Data.TextureSlotIndex++;
			}
		}

		FGMat4 transform =
			glm::translate(FGMat4(1.0f), FGVec3(_Position.x, _Position.y, _ZOrder)) *
			glm::rotate(FGMat4(1.0f), _Rotation, FGVec3(0, 0, 1)) *
			glm::scale(FGMat4(1.0f), FGVec3(_Size.x, _Size.y, 1.0f));

		for (size_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferPtr->Color = FGVec4(_Color.r, _Color.g, _Color.b, _Color.a);
			s_Data.QuadVertexBufferPtr->TexCoord = _SubTex->GetTexCoords()[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TexScaling = FGVec2(_TextureScaling.x, _TextureScaling.y);
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		++s_Data.Statistics.QuadCount;
	}


	void CRenderer2D::DrawQuad(const CPrimitive2D& Quad)
	{
		ARC_PROFILE_FUNCTION();

		DrawQuad(Quad.GetLocation(), Quad.GetRotation(), Quad.GetScale(), Quad.GetTransform().ZOrder, Quad.Color, Quad.Texture ? Quad.Texture : s_Data.WhiteTexture, Quad.TextureScaling);
	}

	CRenderer2D::SStatistics CRenderer2D::GetStats()
	{
		return s_Data.Statistics;
	}

	void CRenderer2D::ResetStats()
	{
		memset(&s_Data.Statistics, 0, sizeof(SStatistics));
	}

	void CRenderer2D::FlushAndReset()
	{
		EndScene();
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;
	}
}