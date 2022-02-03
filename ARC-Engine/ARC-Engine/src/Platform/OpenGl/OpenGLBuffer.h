#pragma once
#include "ARC\Renderer\Buffer.h"

namespace ARC {
	class COpenGLVertexBuffer : public CVertexBuffer
	{
	public:
		COpenGLVertexBuffer(float* _Vertices, uint32_t _Size);
		virtual ~COpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void SetLayout(const CBufferLayout& _Layout) override { m_Layout = _Layout; }
		virtual const CBufferLayout& GetLayout() const override { return m_Layout; }

	private:
		uint32_t m_Renderer_id;
		CBufferLayout m_Layout;
	};
	
	class COpenGLIndexBuffer : public CIndexBuffer
	{
	public:
		COpenGLIndexBuffer(uint32_t* _Indiices, uint32_t _Count);
		virtual ~COpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual uint32_t GetCount() const override;

	private:
		uint32_t m_Renderer_id;
		uint32_t m_Count;
	};
}