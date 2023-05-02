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
#include "../Scene/EditorCamera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "ARC/Scene/Entity.h"
namespace ARC {
	struct SQuadVertex
	{
		FGLMVec3 Position;
		FGLMVec4 Color;
		FGLMVec2 TexCoord;
		FGLMVec2 TexScaling;
		int TexIndex;
		int EntityId;
	};
	struct SCircleVertex
	{
		FGLMVec3 WorldPosition;
		FGLMVec3 LocalPosition;
		FGLMVec4 Color;
		float Thickness;
		float Sharpness;
		int EntityId;
	};
	struct SLineVertex
	{
		FGLMVec3 Position;
		FGLMVec4 Color;
		int EntityId;
	};
	struct SRenderer2DData {
		//@TODO : Load from ini file
		static constexpr TUInt32 MaxQuads = 1000;
		static constexpr TUInt32 MaxVertices = MaxQuads * 4;
		static constexpr TUInt32 MaxIndices = MaxQuads * 6;
		static constexpr TUInt32 MaxTextureSlots = 32;
		
		struct SQuadData {
			TRef<CVertexArray> VertexArray;
			TRef<CVertexBuffer> VertexBuffer;
			TRef<CShader> Shader;

			TUInt32 TranslucentIndexCount = 0;
			SQuadVertex* TranslucentVertexBufferBase = nullptr;
			SQuadVertex* TranslucentVertexBufferBasePtr = nullptr;

			TUInt32 OpaqueIndexCount = 0;
			SQuadVertex* OpaqueVertexBufferBase = nullptr;
			SQuadVertex* OpaqueVertexBufferBasePtr = nullptr;

			FGLMVec4 VertexPositions[4];
		} Quad;
		struct SCircleData {
			TRef<CVertexArray> VertexArray;
			TRef<CVertexBuffer> VertexBuffer;
			TRef<CShader> Shader;

			TUInt32 TranslucentIndexCount = 0;
			SCircleVertex* TranslucentVertexBufferBase = nullptr;
			SCircleVertex* TranslucentVertexBufferBasePtr = nullptr;

			FGLMVec4 VertexPositions[4];
		} Circle;
		struct SLineData {
			TRef<CVertexArray> VertexArray;
			TRef<CVertexBuffer> VertexBuffer;
			TRef<CShader> Shader;

			float Thickness= 2;

			TUInt32 IndexCount = 0;
			SLineVertex* VertexBufferBase = nullptr;
			SLineVertex* VertexBufferBasePtr = nullptr;
		} Line;

		TRef<CTexture2D> WhiteTexture;
		std::array<TRef<CTexture2D>, MaxTextureSlots> TextureSlots;
		TUInt32 TextureSlotIndex = 1;

		FGLMMat4 CameraTransform;

		SRenderer2D::SStatistics Statistics;

	};
	static SRenderer2DData sData;

