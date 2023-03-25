#include "arc_pch.h"
#include "OpenGLTexture.h"
#include "stb_image.h"
#include "ARC\Core\Core.h"
#include "glad\glad.h"

namespace ARC {
	COpenGLTexture2D::COpenGLTexture2D(const std::filesystem::path& _Path, bool bManualClear) :
		CTexture2D(Load(_Path)),
		mPath(_Path)
	{
		if (!bManualClear)
		{
			ClearData();
		}
	}

	COpenGLTexture2D::COpenGLTexture2D(const TVec2<uint32_t>& _Dimentions) :
		CTexture2D(Load(_Dimentions))
	{
		
	}

	COpenGLTexture2D::~COpenGLTexture2D()
	{
		glDeleteTextures(1, &mRendererID);
	}

	void COpenGLTexture2D::SetData(void* _Data, uint32_t _Size)
	{
		ARC_CORE_ASSERT(_Size == Dimensions.x * Dimensions.y * mBytesPerPixel, "Data must be entire texture");
		glTextureSubImage2D(mRendererID, 0, 0, 0, Dimensions.x, Dimensions.y, mDataFormat, GL_UNSIGNED_BYTE, _Data);
	}

	void COpenGLTexture2D::Bind(uint32_t _Slot /*= 0*/) const
	{
		glBindTextureUnit(_Slot, mRendererID);
	}

	void COpenGLTexture2D::ClearData()
	{
		if (mData)
		{
			stbi_image_free(mData);
		}
	}

	TVec4<unsigned char> COpenGLTexture2D::GetPixelColor(TVec2<uint32_t> xy)
	{
		TVec4<unsigned char> rval;
		const stbi_uc* p = mData + (4 * (xy.y * Dimensions.x + xy.x));
		rval.r = p[0];
		rval.g = p[1];
		rval.b = p[2];
		rval.a = p[3];
		return rval;
	}

	TVec2<uint32_t> COpenGLTexture2D::Load(const std::filesystem::path& _Path)
	{
		ARC_PROFILE_FUNCTION();
		int channels, width, height;
		stbi_set_flip_vertically_on_load(1);
		mData = stbi_load(_Path.string().c_str(), &width, &height, &channels, 0);
		ARC_CORE_ASSERT(mData, "Error loading texture from file");

		mInternalFormat = 0;
		mDataFormat = 0;
		mBytesPerPixel = channels;

		switch (channels)
		{
		case 4:
			mInternalFormat = GL_RGBA8;
			mDataFormat = GL_RGBA;
			break;
		case 3:
			mInternalFormat = GL_RGB8;
			mDataFormat = GL_RGB;
			break;
		case 2:
			mInternalFormat = GL_RG8;
			mDataFormat = GL_RG;
			break;
		case 1:
			mInternalFormat = GL_R;
			mDataFormat = GL_R;
			break;
		default:
			ARC_CORE_ASSERT(mData, "Texture format is not supported");
			break;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
		glTextureStorage2D(mRendererID, 1, mInternalFormat, width, height);

		glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(mRendererID, 0, 0, 0, width, height, mDataFormat, GL_UNSIGNED_BYTE, mData);
		return TVec2<uint32_t>(width, height);
	}

	TVec2<uint32_t> COpenGLTexture2D::Load(const TVec2<uint32_t>& _Dimentions)
	{
		ARC_PROFILE_FUNCTION();
		mInternalFormat = GL_RGBA8;
		mDataFormat = GL_RGBA;
		mBytesPerPixel = 4;

		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
		glTextureStorage2D(mRendererID, 1, mInternalFormat, _Dimentions.x, _Dimentions.y);

		glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		return _Dimentions;
	}
}