#pragma once
#include "..\Buffer\Buffer.h"
#include <memory>

namespace ARC {
	class CVertexArray
	{
	public:
		virtual ~CVertexArray() {};
		static CVertexArray* Create();

		virtual void AddVertexBuffer(const std::shared_ptr<CVertexBuffer>& _VertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<CIndexBuffer>& _IndexBuffer) = 0;
		
		virtual const std::vector<std::shared_ptr<CVertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const std::shared_ptr<CIndexBuffer>& GetIndexBuffer() const = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};
}