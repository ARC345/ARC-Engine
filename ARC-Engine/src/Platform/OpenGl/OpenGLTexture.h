#pragma once
#include "ARC\Renderer\Texture.h"
#include <utility>

namespace arc {
	class opengl_texture_2d :
		public texture_2d
	{
		public:
			opengl_texture_2d(const vec2<uint32_t>& dimentions);
			opengl_texture_2d(const std::filesystem::path & path, bool bManualClear = false);
			virtual ~opengl_texture_2d() override;

			virtual void set_data(void* data, uint32_t size) override;

			virtual void bind(uint32_t slot = 0) const override;
			virtual void ClearData() override;

			virtual const std::filesystem::path& get_path() const override { return path_; };

			virtual vec4<unsigned char> get_pixel_color(vec2<uint32_t> xy) override;

			virtual bool operator==(const texture& tex) const override {
				return renderer_id_ == ((opengl_texture_2d&)tex).renderer_id_;
			};

			uint32_t get_renderer_id() override { return renderer_id_; };

	private:
			vec2<uint32_t> Load(const vec2<uint32_t>& dimentions);
			vec2<uint32_t> Load(const std::filesystem::path& path);
		private:
			uint32_t bytes_per_pixel_;

			unsigned int internal_format_; //GLenum 
			unsigned int data_format_; //GLenum 
			unsigned char* data_;
			std::filesystem::path path_;
			uint32_t renderer_id_;
	};
}