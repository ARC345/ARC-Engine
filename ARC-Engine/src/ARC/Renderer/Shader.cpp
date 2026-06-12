#include "arc_pch.h"
#include "Renderer.h"
#include "Platform/OpenGl/OpenGLShader.h"
#include "ARC/Core/Core.h"
#include "ARC/Types/Color.h"
#include "ARC/Wrappers/Glm.h"
#include "glm/gtx/compatibility.hpp"
#include "glm/gtc/type_ptr.inl"

namespace arc {
	ref<shader> shader::create(const string& name, const string& vertex_src, const string& fragment_src)
	{
		switch (renderer::get_current_api())
		{
		case renderer_api::renderer_api::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (renderer_api::None)");
			return nullptr;
		case renderer_api::renderer_api::OpenGL:
			return create_ref<opengl_shader>(name, vertex_src, fragment_src);
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}

	ref<shader> shader::create(const string& path)
	{
		switch (renderer::get_current_api())
		{
		case renderer_api::renderer_api::None:
			ARC_CORE_ASSERT(false, "Selected renderer API is not supported (renderer_api::None)");
			return nullptr;
		case renderer_api::renderer_api::OpenGL:
			return create_ref<opengl_shader>(path);
		}
		ARC_CORE_ASSERT(false, "Unknown renderer API");

		return nullptr;
	}

	void shader_library::Add(const ref<shader>& shader)
	{
		auto& name = shader->get_name();
		Add(name, shader);
	}

	void shader_library::Add(const string& name, const ref<shader>& shader)
	{
		ARC_CORE_ASSERT(!Exists(name), "Shader Already Exists");
		shaders_[name] = shader;
	}

	ref<shader> shader_library::Load(const string& path, const string& name /*= ""*/)
	{
		ARC_PROFILE_FUNCTION();
		auto shader = shader::create(path);
		if(name == "") Add(shader);
		else Add(name, shader);
		return shader;
	}

	ref<shader> shader_library::get_shader(const string& name)
	{
		ARC_CORE_ASSERT(Exists(name), "Shader Not Found");
		return shaders_[name];
	}

	bool shader_library::Exists(const string& name) const
	{
		return shaders_.find(name) != shaders_.end();
	}

	template<typename T>
	void shader::Set(const string& name, const T& value)
	{
		ARC_CORE_WARN("Uploading {0} to Shader is currently not supported", typeid(T).name());
	}

	template<>
	void shader::Set<vec4>(const string& name, const vec4& value)
	{
		set_float4(name, value.Data());
	}
	template<>
	void shader::Set<vec3>(const string& name, const vec3& value)
	{
		set_float3(name, value.Data());
	}
	template<>
	void shader::Set<vec2>(const string& name, const vec2& value)
	{
		set_float2(name, value.Data());
	}
	template<>
	void shader::Set<color4>(const string& name, const color4& value)
	{
		set_float4(name, value.Data());
	}
	template<>
	void shader::Set<vec4>(const string& name, const glm::vec4& value)
	{
		set_float4(name, glm::value_ptr(value));
	}
	template<>
	void shader::Set<mat4>(const string& name, const glm::mat4& value)
	{
		set_mat4(name, glm::value_ptr(value));
	}
	template<>
	void shader::Set<mat3>(const string& name, const glm::mat3& value)
	{
		set_mat3(name, glm::value_ptr(value));
	}
	template<>
	void shader::Set<float>(const string& name, const float& value)
	{
		set_float(name, &value);
	}
	template<>
	void shader::Set<glm::float2>(const string& name, const glm::float2& value)
	{
		set_float2(name, glm::value_ptr(value));
	}
	template<>
	void shader::Set<glm::float3>(const string& name, const glm::float3& value)
	{
		set_float3(name, glm::value_ptr(value));
	}
	template<>
	void shader::Set<int>(const string& name, const int& value)
	{
		set_int(name, &value);
	}

	template<typename T>
	void shader::set_array(const string& name, const T* value, uint32_t count)
	{
		ARC_CORE_WARN("Uploading {0} Array to Shader is currently not supported", typeid(T).name());
	}

	template<>
	void shader::set_array<int>(const string& name, const int* value, uint32_t count)
	{
		set_int_array(name, value, count);
	}
	template<>
	void shader::set_array<float>(const string& name, const float* value, uint32_t count)
	{
		set_float_array( name, value, count);
	}
}