	void SRenderer2D::Init()
	{
		ARC_PROFILE_FUNCTION();

		sData.WhiteTexture = CTexture2D::Create(TVec2<TUInt32>(1, 1));
		TUInt32 whiteTextureData = 0xffffffff;
		sData.WhiteTexture->SetData(&whiteTextureData, sizeof(TUInt32));

		int32_t samplers[sData.MaxTextureSlots];
		for (int32_t i = 0; i < sData.MaxTextureSlots; i++)
			samplers[i] = i;
		//--------------------------------+[Code for Quad]+--------------------------------//
		
		sData.Quad.VertexArray = CVertexArray::Create();
		sData.Quad.VertexBuffer = CVertexBuffer::Create(sData.MaxVertices * sizeof(SQuadVertex));
		sData.Quad.VertexBuffer->SetLayout({
			{ EShaderDataType::Float3, "a_Position" },
			{ EShaderDataType::Float4, "a_Color" },
			{ EShaderDataType::Float2, "a_TexCoord" },
			{ EShaderDataType::Float2, "a_TexScaling" },
			{ EShaderDataType::Int, "a_TexIndex" },
			{ EShaderDataType::Int, "a_EntityId" }
			});
		sData.Quad.VertexArray->AddVertexBuffer(sData.Quad.VertexBuffer);
		sData.Quad.OpaqueVertexBufferBase = new SQuadVertex[sData.MaxVertices];
		sData.Quad.TranslucentVertexBufferBase = new SQuadVertex[sData.MaxVertices];
		
		auto* quad_indices = new TUInt32[sData.MaxIndices];

		TUInt32 offset = 0;
		for (TUInt32 i = 0; i < sData.MaxIndices; i += 6)
		{
			quad_indices[i+ 0] = offset + 0;
			quad_indices[i+ 1] = offset + 1;
			quad_indices[i+ 2] = offset + 2;

			quad_indices[i+ 3] = offset + 2;
			quad_indices[i+ 4] = offset + 3;
			quad_indices[i+ 5] = offset + 0;

			offset += 4;
		}
		TRef<CIndexBuffer> quad_ib = CIndexBuffer::Create(quad_indices, sData.MaxIndices);
		sData.Quad.VertexArray->SetIndexBuffer(quad_ib);
		
		// @TODO make thread safe
		delete[] quad_indices;

		sData.Quad.Shader = CShader::Create("assets/shaders/R2D_Quad.glsl");
		sData.Quad.Shader->Bind();
		sData.Quad.Shader->SetArray<int>("u_Textures", samplers, sData.MaxTextureSlots);

		sData.TextureSlots[0] = sData.WhiteTexture;
		sData.Quad.VertexPositions[0] = { -0.5f, -0.5f,  0.0f,  1.0f};
		sData.Quad.VertexPositions[1] = {  0.5f, -0.5f,  0.0f,  1.0f};
		sData.Quad.VertexPositions[2] = {  0.5f,  0.5f,  0.0f,  1.0f};
		sData.Quad.VertexPositions[3] = { -0.5f,  0.5f,  0.0f,  1.0f}; 
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~-[Code for Quad]-~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		//-------------------------------+[Code for Circle]+-------------------------------//
		sData.Circle.VertexArray = CVertexArray::Create();
		sData.Circle.VertexBuffer = CVertexBuffer::Create(sData.MaxVertices * sizeof(SCircleVertex));
		sData.Circle.VertexBuffer->SetLayout({
			{ EShaderDataType::Float3, "a_WorldPosition" },
			{ EShaderDataType::Float3, "a_LocalPosition" },
			{ EShaderDataType::Float4, "a_Color" },
			{ EShaderDataType::Float, "a_Thickness" },
			{ EShaderDataType::Float, "a_Sharpness" },
			{ EShaderDataType::Int, "a_EntityId" }
			});
		sData.Circle.VertexArray->AddVertexBuffer(sData.Circle.VertexBuffer);
		sData.Circle.TranslucentVertexBufferBase = new SCircleVertex[sData.MaxVertices];
		sData.Circle.VertexArray->SetIndexBuffer(quad_ib); // reuse quad one as same

		sData.Circle.Shader = CShader::Create("assets/shaders/R2D_Circle.glsl");
		sData.Circle.Shader->Bind();

		sData.Circle.VertexPositions[0] = { -0.5f, -0.5f,  0.0f,  1.0f };
		sData.Circle.VertexPositions[1] = { 0.5f, -0.5f,  0.0f,  1.0f };
		sData.Circle.VertexPositions[2] = { 0.5f,  0.5f,  0.0f,  1.0f };
		sData.Circle.VertexPositions[3] = { -0.5f,  0.5f,  0.0f,  1.0f };
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~-[Code for Circle]-~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		//--------------------------------+[Code for Line]+--------------------------------//
		sData.Line.VertexArray = CVertexArray::Create();
		sData.Line.VertexBuffer = CVertexBuffer::Create(sData.MaxVertices * sizeof(SLineVertex));
		sData.Line.VertexBuffer->SetLayout({
			{ EShaderDataType::Float3, "a_Position" },
			{ EShaderDataType::Float4, "a_Color" },
			{ EShaderDataType::Int, "a_EntityId" }
			});
		sData.Line.VertexArray->AddVertexBuffer(sData.Line.VertexBuffer);
		sData.Line.VertexBufferBase = new SLineVertex[sData.MaxVertices];

		sData.Line.Shader = CShader::Create("assets/shaders/R2D_Line.glsl");
		sData.Line.Shader->Bind();
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~-[Code for Line]-~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


	}

