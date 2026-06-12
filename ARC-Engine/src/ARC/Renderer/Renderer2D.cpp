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
namespace arc {
	struct quad_vertex
	{
		vec3 Position;
		vec4 Color;
		vec2 TexCoord;
		vec2 TexScaling;
		int TexIndex;
		int EntityId;
	};
	struct circle_vertex
	{
		vec3 WorldPosition;
		vec3 LocalPosition;
		vec4 Color;
		float Thickness;
		float Sharpness;
		int EntityId;
	};
	struct line_vertex
	{
		vec3 Position;
		vec4 Color;
		int EntityId;
	};
	struct SRenderer2DData {
		//@TODO : Load from ini file
		static constexpr u32 MaxQuads = 1000;
		static constexpr u32 MaxVertices = MaxQuads * 4;
		static constexpr u32 MaxIndices = MaxQuads * 6;
		static constexpr u32 MaxTextureSlots = 32;
		
		struct SQuadData {
			ref<vertex_array> VertexArray;
			ref<vertex_buffer> VertexBuffer;
			ref<shader> Shader;

			u32 TranslucentIndexCount = 0;
			quad_vertex* TranslucentVertexBufferBase = nullptr;
			quad_vertex* TranslucentVertexBufferBasePtr = nullptr;

			u32 OpaqueIndexCount = 0;
			quad_vertex* OpaqueVertexBufferBase = nullptr;
			quad_vertex* OpaqueVertexBufferBasePtr = nullptr;

			vec4 VertexPositions[4];
		} Quad;
		struct SCircleData {
			ref<vertex_array> VertexArray;
			ref<vertex_buffer> VertexBuffer;
			ref<shader> Shader;

			u32 TranslucentIndexCount = 0;
			circle_vertex* TranslucentVertexBufferBase = nullptr;
			circle_vertex* TranslucentVertexBufferBasePtr = nullptr;

			vec4 VertexPositions[4];
		} Circle;
		struct SLineData {
			ref<vertex_array> VertexArray;
			ref<vertex_buffer> VertexBuffer;
			ref<shader> Shader;

			float Thickness= 2;

			u32 IndexCount = 0;
			line_vertex* VertexBufferBase = nullptr;
			line_vertex* VertexBufferBasePtr = nullptr;
		} Line;

		ref<texture_2d> WhiteTexture;
		std::array<ref<texture_2d>, MaxTextureSlots> TextureSlots;
		u32 TextureSlotIndex = 1;

		mat4 CameraTransform;

		renderer_2d::statistics Statistics;

	};
	static SRenderer2DData sData;

