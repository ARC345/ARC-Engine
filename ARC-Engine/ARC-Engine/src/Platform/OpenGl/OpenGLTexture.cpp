#include "arc_pch.h"
#include "OpenGLTexture.h"
#include "stb_image.h"
#include "ARC\Core\Core.h"
#include "glad\glad.h"

namespace ARC {
	COpenGLTexture2D::COpenGLTexture2D(const std::string& _Path) : 
		CTexture2D(Load(_Path))
	{
		if (m_data)
		{	
			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, GL_RGB8, GetWidth(), GetHeight());

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, GetWidth(), GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, m_data);
	
			stbi_image_free(m_data);
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
		m_data = stbi_load(_Path.c_str(), &width, &height, &channels, 0);
		ARC_CORE_ASSERT(m_data, "Error loading texture from file");

		return {width, height};
	}

}