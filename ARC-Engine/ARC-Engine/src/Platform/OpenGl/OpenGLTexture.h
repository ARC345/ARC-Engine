#pragma once
#include "ARC\Renderer\Texture.h"
#include <utility>

namespace ARC {
	class COpenGLTexture2D :
		public CTexture2D
	{
		public:
			COpenGLTexture2D(const TVec2<uint32_t>& _Dimentions);
			COpenGLTexture2D(const std::string& _Path);
			virtual ~COpenGLTexture2D() override;

			virtual void SetData(void* _Data, uint32_t _Size) override;

			virtual void Bind(uint32_t _Slot = 0) const override;
		private:
			TVec2<uint32_t> Load(const TVec2<uint32_t>& _Dimentions);
			TVec2<uint32_t> Load(const std::string& _Path);
		private:
			uint32_t m_BytesPerPixel;

			unsigned int m_InternalFormat; //GLenum 
			unsigned int m_DataFormat; //GLenum 

			unsigned char* m_Data;
			std::string m_Path;
			uint32_t m_RendererID;
	};
}