	void renderer_2d::init()
	{
		ARC_PROFILE_FUNCTION();

		sData.WhiteTexture = texture_2d::create(vec2<u32>(1, 1));
		u32 whiteTextureData = 0xffffffff;
		sData.WhiteTexture->set_data(&whiteTextureData, sizeof(u32));

		int32_t samplers[sData.MaxTextureSlots];
		for (int32_t i = 0; i < sData.MaxTextureSlots; i++)
			samplers[i] = i;
		//--------------------------------+[Code for Quad]+--------------------------------//
		
		sData.Quad.VertexArray = vertex_array::create();
		sData.Quad.VertexBuffer = vertex_buffer::create(sData.MaxVertices * sizeof(quad_vertex));
		sData.Quad.VertexBuffer->set_layout({
			{ shader_data_type::Float3, "a_Position" },
			{ shader_data_type::Float4, "a_Color" },
			{ shader_data_type::Float2, "a_TexCoord" },
			{ shader_data_type::Float2, "a_TexScaling" },
			{ shader_data_type::Int, "a_TexIndex" },
			{ shader_data_type::Int, "a_EntityId" }
			});
		sData.Quad.VertexArray->add_vertex_buffer(sData.Quad.VertexBuffer);
		sData.Quad.OpaqueVertexBufferBase = new quad_vertex[sData.MaxVertices];
		sData.Quad.TranslucentVertexBufferBase = new quad_vertex[sData.MaxVertices];
		
		auto* quad_indices = new u32[sData.MaxIndices];

		u32 offset = 0;
		for (u32 i = 0; i < sData.MaxIndices; i += 6)
		{
			quad_indices[i+ 0] = offset + 0;
			quad_indices[i+ 1] = offset + 1;
			quad_indices[i+ 2] = offset + 2;

			quad_indices[i+ 3] = offset + 2;
			quad_indices[i+ 4] = offset + 3;
			quad_indices[i+ 5] = offset + 0;

			offset += 4;
		}
		ref<index_buffer> quad_ib = index_buffer::create(quad_indices, sData.MaxIndices);
		sData.Quad.VertexArray->set_index_buffer(quad_ib);
		
		// @TODO make thread safe
		delete[] quad_indices;

		sData.Quad.Shader = shader::create("assets/shaders/R2D_Quad.glsl");
		sData.Quad.Shader->bind();
		sData.Quad.Shader->set_array<int>("u_Textures", samplers, sData.MaxTextureSlots);

		sData.TextureSlots[0] = sData.WhiteTexture;
		sData.Quad.VertexPositions[0] = { -0.5f, -0.5f,  0.0f,  1.0f};
		sData.Quad.VertexPositions[1] = {  0.5f, -0.5f,  0.0f,  1.0f};
		sData.Quad.VertexPositions[2] = {  0.5f,  0.5f,  0.0f,  1.0f};
		sData.Quad.VertexPositions[3] = { -0.5f,  0.5f,  0.0f,  1.0f}; 
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~-[Code for Quad]-~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		//-------------------------------+[Code for Circle]+-------------------------------//
		sData.Circle.VertexArray = vertex_array::create();
		sData.Circle.VertexBuffer = vertex_buffer::create(sData.MaxVertices * sizeof(circle_vertex));
		sData.Circle.VertexBuffer->set_layout({
			{ shader_data_type::Float3, "a_WorldPosition" },
			{ shader_data_type::Float3, "a_LocalPosition" },
			{ shader_data_type::Float4, "a_Color" },
			{ shader_data_type::Float, "a_Thickness" },
			{ shader_data_type::Float, "a_Sharpness" },
			{ shader_data_type::Int, "a_EntityId" }
			});
		sData.Circle.VertexArray->add_vertex_buffer(sData.Circle.VertexBuffer);
		sData.Circle.TranslucentVertexBufferBase = new circle_vertex[sData.MaxVertices];
		sData.Circle.VertexArray->set_index_buffer(quad_ib); // reuse quad one as same

		sData.Circle.Shader = shader::create("assets/shaders/R2D_Circle.glsl");
		sData.Circle.Shader->bind();

		sData.Circle.VertexPositions[0] = { -0.5f, -0.5f,  0.0f,  1.0f };
		sData.Circle.VertexPositions[1] = { 0.5f, -0.5f,  0.0f,  1.0f };
		sData.Circle.VertexPositions[2] = { 0.5f,  0.5f,  0.0f,  1.0f };
		sData.Circle.VertexPositions[3] = { -0.5f,  0.5f,  0.0f,  1.0f };
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~-[Code for Circle]-~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
		//--------------------------------+[Code for Line]+--------------------------------//
		sData.Line.VertexArray = vertex_array::create();
		sData.Line.VertexBuffer = vertex_buffer::create(sData.MaxVertices * sizeof(line_vertex));
		sData.Line.VertexBuffer->set_layout({
			{ shader_data_type::Float3, "a_Position" },
			{ shader_data_type::Float4, "a_Color" },
			{ shader_data_type::Int, "a_EntityId" }
			});
		sData.Line.VertexArray->add_vertex_buffer(sData.Line.VertexBuffer);
		sData.Line.VertexBufferBase = new line_vertex[sData.MaxVertices];

		sData.Line.Shader = shader::create("assets/shaders/R2D_Line.glsl");
		sData.Line.Shader->bind();
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~-[Code for Line]-~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


	}

	void renderer_2d::shutdown()
	{
	}

