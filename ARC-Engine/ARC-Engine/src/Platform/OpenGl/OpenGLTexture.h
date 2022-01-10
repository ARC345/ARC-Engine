#pragma once
#include "ARC\Renderer\Texture.h"
#include <utility>

namespace ARC {
	class COpenGLTexture2D :
		public CTexture2D
	{
		public:
			COpenGLTexture2D(const std::string& _Path);
			virtual ~COpenGLTexture2D() override;

			virtual void Bind(uint32_t _Slot = 0) const override;
		private:
			std::pair<uint32_t, uint32_t> Load(const std::string& _Path);
		private:
			unsigned char* m_data;
			std::string m_Path;
			uint32_t m_RendererID;
	};
}