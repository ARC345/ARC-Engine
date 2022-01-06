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
	virtual void AddVertexBuffer(const std::shared_ptr<CVertexBuffer>& _VertexBuffer) override;
	virtual void SetIndexBuffer(const std::shared_ptr<CIndexBuffer>& _IndexBuffer) override;

	virtual const std::vector<std::shared_ptr<CVertexBuffer>>& GetVertexBuffers() const override;
	virtual const std::shared_ptr<CIndexBuffer>& GetIndexBuffer() const override;

	virtual void Bind() const override;
	virtual void UnBind() const override;

private:
	uint32_t m_RendererId;
	std::vector<std::shared_ptr<CVertexBuffer>> m_VertexBuffers;
	std::shared_ptr<CIndexBuffer> m_IndexBuffer;
};
}