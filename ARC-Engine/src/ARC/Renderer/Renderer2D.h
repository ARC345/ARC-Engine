#pragma once
#include "ARC/Types/Color.h"
#include "ARC/Types/Transform2D.h"
#include "../Scene/Entity.h"

namespace arc { class editor_camera; }

namespace arc { class editor_camera; }

namespace arc { class camera; }

namespace arc { class sub_texture_2d; }
namespace arc { class primitive_2d; }

namespace arc { class orthographic_camera; }
namespace arc { class vertex_array; }
namespace arc { class texture_2d; }
namespace arc { class shader; }

namespace arc {
	
	namespace geometry_type {
		enum geometry_type { Quad = 1, Circle= 2, Line=3};
	};
	using geometry_type = u8;
	class renderer_2d
	{
		public:
			static void init();
			static void shutdown();

			static void begin_scene(const orthographic_camera& pCamera);
			static void begin_scene(const editor_camera& pCamera);
			static void begin_scene(const camera& pCamera, const transform_2d& pTransform);

			constexpr static void end_scene(geometry_type pG = geometry_type::Quad | geometry_type::Circle | geometry_type::Line, transparency_type pT = transparency_type::Opaque | transparency_type::Translucent | transparency_type::Transparent);
			constexpr static void flush(geometry_type pG = geometry_type::Quad | geometry_type::Circle | geometry_type::Line, transparency_type pT = transparency_type::Opaque | transparency_type::Translucent | transparency_type::Transparent);
			/*
			*	@param pPosition: Center location of quad.
			*	@param pRotation: Rotation of quad in radians.
			*	@param pSize: size of quad.
			*	@param pColor: Color of quad.
			*	@param pTexture: Texture of quad.
			*	@param pTextureScaling: Scaling applied to the texture.
			*/
			static void draw_quad(const vec3& pPosition, const float pRotation = 0.f, const vec2& pSize = vec2::OneVector(), const transparency_type pTransparencyLevel = transparency_type::Translucent, const color4& pColor = color4::White(), const ref<texture_2d>& pTex = nullptr, const vec2& pTextureScaling = vec2::OneVector(), const int& pId = -1);
			/*
			*	@param pPosition: Center location of quad.
			*	@param pRotation: Rotation of quad in radians.
			*	@param pSize: size of quad.
			*	@param pColor: Color of quad.
			*	@param pTexture: Texture of quad.
			*	@param pTextureScaling: Scaling applied to the texture.
			*/
			static void draw_quad(const vec3& pPosition, const float pRotation, const vec2& pSize, const transparency_type pTransparencyLevel, const color4& pColor, const ref<sub_texture_2d>& pTex, const vec2& pTextureScaling, const int& pId = -1);
			static void draw_quad(const primitive_2d& Quad, const int& pId = -1);

			static void draw_circle(const vec3& pPosition, const float pRotation = 0.f, const vec2& pSize = vec2::OneVector(), const color4& pColor = color4::White(), const float pThickness = 1.f, const float pSharpness = 0.995f, const int& pId = -1);
			static void draw_line(const vec3& pPositionStart, const vec3& pPositionEnd, const color4& pColor = color4::White(), const int& pId = -1);
			static void DrawRect(const vec3& pPosition, const float pRotation, const vec2& pSize, const color4& pColor, const int& pId = -1);
			
			static void set_line_thickness(float pThickness);
			static float get_line_thickness();

			struct statistics
			{
				u32 DrawCalls = 0;
				u32 QuadCount = 0;
				
				u32 get_total_vertex_count() const { return QuadCount * 4; }
				u32 get_total_index_count() const{ return QuadCount * 6; }
			};
			static statistics get_stats();
			static void ResetStats();

		private:
			constexpr static void flush_and_reset(geometry_type pG, transparency_type pT);
	};
}