	void SRenderer2D::Shutdown()
	{
	}

	void SRenderer2D::BeginScene(const COrthographicCamera& pCamera)
	{
		FGLMMat4 Trans = pCamera.GetViewProjectionMatrix();

		sData.Quad.Shader->Bind();
		sData.Quad.Shader->Set<FGLMMat4>("u_ViewProjection", pCamera.GetViewProjectionMatrix());
		
		sData.Quad.TranslucentIndexCount = 0;
		sData.Quad.TranslucentVertexBufferBasePtr = sData.Quad.TranslucentVertexBufferBase;

		sData.Quad.OpaqueIndexCount = 0;
		sData.Quad.OpaqueVertexBufferBasePtr = sData.Quad.OpaqueVertexBufferBase;

		sData.Circle.Shader->Bind();
		sData.Circle.Shader->Set<FGLMMat4>("u_ViewProjection", Trans);

		sData.Circle.TranslucentIndexCount = 0;
		sData.Circle.TranslucentVertexBufferBasePtr = sData.Circle.TranslucentVertexBufferBase;

		sData.TextureSlotIndex = 1;
		sData.CameraTransform = Trans;
	}

	void SRenderer2D::BeginScene(const CEditorCamera& pCamera)
	{
		FGLMMat4 Trans = pCamera.GetViewProjectionMatrix();

		sData.Quad.Shader->Bind();
		sData.Quad.Shader->Set<FGLMMat4>("u_ViewProjection", Trans);

		sData.Quad.TranslucentIndexCount = 0;
		sData.Quad.TranslucentVertexBufferBasePtr = sData.Quad.TranslucentVertexBufferBase;

		sData.Quad.OpaqueIndexCount = 0;
		sData.Quad.OpaqueVertexBufferBasePtr = sData.Quad.OpaqueVertexBufferBase;

		sData.Circle.Shader->Bind();
		sData.Circle.Shader->Set<FGLMMat4>("u_ViewProjection", Trans);

		sData.Circle.TranslucentIndexCount = 0;
		sData.Circle.TranslucentVertexBufferBasePtr = sData.Circle.TranslucentVertexBufferBase;

		sData.Line.Shader->Bind();
		sData.Line.Shader->Set<FGLMMat4>("u_ViewProjection", Trans);
		
		sData.Line.IndexCount = 0;
		sData.Line.VertexBufferBasePtr = sData.Line.VertexBufferBase;

		sData.TextureSlotIndex = 1;
		sData.CameraTransform = glm::translate(glm::mat4(1.0f), pCamera.GetPosition()) * glm::toMat4(pCamera.GetOrientation());
	}

