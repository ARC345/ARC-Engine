#pragma once
#include "..\Buffer\Buffer.h"
#include <memory>

namespace ARC {
	class CVertexArray
	{
	public:
		virtual ~CVertexArray() {};
		static CVertexArray* Create();

		virtual void AddVertexBuffer(const TRef<CVertexBuffer>& _VertexBuffer) = 0;
		virtual void SetIndexBuffer(const TRef<CIndexBuffer>& _IndexBuffer) = 0;
		
		virtual const std::vector<TRef<CVertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const TRef<CIndexBuffer>& GetIndexBuffer() const = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};
}