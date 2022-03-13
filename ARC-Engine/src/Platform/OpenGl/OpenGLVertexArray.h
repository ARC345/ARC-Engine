#pragma once
#include "..\..\ARC\Renderer\VertexArray.h"

namespace ARC {
class COpenGLVertexArray : public CVertexArray
{
	
public:
	COpenGLVertexArray();
	virtual ~COpenGLVertexArray();

	/*
	* @note Do not add vertex buffer before assigning a layout
	*/
	virtual void AddVertexBuffer(const TRef<CVertexBuffer>& _VertexBuffer) override;
	virtual void SetIndexBuffer(const TRef<CIndexBuffer>& _IndexBuffer) override;

	virtual const std::vector<TRef<CVertexBuffer>>& GetVertexBuffers() const override;
	virtual const TRef<CIndexBuffer>& GetIndexBuffer() const override;

	virtual void Bind() const override;
	virtual void UnBind() const override;

private:
	uint32_t m_RendererId;
	std::vector<TRef<CVertexBuffer>> m_VertexBuffers;
	TRef<CIndexBuffer> m_IndexBuffer;
};
}