	void renderer_2d::begin_scene(const orthographic_camera& pCamera)
	{
		mat4 Trans = pCamera.get_view_projection_matrix();

		sData.Quad.Shader->bind();
		sData.Quad.Shader->Set<mat4>("u_ViewProjection", pCamera.get_view_projection_matrix());
		
		sData.Quad.TranslucentIndexCount = 0;
		sData.Quad.TranslucentVertexBufferBasePtr = sData.Quad.TranslucentVertexBufferBase;

		sData.Quad.OpaqueIndexCount = 0;
		sData.Quad.OpaqueVertexBufferBasePtr = sData.Quad.OpaqueVertexBufferBase;

		sData.Circle.Shader->bind();
		sData.Circle.Shader->Set<mat4>("u_ViewProjection", Trans);

		sData.Circle.TranslucentIndexCount = 0;
		sData.Circle.TranslucentVertexBufferBasePtr = sData.Circle.TranslucentVertexBufferBase;

		sData.TextureSlotIndex = 1;
		sData.CameraTransform = Trans;
	}

	void renderer_2d::begin_scene(const editor_camera& pCamera)
	{
		mat4 Trans = pCamera.get_view_projection_matrix();

		sData.Quad.Shader->bind();
		sData.Quad.Shader->Set<mat4>("u_ViewProjection", Trans);

		sData.Quad.TranslucentIndexCount = 0;
		sData.Quad.TranslucentVertexBufferBasePtr = sData.Quad.TranslucentVertexBufferBase;

		sData.Quad.OpaqueIndexCount = 0;
		sData.Quad.OpaqueVertexBufferBasePtr = sData.Quad.OpaqueVertexBufferBase;

		sData.Circle.Shader->bind();
		sData.Circle.Shader->Set<mat4>("u_ViewProjection", Trans);

		sData.Circle.TranslucentIndexCount = 0;
		sData.Circle.TranslucentVertexBufferBasePtr = sData.Circle.TranslucentVertexBufferBase;

		sData.Line.Shader->bind();
		sData.Line.Shader->Set<mat4>("u_ViewProjection", Trans);
		
		sData.Line.IndexCount = 0;
		sData.Line.VertexBufferBasePtr = sData.Line.VertexBufferBase;

		sData.TextureSlotIndex = 1;
		sData.CameraTransform = glm::translate(glm::mat4(1.0f), pCamera.get_position()) * glm::toMat4(pCamera.get_orientation());
	}

	void renderer_2d::begin_scene(const camera& pCamera, const transform_2d& pTransform)
	{
		mat4 Trans = pCamera.get_projection() * glm::inverse(shpr::Conv<mat4>(pTransform));

		sData.Quad.Shader->bind();
		sData.Quad.Shader->Set<mat4>("u_ViewProjection", Trans);
		
		sData.Quad.TranslucentIndexCount = 0;
		sData.Quad.TranslucentVertexBufferBasePtr = sData.Quad.TranslucentVertexBufferBase;
		 
		sData.Quad.OpaqueIndexCount = 0;
		sData.Quad.OpaqueVertexBufferBasePtr = sData.Quad.OpaqueVertexBufferBase;

		sData.Circle.Shader->bind();
		sData.Circle.Shader->Set<mat4>("u_ViewProjection", Trans);

		sData.Circle.TranslucentIndexCount = 0;
		sData.Circle.TranslucentVertexBufferBasePtr = sData.Circle.TranslucentVertexBufferBase;

		sData.Line.Shader->bind();
		sData.Line.Shader->Set<mat4>("u_ViewProjection", Trans);

		sData.Line.IndexCount = 0;
		sData.Line.VertexBufferBasePtr = sData.Line.VertexBufferBase;

		sData.TextureSlotIndex = 1;
		sData.CameraTransform = shpr::Conv<glm::mat4>(pTransform);
	}

