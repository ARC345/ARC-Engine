#pragma once
#include <string>

namespace ARC {
	class CShader
	{
	public:
		virtual ~CShader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		static CShader* Create(const std::string& _VertexSrc, const std::string& _FragmentSrc);
	};
}