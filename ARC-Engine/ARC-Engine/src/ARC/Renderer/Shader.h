#pragma once

#include "../../../out/glm/glm/glm.hpp"

namespace ARC {
	class CShader
	{
	public:
		CShader(const std::string& _VertexSrc, const std::string& _FragmentSrc);
		~CShader();

		void Bind();
		void UnBind();

		void UploadUniform4Mat(const std::string& _Name, const glm::mat4& _Matrix);

	private:
		uint32_t m_RendererID;
	};
}