	constexpr void renderer_2d::end_scene(geometry_type pG, transparency_type pT)
	{
		ARC_PROFILE_FUNCTION();
		
		if (pG & geometry_type::Quad)
		{
			if (pT & transparency_type::Opaque)
			{
				if (sData.Quad.OpaqueIndexCount) {
					if (auto data_size = u32((u8*)sData.Quad.OpaqueVertexBufferBasePtr - (u8*)sData.Quad.OpaqueVertexBufferBase)) {
						sData.Quad.VertexBuffer->set_data(sData.Quad.OpaqueVertexBufferBase, data_size);
						flush(geometry_type::Quad, transparency_type::Opaque);
					}
				}
			}
			if (pT & transparency_type::Translucent)
			{
				if (sData.Quad.TranslucentIndexCount)
				{
					if (auto data_size = u32((u8*)sData.Quad.TranslucentVertexBufferBasePtr - (u8*)sData.Quad.TranslucentVertexBufferBase)) {
						struct Block { quad_vertex n_[4]; };
						
						std::sort((Block*)sData.Quad.TranslucentVertexBufferBase, (Block*)sData.Quad.TranslucentVertexBufferBasePtr,
							[&](const Block& p1, const Block& p2) -> bool {
								vec3 tmp = vec3(sData.CameraTransform[3]) - p1.n_[0].Position;
								vec3 tmp2 = vec3(sData.CameraTransform[3]) - p2.n_[0].Position;
								return p1.n_[0].Position[2] < p2.n_[0].Position[2];
							});
						Block* _ = (Block*)sData.Quad.TranslucentVertexBufferBase;
						sData.Quad.VertexBuffer->set_data(sData.Quad.TranslucentVertexBufferBase, data_size);
						flush(geometry_type::Quad, transparency_type::Translucent);
					}
				}
			}
		}
		if (pG & geometry_type::Circle)
		{
			if (sData.Circle.TranslucentIndexCount)
			{
				if (auto data_size = u32((u8*)sData.Circle.TranslucentVertexBufferBasePtr - (u8*)sData.Circle.TranslucentVertexBufferBase))
				{
					struct Block { circle_vertex n_[4]; };

					// @ BUG ZINDEXING NOT WORKING FOR QUAD VS CIRCLE if translucent

					std::sort((Block*)sData.Circle.TranslucentVertexBufferBase, (Block*)sData.Circle.TranslucentVertexBufferBasePtr,
						[&](const Block& p1, const Block& p2) -> bool {
							vec3 tmp = vec3(sData.CameraTransform[3]) - p1.n_[0].WorldPosition;
							vec3 tmp2 = vec3(sData.CameraTransform[3]) - p2.n_[0].WorldPosition;
							return p1.n_[0].WorldPosition[2] < p2.n_[0].WorldPosition[2];
						});
					Block* _ = (Block*)sData.Circle.TranslucentVertexBufferBase;
					sData.Circle.VertexBuffer->set_data(sData.Circle.TranslucentVertexBufferBase, data_size);
					flush(geometry_type::Circle, transparency_type::Translucent);
				}
			}
		}
		if (pG & geometry_type::Line)
		{
			if (auto data_size = u32((u8*)sData.Line.VertexBufferBasePtr - (u8*)sData.Line.VertexBufferBase))
			{
				sData.Line.VertexBuffer->set_data(sData.Line.VertexBufferBase, data_size);
				flush(geometry_type::Line, transparency_type::Translucent);
			}
		}
	}

	constexpr void renderer_2d::flush(geometry_type pG, transparency_type pT)
	{
		if (pG & geometry_type::Quad)
		{
			for (u32 i = 0; i < sData.TextureSlotIndex; i++)
				sData.TextureSlots[i]->bind(i);

			if (pT & transparency_type::Opaque)
			{
				if (sData.Quad.OpaqueIndexCount) {
					++sData.Statistics.DrawCalls;
					sData.Quad.Shader->bind();
					render_command::draw_indexed(sData.Quad.VertexArray, sData.Quad.OpaqueIndexCount);
				}
			}
			if (pT & transparency_type::Translucent)
			{
				if (sData.Quad.TranslucentIndexCount) {
					++sData.Statistics.DrawCalls;
					sData.Quad.Shader->bind();
					render_command::draw_indexed(sData.Quad.VertexArray, sData.Quad.TranslucentIndexCount);
				}
			}

		}
		if (pG & geometry_type::Circle)
		{
			if (sData.Circle.TranslucentIndexCount) {
				++sData.Statistics.DrawCalls;
				sData.Circle.Shader->bind();
				render_command::draw_indexed(sData.Circle.VertexArray, sData.Circle.TranslucentIndexCount);
			}
		}
		if (pG & geometry_type::Line)
		{
			if (sData.Line.IndexCount) {
				++sData.Statistics.DrawCalls;
				sData.Line.Shader->bind();
				render_command::set_line_thickness(sData.Line.Thickness);
				render_command::draw_line(sData.Line.VertexArray, sData.Line.IndexCount);
			}
		}
	}