	void SRenderer2D::BeginScene(const CCamera& pCamera, const FTransform2D& pTransform)
	{
		FGLMMat4 Trans = pCamera.GetProjection() * glm::inverse(SHPR::Conv<FGLMMat4>(pTransform));

		sData.Quad.Shader->Bind();
		sData.Quad.Shader->Set<FGLMMat4>("u_ViewProjection", Trans);
		
		sData.Quad.TranslucentIndexCount = 0;
		sData.Quad.TranslucentVertexBufferBasePtr = sData.Quad.TranslucentVertexBufferBase;
		 
		sData.Quad.OpaqueIndexCount = 0;
		sData.Quad.OpaqueVertexBufferBasePtr = sData.Quad.OpaqueVertexBufferBase;

		sData.Circle.Shader->Bind();
		sData.Circle.Shader->Set<FGLMMat4>("u_ViewProjection", Trans);

		sData.Circle.TranslucentIndexCount = 0;
		sData.Circle.TranslucentVertexBufferBasePtr = sData.Circle.TranslucentVertexBufferBase;

		sData.Line.Shader->Bind();
		sData.Line.Shader->Set<FGLMMat4>("u_ViewProjection", Trans);

		sData.Line.IndexCount = 0;
		sData.Line.VertexBufferBasePtr = sData.Line.VertexBufferBase;

		sData.TextureSlotIndex = 1;
		sData.CameraTransform = SHPR::Conv<glm::mat4>(pTransform);
	}

	constexpr void SRenderer2D::EndScene(TGeometery pG, TTransparencyType pT)
	{
		ARC_PROFILE_FUNCTION();
		
		if (pG & EGeometery::Quad)
		{
			if (pT & ETransparencyType::Opaque)
			{
				if (sData.Quad.OpaqueIndexCount) {
					if (auto data_size = TUInt32((TUInt8*)sData.Quad.OpaqueVertexBufferBasePtr - (TUInt8*)sData.Quad.OpaqueVertexBufferBase)) {
						sData.Quad.VertexBuffer->SetData(sData.Quad.OpaqueVertexBufferBase, data_size);
						Flush(EGeometery::Quad, ETransparencyType::Opaque);
					}
				}
			}
			if (pT & ETransparencyType::Translucent)
			{
				if (sData.Quad.TranslucentIndexCount)
				{
					if (auto data_size = TUInt32((TUInt8*)sData.Quad.TranslucentVertexBufferBasePtr - (TUInt8*)sData.Quad.TranslucentVertexBufferBase)) {
						struct Block { SQuadVertex n_[4]; };
						
						std::sort((Block*)sData.Quad.TranslucentVertexBufferBase, (Block*)sData.Quad.TranslucentVertexBufferBasePtr,
							[&](const Block& p1, const Block& p2) -> bool {
								FGLMVec3 tmp = FGLMVec3(sData.CameraTransform[3]) - p1.n_[0].Position;
								FGLMVec3 tmp2 = FGLMVec3(sData.CameraTransform[3]) - p2.n_[0].Position;
								return p1.n_[0].Position[2] < p2.n_[0].Position[2];
							});
						Block* _ = (Block*)sData.Quad.TranslucentVertexBufferBase;
						sData.Quad.VertexBuffer->SetData(sData.Quad.TranslucentVertexBufferBase, data_size);
						Flush(EGeometery::Quad, ETransparencyType::Translucent);
					}
				}
			}
		}
		if (pG & EGeometery::Circle)
		{
			if (sData.Circle.TranslucentIndexCount)
			{
				if (auto data_size = TUInt32((TUInt8*)sData.Circle.TranslucentVertexBufferBasePtr - (TUInt8*)sData.Circle.TranslucentVertexBufferBase))
				{
					struct Block { SCircleVertex n_[4]; };

					// @ BUG ZINDEXING NOT WORKING FOR QUAD VS CIRCLE if translucent

					std::sort((Block*)sData.Circle.TranslucentVertexBufferBase, (Block*)sData.Circle.TranslucentVertexBufferBasePtr,
						[&](const Block& p1, const Block& p2) -> bool {
							FGLMVec3 tmp = FGLMVec3(sData.CameraTransform[3]) - p1.n_[0].WorldPosition;
							FGLMVec3 tmp2 = FGLMVec3(sData.CameraTransform[3]) - p2.n_[0].WorldPosition;
							return p1.n_[0].WorldPosition[2] < p2.n_[0].WorldPosition[2];
						});
					Block* _ = (Block*)sData.Circle.TranslucentVertexBufferBase;
					sData.Circle.VertexBuffer->SetData(sData.Circle.TranslucentVertexBufferBase, data_size);
					Flush(EGeometery::Circle, ETransparencyType::Translucent);
				}
			}
		}
		if (pG & EGeometery::Line)
		{
			if (auto data_size = TUInt32((TUInt8*)sData.Line.VertexBufferBasePtr - (TUInt8*)sData.Line.VertexBufferBase))
			{
				sData.Line.VertexBuffer->SetData(sData.Line.VertexBufferBase, data_size);
				Flush(EGeometery::Line, ETransparencyType::Translucent);
			}
		}
	}

