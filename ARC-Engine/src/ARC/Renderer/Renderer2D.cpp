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
#include "glm/gtx/transform.hpp"
#include <algorithm>
#include "ARC/Wrappers/Glm.h"

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
		SQuadVertex* QuadVertexBufferBasePtr=nullptr;

		std::array<TRef<CTexture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		FGVec4 QuadVertexPositions[4];
		FGMat4 camTransform;

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
		
		auto* quad_indices = new uint32_t[s_Data.MaxIndices];

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
		FGMat4 Trans = _Camera.GetViewProjectionMatrix();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->Set<FGMat4>("u_ViewProjection", _Camera.GetViewProjectionMatrix());
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferBasePtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;

		s_Data.camTransform = Trans;
	}

	void CRenderer2D::BeginScene(const CCamera& _Camera, const FTransform2D& _Transform)
	{
		FGMat4 Trans = _Camera.GetProjection() * glm::inverse(SHPR::Conv<FGMat4>(_Transform));

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->Set<FGMat4>("u_ViewProjection", Trans);
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferBasePtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;
		s_Data.camTransform = Trans;
	}

	template <int Length, typename T>
	struct Block
	{
		T n_[Length];
	};
	void CRenderer2D::EndScene()
	{
		ARC_PROFILE_FUNCTION();

		auto data_size = uint32_t((uint8_t*)s_Data.QuadVertexBufferBasePtr- (uint8_t*)s_Data.QuadVertexBufferBase);

		std::sort((Block<4, SQuadVertex>*)s_Data.QuadVertexBufferBase, (Block<4, SQuadVertex>*)s_Data.QuadVertexBufferBasePtr,
			[&](const Block<4, SQuadVertex>& p1, const Block<4, SQuadVertex>& p2) -> bool {
				FGVec3 tmp = FGVec3(s_Data.camTransform[3]) - p1.n_[0].Position;
				FGVec3 tmp2 = FGVec3(s_Data.camTransform[3]) - p2.n_[0].Position;
				return p1.n_[0].Position[2] < p2.n_[0].Position[2];
			});

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

	void CRenderer2D::DrawQuad(const FVec3& _Position, const float _Rotation, const FVec2& _Size, const FColor4& _Color, const TRef<CTexture2D>& _Tex, const FVec2& _TextureScaling)
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
			 	glm::translate(FGMat4(1.0f), FGVec3(_Position.x, _Position.y, _Position.z)) *
			 	glm::rotate(FGMat4(1.0f), _Rotation, FGVec3(0, 0, 1)) *
			 	glm::scale(FGMat4(1.0f), FGVec3(_Size.x, _Size.y, 1.0f));

		for (size_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferBasePtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferBasePtr->Color = FGVec4(_Color.r, _Color.g, _Color.b, _Color.a);
			s_Data.QuadVertexBufferBasePtr->TexCoord = CTexture2D::TexCoords[i];
			s_Data.QuadVertexBufferBasePtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferBasePtr->TexScaling = FGVec2(_TextureScaling.x, _TextureScaling.y);
			s_Data.QuadVertexBufferBasePtr++;
		}

		s_Data.QuadIndexCount += 6;

		++s_Data.Statistics.QuadCount;
	}

	void CRenderer2D::DrawQuad(const FVec3& _Position, const float _Rotation, const FVec2& _Size, const FColor4& _Color, const TRef<CSubTexture2D>& _SubTex, const FVec2& _TextureScaling)
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
			glm::translate(FGMat4(1.0f), FGVec3(_Position.x, _Position.y, _Position.z)) *
			glm::rotate(FGMat4(1.0f), _Rotation, FGVec3(0, 0, 1)) *
			glm::scale(FGMat4(1.0f), FGVec3(_Size.x, _Size.y, 1.0f));

		for (size_t i = 0; i < 4; i++)
		{
			s_Data.QuadVertexBufferBasePtr->Position = transform * s_Data.QuadVertexPositions[i];
			s_Data.QuadVertexBufferBasePtr->Color = FGVec4(_Color.r, _Color.g, _Color.b, _Color.a);
			s_Data.QuadVertexBufferBasePtr->TexCoord = _SubTex->GetTexCoords()[i];
			s_Data.QuadVertexBufferBasePtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferBasePtr->TexScaling = FGVec2(_TextureScaling.x, _TextureScaling.y);
			s_Data.QuadVertexBufferBasePtr++;
		}

		s_Data.QuadIndexCount += 6;

		++s_Data.Statistics.QuadCount;
	}


	void CRenderer2D::DrawQuad(const CPrimitive2D& Quad)
	{
		ARC_PROFILE_FUNCTION();

		DrawQuad(Quad.GetLocation(), Quad.GetRotation(), Quad.GetScale(), Quad.Color, Quad.Texture ? Quad.Texture : s_Data.WhiteTexture, Quad.TextureScaling);
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
		s_Data.QuadVertexBufferBasePtr = s_Data.QuadVertexBufferBase;
		s_Data.TextureSlotIndex = 1;
	}
}