	void renderer_2d::draw_quad(const vec3& pPosition, const float pRotation, const vec2& pSize, const transparency_type pTransparencyLevel, const color4& pColor, const ref<texture_2d>& pTex, const vec2& pTextureScaling, const int& pId)

	{
		ARC_PROFILE_FUNCTION();

		if (sData.Quad.OpaqueIndexCount >= SRenderer2DData::MaxIndices)
			flush_and_reset(geometry_type::Quad, transparency_type::Opaque);

		if (sData.Quad.TranslucentIndexCount >= SRenderer2DData::MaxIndices)
			flush_and_reset(geometry_type::Quad, transparency_type::Translucent);

		u32 textureIndex = 0;

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

		mat4 transform =
			 	glm::translate(mat4(1.0f), vec3(pPosition.x, pPosition.y, pPosition.z)) *
			 	glm::rotate(mat4(1.0f), pRotation, vec3(0, 0, 1)) *
			 	glm::scale(mat4(1.0f), vec3(pSize.x, pSize.y, 1.0f));

		if (pTransparencyLevel & transparency_type::Opaque) {
			for (size_t i = 0; i < 4; i++)
			{
				sData.Quad.OpaqueVertexBufferBasePtr->Position = transform * sData.Quad.VertexPositions[i];
				sData.Quad.OpaqueVertexBufferBasePtr->Color = vec4(pColor.r, pColor.g, pColor.b, pColor.a);
				sData.Quad.OpaqueVertexBufferBasePtr->TexCoord = texture_2d::TexCoords[i];
				sData.Quad.OpaqueVertexBufferBasePtr->TexIndex = textureIndex;
				sData.Quad.OpaqueVertexBufferBasePtr->TexScaling = vec2(pTextureScaling.x, pTextureScaling.y);
				sData.Quad.OpaqueVertexBufferBasePtr->EntityId = pId;
				sData.Quad.OpaqueVertexBufferBasePtr++;
			}
			sData.Quad.OpaqueIndexCount += 6;
		}
		else if (pTransparencyLevel & transparency_type::Translucent) {
			for (size_t i = 0; i < 4; i++)
			{
				sData.Quad.TranslucentVertexBufferBasePtr->Position = transform * sData.Quad.VertexPositions[i];
				sData.Quad.TranslucentVertexBufferBasePtr->Color = vec4(pColor.r, pColor.g, pColor.b, pColor.a);
				sData.Quad.TranslucentVertexBufferBasePtr->TexCoord = texture_2d::TexCoords[i];
				sData.Quad.TranslucentVertexBufferBasePtr->TexIndex = textureIndex;
				sData.Quad.TranslucentVertexBufferBasePtr->TexScaling = vec2(pTextureScaling.x, pTextureScaling.y);
				sData.Quad.TranslucentVertexBufferBasePtr->EntityId = pId;
				sData.Quad.TranslucentVertexBufferBasePtr++;
			}
			sData.Quad.TranslucentIndexCount += 6;
		}
		++sData.Statistics.QuadCount;
	}