	constexpr void SRenderer2D::Flush(TGeometery pG, TTransparencyType pT)
	{
		if (pG & EGeometery::Quad)
		{
			for (TUInt32 i = 0; i < sData.TextureSlotIndex; i++)
				sData.TextureSlots[i]->Bind(i);

			if (pT & ETransparencyType::Opaque)
			{
				if (sData.Quad.OpaqueIndexCount) {
					++sData.Statistics.DrawCalls;
					sData.Quad.Shader->Bind();
					CRenderCommand::DrawIndexed(sData.Quad.VertexArray, sData.Quad.OpaqueIndexCount);
				}
			}
			if (pT & ETransparencyType::Translucent)
			{
				if (sData.Quad.TranslucentIndexCount) {
					++sData.Statistics.DrawCalls;
					sData.Quad.Shader->Bind();
					CRenderCommand::DrawIndexed(sData.Quad.VertexArray, sData.Quad.TranslucentIndexCount);
				}
			}

		}
		if (pG & EGeometery::Circle)
		{
			if (sData.Circle.TranslucentIndexCount) {
				++sData.Statistics.DrawCalls;
				sData.Circle.Shader->Bind();
				CRenderCommand::DrawIndexed(sData.Circle.VertexArray, sData.Circle.TranslucentIndexCount);
			}
		}
		if (pG & EGeometery::Line)
		{
			if (sData.Line.IndexCount) {
				++sData.Statistics.DrawCalls;
				sData.Line.Shader->Bind();
				CRenderCommand::SetLineThickness(sData.Line.Thickness);
				CRenderCommand::DrawLine(sData.Line.VertexArray, sData.Line.IndexCount);
			}
		}
	}

	void SRenderer2D::DrawQuad(const FVec3& pPosition, const float pRotation, const FVec2& pSize, const TTransparencyType pTransparencyLevel, const FColor4& pColor, const TRef<CTexture2D>& pTex, const FVec2& pTextureScaling, const int& pId)

