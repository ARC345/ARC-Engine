#pragma once

#include "ARC/Renderer/Shader.h"
#include "../../../out/glm/glm/glm.hpp"

namespace ARC {
	class COpenGLShader :
		public CShader
	{
	public:
		COpenGLShader(const std::string& _VertexSrc, const std::string& _FragmentSrc);
		virtual ~COpenGLShader();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		template<typename T>
		void UploadUniform(const std::string& _Name, const T& _Value);

	private:
		uint32_t m_RendererID;
	};
}
