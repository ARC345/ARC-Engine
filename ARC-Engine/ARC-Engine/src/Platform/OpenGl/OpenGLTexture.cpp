#include "arc_pch.h"
#include "OpenGLTexture.h"
#include "stb_image.h"
#include "ARC\Core\Core.h"
#include "glad\glad.h"

namespace ARC {
	COpenGLTexture2D::COpenGLTexture2D(const std::string& _Path) : 
		CTexture2D(Load(_Path))
	{
		if (m_Data)
		{
			stbi_image_free(m_Data);
		}
	}

	COpenGLTexture2D::COpenGLTexture2D(const TVec2<uint32_t>& _Dimentions) :
		CTexture2D(Load(_Dimentions))
	{
		
	}

	COpenGLTexture2D::~COpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void COpenGLTexture2D::SetData(void* _Data, uint32_t _Size)
	{
		ARC_CORE_ASSERT(_Size == Dimensions.x * Dimensions.y * m_BytesPerPixel, "Data must be entire texture");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, Dimensions.x, Dimensions.y, m_DataFormat, GL_UNSIGNED_BYTE, _Data);
	}

	void COpenGLTexture2D::Bind(uint32_t _Slot /*= 0*/) const
	{
		glBindTextureUnit(_Slot, m_RendererID);
	}

	TVec2<uint32_t> COpenGLTexture2D::Load(const std::string& _Path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		m_Data = stbi_load(_Path.c_str(), &width, &height, &channels, 0);
		ARC_CORE_ASSERT(m_Data, "Error loading texture from file");

		m_InternalFormat = 0;
		m_DataFormat = 0;
		m_BytesPerPixel = channels;

		switch (channels)
		{
		case 4:
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
			break;
		case 3:
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
			break;
		case 2:
			m_InternalFormat = GL_RG8;
			m_DataFormat = GL_RG;
			break;
		case 1:
			m_InternalFormat = GL_R;
			m_DataFormat = GL_R;
			break;
		default:
			ARC_CORE_ASSERT(m_Data, "Texture format is not supported");
			break;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, width, height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, m_DataFormat, GL_UNSIGNED_BYTE, m_Data);

		return TVec2<uint32_t>(width, height);
	}

	TVec2<uint32_t> COpenGLTexture2D::Load(const TVec2<uint32_t>& _Dimentions)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;
		m_BytesPerPixel = 4;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, _Dimentions.x, _Dimentions.y);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		return _Dimentions;
	}
}