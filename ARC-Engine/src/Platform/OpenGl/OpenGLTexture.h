#pragma once
#include "ARC\Renderer\Texture.h"
#include <utility>

namespace ARC {
	class COpenGLTexture2D :
		public CTexture2D
	{
		public:
			COpenGLTexture2D(const TVec2<uint32_t>& _Dimentions);
			COpenGLTexture2D(const std::string& _Path, bool bManualClear = false);
			virtual ~COpenGLTexture2D() override;

			virtual void SetData(void* _Data, uint32_t _Size) override;

			virtual void Bind(uint32_t _Slot = 0) const override;
			virtual void ClearData() override;

			virtual const TString& GetPath() const override { return mPath; };

			virtual TVec4<unsigned char> GetPixelColor(TVec2<uint32_t> xy) override;

			virtual bool operator==(const CTexture& _Tex) const override {
				return mRendererID == ((COpenGLTexture2D&)_Tex).mRendererID;
			};

			uint32_t GetRendererID() override { return mRendererID; };

	private:
			TVec2<uint32_t> Load(const TVec2<uint32_t>& _Dimentions);
			TVec2<uint32_t> Load(const std::string& _Path);
		private:
			uint32_t mBytesPerPixel;

			unsigned int mInternalFormat; //GLenum 
			unsigned int mDataFormat; //GLenum 
			unsigned char* mData;
			TString mPath;
			uint32_t mRendererID;
	};
}