	void renderer_2d::draw_quad(const vec3& pPosition, const float pRotation, const vec2& pSize, const transparency_type pTransparencyLevel, const color4& pColor, const ref<sub_texture_2d>& pSubTex, const vec2& pTextureScaling, const int& pId)
	{
		if (sData.Quad.OpaqueIndexCount >= SRenderer2DData::MaxIndices)
			flush_and_reset(geometry_type::Quad, transparency_type::Opaque);
		if (sData.Quad.TranslucentIndexCount >= SRenderer2DData::MaxIndices)
			flush_and_reset(geometry_type::Quad, transparency_type::Translucent);

		u32 textureIndex = 0;

		if (pSubTex != nullptr)
		{
			for (size_t i = 1; i < sData.TextureSlotIndex; i++)
			{
				if (*sData.TextureSlots[i].get() == *pSubTex->get_texture().get())
				{
					textureIndex = i;
					break;
				}
			}
			if (!textureIndex)
			{
				textureIndex = sData.TextureSlotIndex;
				sData.TextureSlots[sData.TextureSlotIndex] = pSubTex->get_texture();
				sData.TextureSlotIndex++;
			}
		}

		mat4 transform =
			glm::translate(mat4(1.0f), vec3(pPosition.x, pPosition.y, pPosition.z)) *
			glm::rotate(mat4(1.0f), pRotation, vec3(0, 0, 1)) *
			glm::scale(mat4(1.0f), vec3(pSize.x, pSize.y, 1.0f));
		
		if (pTransparencyLevel & transparency_type::Opaque) {
			for (size_t i = 0; i < 4; i++)
			{
				sData.Quad.OpaqueVertexBufferBasePtr->Position = transform * sData.Quad.VertexPositions[i];
				sData.Quad.OpaqueVertexBufferBasePtr->Color = vec4(pColor.r, pColor.g, pColor.b, pColor.a);
				sData.Quad.OpaqueVertexBufferBasePtr->TexCoord = pSubTex->get_tex_coords()[i];
				sData.Quad.OpaqueVertexBufferBasePtr->TexIndex = textureIndex;
				sData.Quad.OpaqueVertexBufferBasePtr->TexScaling = vec2(pTextureScaling.x, pTextureScaling.y);
				sData.Quad.OpaqueVertexBufferBasePtr->EntityId = pId;
				sData.Quad.OpaqueVertexBufferBasePtr++;
			}
			sData.Quad.OpaqueIndexCount += 6;
		}
		else if (pTransparencyLevel & transparency_type::Translucent) {
			for (size_t i = 0; i < 4; i++)
			{
				sData.Quad.TranslucentVertexBufferBasePtr->Position = transform * sData.Quad.VertexPositions[i];
				sData.Quad.TranslucentVertexBufferBasePtr->Color = vec4(pColor.r, pColor.g, pColor.b, pColor.a);
				sData.Quad.TranslucentVertexBufferBasePtr->TexCoord = pSubTex->get_tex_coords()[i];
				sData.Quad.TranslucentVertexBufferBasePtr->TexIndex = textureIndex;
				sData.Quad.TranslucentVertexBufferBasePtr->TexScaling = vec2(pTextureScaling.x, pTextureScaling.y);
				sData.Quad.TranslucentVertexBufferBasePtr->EntityId = pId;
				sData.Quad.TranslucentVertexBufferBasePtr++;
			}
			sData.Quad.TranslucentIndexCount += 6;
		}

		++sData.Statistics.QuadCount;
	}


	void renderer_2d::draw_quad(const primitive_2d& Quad, const int& pId)
	{
		ARC_PROFILE_FUNCTION();

		draw_quad(Quad.get_location(), Quad.get_rotation(), Quad.get_scale(), Quad.TransparencyLevel, Quad.Color, Quad.Texture ? Quad.Texture : sData.WhiteTexture, Quad.TextureScaling, pId);
	}

	void renderer_2d::draw_circle(const vec3& pPosition, const float pRotation /*= 0.f*/, const vec2& pSize /*= vec2::OneVector()*/, const color4& pColor /*= color4::White()*/, const float pThickness /*= 1.f*/, const float pSharpness /*= 0.995f*/, const int& pId /*= -1*/)
	{
		ARC_PROFILE_FUNCTION();
		
		if (sData.Circle.TranslucentIndexCount >= SRenderer2DData::MaxIndices){}
			flush_and_reset(geometry_type::Circle, transparency_type::Translucent);

		// multi batch z sorting bug

		mat4 transform =
			glm::translate(mat4(1.0f), vec3(pPosition.x, pPosition.y, pPosition.z)) *
			glm::rotate(mat4(1.0f), pRotation, vec3(0, 0, 1)) *
			glm::scale(mat4(1.0f), vec3(pSize.x, pSize.y, 1.0f));

		for (size_t i = 0; i < 4; i++)
		{
			sData.Circle.TranslucentVertexBufferBasePtr->WorldPosition = transform * sData.Circle.VertexPositions[i];
			sData.Circle.TranslucentVertexBufferBasePtr->LocalPosition = sData.Circle.VertexPositions[i] * 2.f;
			sData.Circle.TranslucentVertexBufferBasePtr->Color = vec4(pColor.r, pColor.g, pColor.b, pColor.a);
			sData.Circle.TranslucentVertexBufferBasePtr->Thickness = pThickness;
			sData.Circle.TranslucentVertexBufferBasePtr->Sharpness = pSharpness;
			sData.Circle.TranslucentVertexBufferBasePtr->EntityId = pId;
			sData.Circle.TranslucentVertexBufferBasePtr++;
		}
		sData.Circle.TranslucentIndexCount += 6;

		++sData.Statistics.QuadCount;
	}

