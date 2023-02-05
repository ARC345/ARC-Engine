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

		uint32_t TranslucentQuadIndexCount = 0;
		SQuadVertex* TranslucentQuadVertexBufferBase=nullptr;
		SQuadVertex* TranslucentQuadVertexBufferBasePtr=nullptr;

		uint32_t OpaqueQuadIndexCount = 0;
		SQuadVertex* OpaqueQuadVertexBufferBase = nullptr;
		SQuadVertex* OpaqueQuadVertexBufferBasePtr = nullptr;

		std::array<TRef<CTexture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		FGVec4 QuadVertexPositions[4];
		FGMat4 CameraTransform;

		CRenderer2D::SStatistics Statistics;

	};
	static SRenderer2DData s_Data;

	void CRenderer2D::Init()
	{
		ARC_PROFILE_FUNCTION();

		//--------------------------------+[Code for square]+--------------------------------//

		//float squarepverts[5 * 4] = {
		//	- 0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		//	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		//	 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		//	-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		//};
		//unsigned int quadpindices[] = { 0, 1, 2, 2, 3, 0 };
		
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
		s_Data.OpaqueQuadVertexBufferBase = new SQuadVertex[s_Data.MaxVertices];
		s_Data.TranslucentQuadVertexBufferBase = new SQuadVertex[s_Data.MaxVertices];
		
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

	void CRenderer2D::BeginScene(const COrthographicCamera& pCamera)
	{
		FGMat4 Trans = pCamera.GetViewProjectionMatrix();

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->Set<FGMat4>("u_ViewProjection", pCamera.GetViewProjectionMatrix());
		
		s_Data.TranslucentQuadIndexCount = 0;
		s_Data.TranslucentQuadVertexBufferBasePtr = s_Data.TranslucentQuadVertexBufferBase;

		s_Data.OpaqueQuadIndexCount = 0;
		s_Data.OpaqueQuadVertexBufferBasePtr = s_Data.OpaqueQuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;

		s_Data.CameraTransform = Trans;
	}

	void CRenderer2D::BeginScene(const CCamera& pCamera, const FTransform2D& pTransform)
	{
		FGMat4 Trans = pCamera.GetProjection() * glm::inverse(SHPR::Conv<FGMat4>(pTransform));

		s_Data.TextureShader->Bind();
		s_Data.TextureShader->Set<FGMat4>("u_ViewProjection", Trans);
		
		s_Data.TranslucentQuadIndexCount = 0;
		s_Data.TranslucentQuadVertexBufferBasePtr = s_Data.TranslucentQuadVertexBufferBase;

		s_Data.OpaqueQuadIndexCount = 0;
		s_Data.OpaqueQuadVertexBufferBasePtr = s_Data.OpaqueQuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
		s_Data.CameraTransform = Trans;
	}

	void CRenderer2D::EndScene_Translucent()
	{
		ARC_PROFILE_FUNCTION();
		auto data_size = uint32_t((uint8_t*)s_Data.TranslucentQuadVertexBufferBasePtr - (uint8_t*)s_Data.TranslucentQuadVertexBufferBase);

		struct Block { SQuadVertex n_[4]; };

		std::sort((Block*)s_Data.TranslucentQuadVertexBufferBase, (Block*)s_Data.TranslucentQuadVertexBufferBasePtr,
			[&](const Block& p1, const Block& p2) -> bool {
				FGVec3 tmp = FGVec3(s_Data.CameraTransform[3]) - p1.n_[0].Position;
				FGVec3 tmp2 = FGVec3(s_Data.CameraTransform[3]) - p2.n_[0].Position;
				return p1.n_[0].Position[2] < p2.n_[0].Position[2];
			});

		s_Data.QuadVertexBuffer->SetData(s_Data.TranslucentQuadVertexBufferBase, data_size);
		FlushTranslucent();
	}

	void CRenderer2D::EndScene_Opaque()
	{
		ARC_PROFILE_FUNCTION();
		auto data_size = uint32_t((uint8_t*)s_Data.OpaqueQuadVertexBufferBasePtr - (uint8_t*)s_Data.OpaqueQuadVertexBufferBase);
		s_Data.QuadVertexBuffer->SetData(s_Data.OpaqueQuadVertexBufferBase, data_size);
		FlushOpaque();
	}
	
	void CRenderer2D::EndScene()
	{
		EndScene_Opaque();
		EndScene_Translucent();
	}

	void CRenderer2D::FlushTranslucent()
	{
		ARC_PROFILE_FUNCTION();

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		if (s_Data.TranslucentQuadIndexCount != 0) {
			++s_Data.Statistics.DrawCalls;
			CRenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.TranslucentQuadIndexCount);
		}
	}
	void CRenderer2D::FlushOpaque()
	{
		ARC_PROFILE_FUNCTION();

		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			s_Data.TextureSlots[i]->Bind(i);

		if (s_Data.OpaqueQuadIndexCount != 0) {
			++s_Data.Statistics.DrawCalls;
			CRenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.OpaqueQuadIndexCount);
		}
	}

	void CRenderer2D::DrawQuad(const FVec3& pPosition, const float pRotation, const FVec2& pSize, const ETransparencyType pTransparencyLevel, const FColor4& pColor, const TRef<CTexture2D>& pTex, const FVec2& pTextureScaling)
	{
		ARC_PROFILE_FUNCTION();

		if (s_Data.TranslucentQuadIndexCount >= SRenderer2DData::MaxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;

		if (pTex != nullptr)
		{
			for (size_t i = 1; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i].get() == *pTex.get())
				{
					textureIndex = (float)i;
					break;
				}
			}
			if (textureIndex == 0.0f)
			{
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = pTex;
				s_Data.TextureSlotIndex++;
			}
		}

		FGMat4 transform =
			 	glm::translate(FGMat4(1.0f), FGVec3(pPosition.x, pPosition.y, pPosition.z)) *
			 	glm::rotate(FGMat4(1.0f), pRotation, FGVec3(0, 0, 1)) *
			 	glm::scale(FGMat4(1.0f), FGVec3(pSize.x, pSize.y, 1.0f));

		switch (pTransparencyLevel)
		{
		case ETransparencyType::Opaque:
			for (size_t i = 0; i < 4; i++)
			{
				s_Data.OpaqueQuadVertexBufferBasePtr->Position = transform * s_Data.QuadVertexPositions[i];
				s_Data.OpaqueQuadVertexBufferBasePtr->Color = FGVec4(pColor.r, pColor.g, pColor.b, pColor.a);
				s_Data.OpaqueQuadVertexBufferBasePtr->TexCoord = CTexture2D::TexCoords[i];
				s_Data.OpaqueQuadVertexBufferBasePtr->TexIndex = textureIndex;
				s_Data.OpaqueQuadVertexBufferBasePtr->TexScaling = FGVec2(pTextureScaling.x, pTextureScaling.y);
				s_Data.OpaqueQuadVertexBufferBasePtr++;
			}
			s_Data.OpaqueQuadIndexCount += 6;

			break;
		case ETransparencyType::Translucent:
			for (size_t i = 0; i < 4; i++)
			{
				s_Data.TranslucentQuadVertexBufferBasePtr->Position = transform * s_Data.QuadVertexPositions[i];
				s_Data.TranslucentQuadVertexBufferBasePtr->Color = FGVec4(pColor.r, pColor.g, pColor.b, pColor.a);
				s_Data.TranslucentQuadVertexBufferBasePtr->TexCoord = CTexture2D::TexCoords[i];
				s_Data.TranslucentQuadVertexBufferBasePtr->TexIndex = textureIndex;
				s_Data.TranslucentQuadVertexBufferBasePtr->TexScaling = FGVec2(pTextureScaling.x, pTextureScaling.y);
				s_Data.TranslucentQuadVertexBufferBasePtr++;
			}
			s_Data.TranslucentQuadIndexCount += 6;

			break;
		}

		++s_Data.Statistics.QuadCount;
	}

	void CRenderer2D::DrawQuad(const FVec3& pPosition, const float pRotation, const FVec2& pSize, const ETransparencyType pTransparencyLevel, const FColor4& pColor, const TRef<CSubTexture2D>& pSubTex, const FVec2& pTextureScaling)
	{

		if (s_Data.OpaqueQuadIndexCount >= SRenderer2DData::MaxIndices)
			FlushAndReset_Opaque();
		if (s_Data.TranslucentQuadIndexCount >= SRenderer2DData::MaxIndices)
			FlushAndReset_Translucent();

		float textureIndex = 0.0f;

		if (pSubTex != nullptr)
		{
			for (size_t i = 1; i < s_Data.TextureSlotIndex; i++)
			{
				if (*s_Data.TextureSlots[i].get() == *pSubTex->GetTexture().get())
				{
					textureIndex = (float)i;
					break;
				}
			}
			if (textureIndex == 0.0f)
			{
				textureIndex = (float)s_Data.TextureSlotIndex;
				s_Data.TextureSlots[s_Data.TextureSlotIndex] = pSubTex->GetTexture();
				s_Data.TextureSlotIndex++;
			}
		}

		FGMat4 transform =
			glm::translate(FGMat4(1.0f), FGVec3(pPosition.x, pPosition.y, pPosition.z)) *
			glm::rotate(FGMat4(1.0f), pRotation, FGVec3(0, 0, 1)) *
			glm::scale(FGMat4(1.0f), FGVec3(pSize.x, pSize.y, 1.0f));
		
		switch (pTransparencyLevel)
		{
		case ETransparencyType::Opaque:
			for (size_t i = 0; i < 4; i++)
			{
				s_Data.OpaqueQuadVertexBufferBasePtr->Position = transform * s_Data.QuadVertexPositions[i];
				s_Data.OpaqueQuadVertexBufferBasePtr->Color = FGVec4(pColor.r, pColor.g, pColor.b, pColor.a);
				s_Data.OpaqueQuadVertexBufferBasePtr->TexCoord = pSubTex->GetTexCoords()[i];
				s_Data.OpaqueQuadVertexBufferBasePtr->TexIndex = textureIndex;
				s_Data.OpaqueQuadVertexBufferBasePtr->TexScaling = FGVec2(pTextureScaling.x, pTextureScaling.y);
				s_Data.OpaqueQuadVertexBufferBasePtr++;
			}
			s_Data.OpaqueQuadIndexCount += 6;
			break;
		case ETransparencyType::Translucent:
			for (size_t i = 0; i < 4; i++)
			{
				s_Data.TranslucentQuadVertexBufferBasePtr->Position = transform * s_Data.QuadVertexPositions[i];
				s_Data.TranslucentQuadVertexBufferBasePtr->Color = FGVec4(pColor.r, pColor.g, pColor.b, pColor.a);
				s_Data.TranslucentQuadVertexBufferBasePtr->TexCoord = pSubTex->GetTexCoords()[i];
				s_Data.TranslucentQuadVertexBufferBasePtr->TexIndex = textureIndex;
				s_Data.TranslucentQuadVertexBufferBasePtr->TexScaling = FGVec2(pTextureScaling.x, pTextureScaling.y);
				s_Data.TranslucentQuadVertexBufferBasePtr++;
			}
			s_Data.TranslucentQuadIndexCount += 6;

			break;
		}

		++s_Data.Statistics.QuadCount;
	}


	void CRenderer2D::DrawQuad(const CPrimitive2D& Quad)
	{
		ARC_PROFILE_FUNCTION();

		DrawQuad(Quad.GetLocation(), Quad.GetRotation(), Quad.GetScale(), Quad.TransparencyLevel, Quad.Color, Quad.Texture ? Quad.Texture : s_Data.WhiteTexture, Quad.TextureScaling);
	}

	CRenderer2D::SStatistics CRenderer2D::GetStats()
	{
		return s_Data.Statistics;
	}

	void CRenderer2D::ResetStats()
	{
		memset(&s_Data.Statistics, 0, sizeof(SStatistics));
	}

	void CRenderer2D::FlushAndReset_Translucent()
	{
		EndScene_Translucent();
		s_Data.TranslucentQuadIndexCount = 0;
		s_Data.TranslucentQuadVertexBufferBasePtr = s_Data.TranslucentQuadVertexBufferBase;
	}
	void CRenderer2D::FlushAndReset_Opaque()
	{
		EndScene_Opaque();
		s_Data.OpaqueQuadIndexCount = 0;
		s_Data.OpaqueQuadVertexBufferBasePtr = s_Data.OpaqueQuadVertexBufferBase;
	}

	void CRenderer2D::FlushAndReset()
	{
		EndScene();
		s_Data.TranslucentQuadIndexCount = 0;
		s_Data.TranslucentQuadVertexBufferBasePtr = s_Data.TranslucentQuadVertexBufferBase;

		s_Data.OpaqueQuadIndexCount = 0;
		s_Data.OpaqueQuadVertexBufferBasePtr = s_Data.OpaqueQuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}
}