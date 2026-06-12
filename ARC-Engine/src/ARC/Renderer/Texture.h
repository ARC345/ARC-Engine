#pragma once
#include <string>
#include "..\Types\vector.h"
#include "glm\ext\vector_float2.hpp"

namespace arc {
	class texture
	{
		public:
			virtual ~texture() = default;

			virtual void ClearData() = 0;
			virtual const std::filesystem::path& get_path() const = 0;
			virtual void set_data(void*, uint32_t size) = 0;
			virtual void bind(uint32_t slot = 0) const = 0;

			virtual uint32_t get_renderer_id() = 0;

			virtual bool operator==(const texture& tex) const = 0;
	};
	
	class texture_2d : public texture
	{
		public:
			static ref<texture_2d> create(const vec2<uint32_t> dimentions);
			static ref<texture_2d> create(const std::filesystem::path& path, bool bManualClear = false);

			virtual vec4<unsigned char> get_pixel_color(vec2<uint32_t> xy) = 0;
			
		protected:
			inline texture_2d(vec2<uint32_t> dimentions) : Dimensions(dimentions) {}
		public:
			vec2<uint32_t> Dimensions;

			static constexpr glm::vec2 TexCoords[] = {
				{0.f, 0.f},
				{1.f, 0.f},
				{1.f, 1.f},
				{0.f, 1.f}
			};
			
	};
}