	{
		ARC_PROFILE_FUNCTION();

		if (sData.Quad.OpaqueIndexCount >= SRenderer2DData::MaxIndices)
			FlushAndReset(EGeometery::Quad, ETransparencyType::Opaque);

		if (sData.Quad.TranslucentIndexCount >= SRenderer2DData::MaxIndices)
			FlushAndReset(EGeometery::Quad, ETransparencyType::Translucent);

		TUInt32 textureIndex = 0;

		if (pTex != nullptr)
		{
			for (size_t i = 1; i < sData.TextureSlotIndex; i++)
			{
				if (*sData.TextureSlots[i].get() == *pTex.get())
				{
					textureIndex = i;
					break;
				}
			}
			if (!textureIndex)
			{
				textureIndex = sData.TextureSlotIndex;
				sData.TextureSlots[sData.TextureSlotIndex] = pTex;
				sData.TextureSlotIndex++;
			}
		}

		FGLMMat4 transform =
			 	glm::translate(FGLMMat4(1.0f), FGLMVec3(pPosition.x, pPosition.y, pPosition.z)) *
			 	glm::rotate(FGLMMat4(1.0f), pRotation, FGLMVec3(0, 0, 1)) *
			 	glm::scale(FGLMMat4(1.0f), FGLMVec3(pSize.x, pSize.y, 1.0f));

		if (pTransparencyLevel & ETransparencyType::Opaque) {
			for (size_t i = 0; i < 4; i++)
			{
				sData.Quad.OpaqueVertexBufferBasePtr->Position = transform * sData.Quad.VertexPositions[i];
				sData.Quad.OpaqueVertexBufferBasePtr->Color = FGLMVec4(pColor.r, pColor.g, pColor.b, pColor.a);
				sData.Quad.OpaqueVertexBufferBasePtr->TexCoord = CTexture2D::TexCoords[i];
				sData.Quad.OpaqueVertexBufferBasePtr->TexIndex = textureIndex;
				sData.Quad.OpaqueVertexBufferBasePtr->TexScaling = FGLMVec2(pTextureScaling.x, pTextureScaling.y);
				sData.Quad.OpaqueVertexBufferBasePtr->EntityId = pId;
				sData.Quad.OpaqueVertexBufferBasePtr++;
			}
			sData.Quad.OpaqueIndexCount += 6;
		}
		else if (pTransparencyLevel & ETransparencyType::Translucent) {
			for (size_t i = 0; i < 4; i++)
			{
				sData.Quad.TranslucentVertexBufferBasePtr->Position = transform * sData.Quad.VertexPositions[i];
				sData.Quad.TranslucentVertexBufferBasePtr->Color = FGLMVec4(pColor.r, pColor.g, pColor.b, pColor.a);
				sData.Quad.TranslucentVertexBufferBasePtr->TexCoord = CTexture2D::TexCoords[i];
				sData.Quad.TranslucentVertexBufferBasePtr->TexIndex = textureIndex;
				sData.Quad.TranslucentVertexBufferBasePtr->TexScaling = FGLMVec2(pTextureScaling.x, pTextureScaling.y);
				sData.Quad.TranslucentVertexBufferBasePtr->EntityId = pId;
				sData.Quad.TranslucentVertexBufferBasePtr++;
			}
			sData.Quad.TranslucentIndexCount += 6;
		}
		++sData.Statistics.QuadCount;
	}

	void SRenderer2D::DrawQuad(const FVec3& pPosition, const float pRotation, const FVec2& pSize, const TTransparencyType pTransparencyLevel, const FColor4& pColor, const TRef<CSubTexture2D>& pSubTex, const FVec2& pTextureScaling, const int& pId)
	{
		if (sData.Quad.OpaqueIndexCount >= SRenderer2DData::MaxIndices)
			FlushAndReset(EGeometery::Quad, ETransparencyType::Opaque);
		if (sData.Quad.TranslucentIndexCount >= SRenderer2DData::MaxIndices)
			FlushAndReset(EGeometery::Quad, ETransparencyType::Translucent);

		TUInt32 textureIndex = 0;

		if (pSubTex != nullptr)
		{
			for (size_t i = 1; i < sData.TextureSlotIndex; i++)
			{
				if (*sData.TextureSlots[i].get() == *pSubTex->GetTexture().get())
				{
					textureIndex = i;
					break;
				}
			}
			if (!textureIndex)
			{
				textureIndex = sData.TextureSlotIndex;
				sData.TextureSlots[sData.TextureSlotIndex] = pSubTex->GetTexture();
				sData.TextureSlotIndex++;
			}
		}

		FGLMMat4 transform =
			glm::translate(FGLMMat4(1.0f), FGLMVec3(pPosition.x, pPosition.y, pPosition.z)) *
			glm::rotate(FGLMMat4(1.0f), pRotation, FGLMVec3(0, 0, 1)) *
			glm::scale(FGLMMat4(1.0f), FGLMVec3(pSize.x, pSize.y, 1.0f));
		
		if (pTransparencyLevel & ETransparencyType::Opaque) {
			for (size_t i = 0; i < 4; i++)
			{
				sData.Quad.OpaqueVertexBufferBasePtr->Position = transform * sData.Quad.VertexPositions[i];
				sData.Quad.OpaqueVertexBufferBasePtr->Color = FGLMVec4(pColor.r, pColor.g, pColor.b, pColor.a);
				sData.Quad.OpaqueVertexBufferBasePtr->TexCoord = pSubTex->GetTexCoords()[i];
				sData.Quad.OpaqueVertexBufferBasePtr->TexIndex = textureIndex;
				sData.Quad.OpaqueVertexBufferBasePtr->TexScaling = FGLMVec2(pTextureScaling.x, pTextureScaling.y);
				sData.Quad.OpaqueVertexBufferBasePtr->EntityId = pId;
				sData.Quad.OpaqueVertexBufferBasePtr++;
			}
			sData.Quad.OpaqueIndexCount += 6;
		}
		else if (pTransparencyLevel & ETransparencyType::Translucent) {
			for (size_t i = 0; i < 4; i++)
			{
				sData.Quad.TranslucentVertexBufferBasePtr->Position = transform * sData.Quad.VertexPositions[i];
				sData.Quad.TranslucentVertexBufferBasePtr->Color = FGLMVec4(pColor.r, pColor.g, pColor.b, pColor.a);
				sData.Quad.TranslucentVertexBufferBasePtr->TexCoord = pSubTex->GetTexCoords()[i];
				sData.Quad.TranslucentVertexBufferBasePtr->TexIndex = textureIndex;
				sData.Quad.TranslucentVertexBufferBasePtr->TexScaling = FGLMVec2(pTextureScaling.x, pTextureScaling.y);
				sData.Quad.TranslucentVertexBufferBasePtr->EntityId = pId;
				sData.Quad.TranslucentVertexBufferBasePtr++;
			}
			sData.Quad.TranslucentIndexCount += 6;
		}

		++sData.Statistics.QuadCount;
	}


