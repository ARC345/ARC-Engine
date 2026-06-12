#pragma once
#include <unordered_map>
#include "ARC\Types\Pointer.h"

namespace arc {
	class shader
	{
	public:
		virtual ~shader() = default;

		virtual void bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const string& get_name() const = 0;

		static ref<shader> create(const string& name, const string& vertex_src, const string& fragment_src);
		static ref<shader> create(const string& path);

		template<typename T>
		inline void Set(const string& name, const T& value);
		template<typename T>
		inline void set_array(const string& name, const T* value, uint32_t count);

	protected:
		virtual void set_int(const string& name, const int* value) = 0;
		virtual void set_int2(const string& name, const int* value) = 0;
		virtual void set_int3(const string& name, const int* value) = 0;
		virtual void set_int4(const string& name, const int* value) = 0;
		virtual void set_mat3(const string& name, const float* value) = 0;
		virtual void set_mat4(const string& name, const float* value) = 0;
		virtual void set_float(const string& name, const float* value) = 0;
		virtual void set_float2(const string& name, const float* value) = 0;
		virtual void set_float3(const string& name, const float* value) = 0;
		virtual void set_float4(const string& name, const float* value) = 0;
		
		virtual void set_int_array(const string& name, const int* values, uint32_t count) = 0;
		virtual void set_float_array(const string& name, const float* values, uint32_t count) = 0;
	};

	class shader_library
	{
	public:
		void Add(const ref<shader>& shader);
		void Add(const string& name, const ref<shader>& shader);
		ref<shader> Load(const string& path, const string& name = "");
	public:
		ref<shader> get_shader(const string& name); 
		bool Exists(const string& name) const; 
	private:
		std::unordered_map<string, ref<shader>> shaders_;
	};
}