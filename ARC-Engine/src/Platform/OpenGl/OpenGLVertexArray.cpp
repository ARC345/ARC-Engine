#include "arc_pch.h"
#include "OpenGLVertexArray.h"
#include "glad\glad.h"
#include "ARC\Renderer\Buffer.h"

namespace ARC {
	struct SOpenGLShaderHelper {
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
	};

	COpenGLVertexArray::COpenGLVertexArray()
	{
		glCreateVertexArrays(1, &mRendererId);
	}

	COpenGLVertexArray::~COpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &mRendererId);
	}
	
	void COpenGLVertexArray::AddVertexBuffer(const TRef<CVertexBuffer>& _VertexBuffer)
	{
		Bind();
		_VertexBuffer->Bind();

		ARC_CORE_ASSERT(_VertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		uint32_t index = 0;
		const auto& layout = _VertexBuffer->GetLayout();
		for (const auto& ielement : layout)
		{
			switch (ielement.Type)
			{
			case EShaderDataType::Float:
			case EShaderDataType::Float2:
			case EShaderDataType::Float3:
			case EShaderDataType::Float4:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index,
					SShaderHelper::GetComponentCount(ielement.Type),
					SOpenGLShaderHelper::ShaderTypeToOpenGlBaseType(ielement.Type),
					ielement.bNormalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)(const TUInt64)ielement.Offset);
				index++;
				break;
			}
			case EShaderDataType::Int:
			case EShaderDataType::Int2:
			case EShaderDataType::Int3:
			case EShaderDataType::Int4:
			case EShaderDataType::Bool:
			{
				glEnableVertexAttribArray(index);
				glVertexAttribIPointer(index,
					SShaderHelper::GetComponentCount(ielement.Type),
					SOpenGLShaderHelper::ShaderTypeToOpenGlBaseType(ielement.Type),
					layout.GetStride(),
					(const void*)(const TUInt64)ielement.Offset);
				index++;
				break;
			}
			case EShaderDataType::Mat3:
			case EShaderDataType::Mat4:
			{
				uint8_t count = SShaderHelper::GetComponentCount(ielement.Type);
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(index,
						count,
						SOpenGLShaderHelper::ShaderTypeToOpenGlBaseType(ielement.Type),
						ielement.bNormalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(ielement.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(index, 1);
					index++;
				}
				break;
			}
			default:
				ARC_CORE_ASSERT(false, "Unknown EShaderDataType!");
			}
		}

		mVertexBuffers.push_back(_VertexBuffer);
	}

	void COpenGLVertexArray::SetIndexBuffer(const TRef<CIndexBuffer>& _IndexBuffer)
	{
		Bind();
		_IndexBuffer->Bind();

		mIndexBuffer = _IndexBuffer;
	}

	const std::vector<TRef<CVertexBuffer>>& COpenGLVertexArray::GetVertexBuffers() const
	{
		return mVertexBuffers;
	}

	const TRef<CIndexBuffer>& COpenGLVertexArray::GetIndexBuffer() const
	{
		return mIndexBuffer;
	}
	void COpenGLVertexArray::Bind() const
	{
		glBindVertexArray(mRendererId);
	}

	void COpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

}