	void SRenderer2D::DrawQuad(const CPrimitive2D& Quad, const int& pId)
	{
		ARC_PROFILE_FUNCTION();

		DrawQuad(Quad.GetLocation(), Quad.GetRotation(), Quad.GetScale(), Quad.TransparencyLevel, Quad.Color, Quad.Texture ? Quad.Texture : sData.WhiteTexture, Quad.TextureScaling, pId);
	}

	void SRenderer2D::DrawCircle(const FVec3& pPosition, const float pRotation /*= 0.f*/, const FVec2& pSize /*= FVec2::OneVector()*/, const FColor4& pColor /*= FColor4::White()*/, const float pThickness /*= 1.f*/, const float pSharpness /*= 0.995f*/, const int& pId /*= -1*/)
	{
		ARC_PROFILE_FUNCTION();
		
		if (sData.Circle.TranslucentIndexCount >= SRenderer2DData::MaxIndices){}
			FlushAndReset(EGeometery::Circle, ETransparencyType::Translucent);

		// multi batch z sorting bug

		FGLMMat4 transform =
			glm::translate(FGLMMat4(1.0f), FGLMVec3(pPosition.x, pPosition.y, pPosition.z)) *
			glm::rotate(FGLMMat4(1.0f), pRotation, FGLMVec3(0, 0, 1)) *
			glm::scale(FGLMMat4(1.0f), FGLMVec3(pSize.x, pSize.y, 1.0f));

		for (size_t i = 0; i < 4; i++)
		{
			sData.Circle.TranslucentVertexBufferBasePtr->WorldPosition = transform * sData.Circle.VertexPositions[i];
			sData.Circle.TranslucentVertexBufferBasePtr->LocalPosition = sData.Circle.VertexPositions[i] * 2.f;
			sData.Circle.TranslucentVertexBufferBasePtr->Color = FGLMVec4(pColor.r, pColor.g, pColor.b, pColor.a);
			sData.Circle.TranslucentVertexBufferBasePtr->Thickness = pThickness;
			sData.Circle.TranslucentVertexBufferBasePtr->Sharpness = pSharpness;
			sData.Circle.TranslucentVertexBufferBasePtr->EntityId = pId;
			sData.Circle.TranslucentVertexBufferBasePtr++;
		}
		sData.Circle.TranslucentIndexCount += 6;

		++sData.Statistics.QuadCount;
	}