	void renderer_2d::draw_line(const vec3& pPositionStart, const vec3& pPositionEnd, const color4& pColor /*= color4::White()*/, const int& pId /*= -1*/)
	{
		sData.Line.VertexBufferBasePtr->Position = vec3(pPositionStart.x, pPositionStart.y, pPositionStart.z);
		sData.Line.VertexBufferBasePtr->Color = vec4(pColor.r, pColor.g, pColor.b, pColor.a);
		sData.Line.VertexBufferBasePtr->EntityId = pId;

		sData.Line.VertexBufferBasePtr++;

		sData.Line.VertexBufferBasePtr->Position = vec3(pPositionEnd.x, pPositionEnd.y, pPositionEnd.z);
		sData.Line.VertexBufferBasePtr->Color = vec4(pColor.r, pColor.g, pColor.b, pColor.a);
		sData.Line.VertexBufferBasePtr->EntityId = pId;

		sData.Line.VertexBufferBasePtr++;

		sData.Line.IndexCount+=2;
	}

	void renderer_2d::DrawRect(const vec3& pPosition, const float pRotation, const vec2& pSize, const color4& pColor, const int& pId /*= -1*/)
	{
		mat4 transform =
			glm::translate(mat4(1.0f), vec3(pPosition.x, pPosition.y, pPosition.z)) *
			glm::rotate(mat4(1.0f), pRotation, vec3(0, 0, 1)) *
			glm::scale(mat4(1.0f), vec3(pSize.x, pSize.y, 1.0f));
		
		vec3 verts[4];

		for (size_t i = 0; i < 4; i++)
			verts[i] = transform * sData.Quad.VertexPositions[i];
		for (size_t i = 0; i < 4; i++)
			draw_line(reinterpret_cast<vec3&>(verts[i]), reinterpret_cast<vec3&>(verts[(i+1)%4]), pColor, pId);

	}

	renderer_2d::statistics renderer_2d::get_stats()
	{
		return sData.Statistics;
	}

	void renderer_2d::ResetStats()
	{
		memset(&sData.Statistics, 0, sizeof(statistics));
	}
	
	constexpr void renderer_2d::flush_and_reset(geometry_type pG, transparency_type pT) {
		
		end_scene(pG, pT);

		if (pG & geometry_type::Quad)
		{
			if (pT & transparency_type::Translucent)
			{
				sData.Quad.TranslucentIndexCount = 0;
				sData.Quad.TranslucentVertexBufferBasePtr = sData.Quad.TranslucentVertexBufferBase;
			}
			if (pT & transparency_type::Opaque) {
				sData.Quad.OpaqueIndexCount = 0;
				sData.Quad.OpaqueVertexBufferBasePtr = sData.Quad.OpaqueVertexBufferBase;
			}
		}
		if (pG & geometry_type::Circle)
		{
			if (pT & transparency_type::Translucent)
			{
				sData.Circle.TranslucentIndexCount = 0;
				sData.Circle.TranslucentVertexBufferBasePtr = sData.Circle.TranslucentVertexBufferBase;
			}
		}
		if (pG & geometry_type::Line)
		{
			sData.Line.IndexCount = 0;
			sData.Line.VertexBufferBasePtr = sData.Line.VertexBufferBase;
		}
	}

	void renderer_2d::set_line_thickness(float pThickness)
	{
		sData.Line.Thickness = pThickness;
	}

	float renderer_2d::get_line_thickness()
	{
		return sData.Line.Thickness;
	}

}