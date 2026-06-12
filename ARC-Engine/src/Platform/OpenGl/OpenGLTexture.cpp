#include "arc_pch.h"
#include "OpenGLTexture.h"
#include "stb_image.h"
#include "ARC\Core\Core.h"
#include "glad\glad.h"

namespace arc {
	opengl_texture_2d::opengl_texture_2d(const std::filesystem::path& path, bool bManualClear) :
		texture_2d(Load(path)),
		path_(path)
	{
		if (!bManualClear)
		{
			ClearData();
		}
	}

	opengl_texture_2d::opengl_texture_2d(const vec2<uint32_t>& dimentions) :
		texture_2d(Load(dimentions))
	{
		
	}

	opengl_texture_2d::~opengl_texture_2d()
	{
		glDeleteTextures(1, &renderer_id_);
	}

	void opengl_texture_2d::set_data(void* data, uint32_t size)
	{
		ARC_CORE_ASSERT(size == Dimensions.x * Dimensions.y * bytes_per_pixel_, "Data must be entire texture");
		glTextureSubImage2D(renderer_id_, 0, 0, 0, Dimensions.x, Dimensions.y, data_format_, GL_UNSIGNED_BYTE, data);
	}

	void opengl_texture_2d::bind(uint32_t slot /*= 0*/) const
	{
		glBindTextureUnit(slot, renderer_id_);
	}

	void opengl_texture_2d::ClearData()
	{
		if (data_)
		{
			stbi_image_free(data_);
		}
	}

	vec4<unsigned char> opengl_texture_2d::get_pixel_color(vec2<uint32_t> xy)
	{
		vec4<unsigned char> rval;
		const stbi_uc* p = data_ + (4 * (xy.y * Dimensions.x + xy.x));
		rval.r = p[0];
		rval.g = p[1];
		rval.b = p[2];
		rval.a = p[3];
		return rval;
	}

	vec2<uint32_t> opengl_texture_2d::Load(const std::filesystem::path& path)
	{
		ARC_PROFILE_FUNCTION();
		int channels, width, height;
		stbi_set_flip_vertically_on_load(1);
		data_ = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
		ARC_CORE_ASSERT(data_, "Error loading texture from file");

		internal_format_ = 0;
		data_format_ = 0;
		bytes_per_pixel_ = channels;

		switch (channels)
		{
		case 4:
			internal_format_ = GL_RGBA8;
			data_format_ = GL_RGBA;
			break;
		case 3:
			internal_format_ = GL_RGB8;
			data_format_ = GL_RGB;
			break;
		case 2:
			internal_format_ = GL_RG8;
			data_format_ = GL_RG;
			break;
		case 1:
			internal_format_ = GL_R;
			data_format_ = GL_R;
			break;
		default:
			ARC_CORE_ASSERT(data_, "Texture format is not supported");
			break;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
		glTextureStorage2D(renderer_id_, 1, internal_format_, width, height);

		glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(renderer_id_, 0, 0, 0, width, height, data_format_, GL_UNSIGNED_BYTE, data_);
		return vec2<uint32_t>(width, height);
	}

	vec2<uint32_t> opengl_texture_2d::Load(const vec2<uint32_t>& dimentions)
	{
		ARC_PROFILE_FUNCTION();
		internal_format_ = GL_RGBA8;
		data_format_ = GL_RGBA;
		bytes_per_pixel_ = 4;

		glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id_);
		glTextureStorage2D(renderer_id_, 1, internal_format_, dimentions.x, dimentions.y);

		glTextureParameteri(renderer_id_, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(renderer_id_, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(renderer_id_, GL_TEXTURE_WRAP_T, GL_REPEAT);

		return dimentions;
	}
}