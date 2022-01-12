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

	COpenGLTexture2D::~COpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	void COpenGLTexture2D::Bind(uint32_t _Slot /*= 0*/) const
	{
		glBindTextureUnit(_Slot, m_RendererID);
	}

	std::pair<uint32_t, uint32_t> COpenGLTexture2D::Load(const std::string& _Path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		m_Data = stbi_load(_Path.c_str(), &width, &height, &channels, 0);
		ARC_CORE_ASSERT(m_Data, "Error loading texture from file");

		GLenum internalFormat = 0;
		GLenum dataFormat = 0;

		switch (channels)
		{
		case 4:
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
			break;
		case 3:
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
			break;
		case 2:
			internalFormat = GL_RG8;
			dataFormat = GL_RG;
			break;
		case 1:
			internalFormat = GL_R;
			dataFormat = GL_R;
			break;

		default:
			ARC_CORE_ASSERT(m_Data, "Texture format is not supported");
			break;
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, width, height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, m_Data);

		return {width, height};
	}

}