#include "arc_pch.h"
#include "OpenGLBuffer.h"
#include "glad\glad.h"

namespace ARC {
	//---------------------------[CVertexBuffer]----------------------------//
	
	COpenGLVertexBuffer::COpenGLVertexBuffer(uint32_t _Size)
	{
		glCreateBuffers(1, &mRenderer_id);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, _Size, nullptr, GL_DYNAMIC_DRAW);
	}
	COpenGLVertexBuffer::COpenGLVertexBuffer(float* _Vertices, uint32_t _Size)
	{
		glCreateBuffers(1, &mRenderer_id);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, _Size, _Vertices, GL_STATIC_DRAW);
	}

	COpenGLVertexBuffer::~COpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &mRenderer_id);
	}

	void COpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, mRenderer_id);
	}

	void COpenGLVertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void COpenGLVertexBuffer::SetData(const void* _Data, uint32_t _Size)
	{
		Bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, _Size, _Data);
	}

	//---------------------------[CIndexBuffer]----------------------------//

	COpenGLIndexBuffer::COpenGLIndexBuffer(uint32_t* _Indiices, uint32_t _Count)
		: mCount(_Count)
	{
		glCreateBuffers(1, &mRenderer_id);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _Count*sizeof(uint32_t), _Indiices, GL_STATIC_DRAW);
	}

	COpenGLIndexBuffer::~COpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &mRenderer_id);
	}

	void COpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRenderer_id);
	}

	void COpenGLIndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	uint32_t COpenGLIndexBuffer::GetCount() const
	{
		return mCount;
	}
}
