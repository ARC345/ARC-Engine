#include "arc_pch.h"
#include "OpenGLVertexArray.h"
#include "glad\glad.h"
#include "ARC\Renderer\Buffer.h"

namespace ARC {
	namespace HPR {
		static GLenum ShaderTypeToOpenGlBaseType(EShaderDataType _Type) {
			switch (_Type)
			{
			case EShaderDataType::Float:	return GL_FLOAT;
			case EShaderDataType::Float2:	return GL_FLOAT;
			case EShaderDataType::Float3:	return GL_FLOAT;
			case EShaderDataType::Float4:	return GL_FLOAT;
			case EShaderDataType::Mat3:		return GL_FLOAT;
			case EShaderDataType::Mat4:		return GL_FLOAT;
			case EShaderDataType::Int:		return GL_INT;
			case EShaderDataType::Int2:		return GL_INT;
			case EShaderDataType::Int3:		return GL_INT;
			case EShaderDataType::Int4:		return GL_INT;
			case EShaderDataType::Bool:		return GL_BOOL;
			}

			ARC_CORE_ASSERT(false, "UnknownShaderDataType");
				return 0;
		};
	}

	COpenGLVertexArray::COpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererId);
	}

	COpenGLVertexArray::~COpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererId);
	}
	
	void COpenGLVertexArray::AddVertexBuffer(const TRef<CVertexBuffer>& _VertexBuffer)
	{
		Bind();
		_VertexBuffer->Bind();

		ARC_CORE_ASSERT(_VertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		uint32_t index = 0;
		const auto& layout = _VertexBuffer->GetLayout();
		for (const auto& x : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, HPR::GetComponentCount(x.Type), HPR::ShaderTypeToOpenGlBaseType(x.Type), x.bNormalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)(uint64_t)x.Offset);
			++index;
		}

		m_VertexBuffers.push_back(_VertexBuffer);
	}

	void COpenGLVertexArray::SetIndexBuffer(const TRef<CIndexBuffer>& _IndexBuffer)
	{
		Bind();
		_IndexBuffer->Bind();

		m_IndexBuffer = _IndexBuffer;
	}

	const std::vector<TRef<CVertexBuffer>>& COpenGLVertexArray::GetVertexBuffers() const
	{
		return m_VertexBuffers;
	}

	const TRef<CIndexBuffer>& COpenGLVertexArray::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}
	void COpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererId);
	}

	void COpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

}