	void SRenderer2D::DrawLine(const FVec3& pPositionStart, const FVec3& pPositionEnd, const FColor4& pColor /*= FColor4::White()*/, const int& pId /*= -1*/)
	{
		sData.Line.VertexBufferBasePtr->Position = FGLMVec3(pPositionStart.x, pPositionStart.y, pPositionStart.z);
		sData.Line.VertexBufferBasePtr->Color = FGLMVec4(pColor.r, pColor.g, pColor.b, pColor.a);
		sData.Line.VertexBufferBasePtr->EntityId = pId;

		sData.Line.VertexBufferBasePtr++;

		sData.Line.VertexBufferBasePtr->Position = FGLMVec3(pPositionEnd.x, pPositionEnd.y, pPositionEnd.z);
		sData.Line.VertexBufferBasePtr->Color = FGLMVec4(pColor.r, pColor.g, pColor.b, pColor.a);
		sData.Line.VertexBufferBasePtr->EntityId = pId;

		sData.Line.VertexBufferBasePtr++;

		sData.Line.IndexCount+=2;
	}

	void SRenderer2D::DrawRect(const FVec3& pPosition, const float pRotation, const FVec2& pSize, const FColor4& pColor, const int& pId /*= -1*/)
	{
		FGLMMat4 transform =
			glm::translate(FGLMMat4(1.0f), FGLMVec3(pPosition.x, pPosition.y, pPosition.z)) *
			glm::rotate(FGLMMat4(1.0f), pRotation, FGLMVec3(0, 0, 1)) *
			glm::scale(FGLMMat4(1.0f), FGLMVec3(pSize.x, pSize.y, 1.0f));
		
		FGLMVec3 verts[4];

		for (size_t i = 0; i < 4; i++)
			verts[i] = transform * sData.Quad.VertexPositions[i];
		for (size_t i = 0; i < 4; i++)
			DrawLine(reinterpret_cast<FVec3&>(verts[i]), reinterpret_cast<FVec3&>(verts[(i+1)%4]), pColor, pId);

	}

	SRenderer2D::SStatistics SRenderer2D::GetStats()
	{
		return sData.Statistics;
	}

	void SRenderer2D::ResetStats()
	{
		memset(&sData.Statistics, 0, sizeof(SStatistics));
	}
	
	constexpr void SRenderer2D::FlushAndReset(TGeometery pG, TTransparencyType pT) {
		
		EndScene(pG, pT);

		if (pG & EGeometery::Quad)
		{
			if (pT & ETransparencyType::Translucent)
			{
				sData.Quad.TranslucentIndexCount = 0;
				sData.Quad.TranslucentVertexBufferBasePtr = sData.Quad.TranslucentVertexBufferBase;
			}
			if (pT & ETransparencyType::Opaque) {
				sData.Quad.OpaqueIndexCount = 0;
				sData.Quad.OpaqueVertexBufferBasePtr = sData.Quad.OpaqueVertexBufferBase;
			}
		}
		if (pG & EGeometery::Circle)
		{
			if (pT & ETransparencyType::Translucent)
			{
				sData.Circle.TranslucentIndexCount = 0;
				sData.Circle.TranslucentVertexBufferBasePtr = sData.Circle.TranslucentVertexBufferBase;
			}
		}
		if (pG & EGeometery::Line)
		{
			sData.Line.IndexCount = 0;
			sData.Line.VertexBufferBasePtr = sData.Line.VertexBufferBase;
		}
	}

	void SRenderer2D::SetLineThickness(float pThickness)
	{
		sData.Line.Thickness = pThickness;
	}

	float SRenderer2D::GetLineThickness()
	{
		return sData.Line.Thickness;
	}

}