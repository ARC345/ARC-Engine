#pragma once

#include "ARC/Renderer/Shader.h"
#include <unordered_map>

#include "glm/glm.hpp"
namespace arc {
	class opengl_shader :
		public shader
	{
	public:
		opengl_shader(const string& name, const string& vertex_src, const string& fragment_src);
		opengl_shader(const string& path);

		virtual ~opengl_shader();

		virtual void bind() const override;
		virtual void UnBind() const override;

		virtual inline const string& get_name() const override { return name_; };

	private:
		static unsigned int ShaderTypeFromString(string& type);
		static string StringFromShaderType(unsigned int type);

		std::unordered_map<unsigned int, string> PreProcess(const string& source);
		void Compile(const std::unordered_map<unsigned int, string>& shader_sources);
	private:
		static string seperator;
		string name_;
		uint32_t renderer_id_;
	protected:
		virtual void set_int(const string& name, const int* value) override;
		virtual void set_int2(const string& name, const int* value) override;
		virtual void set_int3(const string& name, const int* value) override;
		virtual void set_int4(const string& name, const int* value) override;
		virtual void set_mat4(const string& name, const float* value) override;
		virtual void set_mat3(const string& name, const float* value) override;
		virtual void set_float(const string& name, const float* value) override;
		virtual void set_float2(const string& name, const float* value) override;
		virtual void set_float3(const string& name, const  float*  value) override;
		virtual void set_float4(const string& name, const  float* value) override;

		virtual void set_int_array(const string& name, const int* values, uint32_t count) override;
		virtual void set_float_array(const string& name, const float* values, uint32_t count) override;

	};
}
