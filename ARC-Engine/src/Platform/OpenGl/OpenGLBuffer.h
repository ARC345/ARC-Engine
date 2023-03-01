#pragma once
#include "ARC\Renderer\Buffer.h"

namespace ARC {
	class COpenGLVertexBuffer : public CVertexBuffer
	{
	public:
		COpenGLVertexBuffer(float* pVertices, TUInt32 pSize);
		COpenGLVertexBuffer(TUInt32 pSize);
		virtual ~COpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void SetLayout(const CBufferLayout& pLayout) override { mLayout = pLayout; }
		virtual const CBufferLayout& GetLayout() const override { return mLayout; }

		virtual void SetData(const void* pData, TUInt32 pSize) override;

	private:
		TUInt32 mRenderer_id;
		CBufferLayout mLayout;
	};
	
	class COpenGLIndexBuffer : public CIndexBuffer
	{
	public:
		COpenGLIndexBuffer(TUInt32* pIndiices, TUInt32 pCount);
		virtual ~COpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual TUInt32 GetCount() const override;

	private:
		TUInt32 mRenderer_id;
		TUInt32 mCount;
	};
}