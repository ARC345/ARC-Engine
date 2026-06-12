#pragma once
#include <vector>
#include <string>
#include "..\Core\Core.h"

namespace arc {
	enum class shader_data_type : uint8_t
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	struct shader_helper {
		static uint32_t get_size(shader_data_type type)
		{
			switch (type)
			{
				case shader_data_type::Float:	return 4u;
				case shader_data_type::Float2:	return 4u * 2u;
				case shader_data_type::Float3:	return 4u * 3u;
				case shader_data_type::Float4:	return 4u * 4u;
				case shader_data_type::Mat3:		return 4u * 3u * 3u;
				case shader_data_type::Mat4:		return 4u * 4u * 4u;
				case shader_data_type::Int:		return 4u;	 
				case shader_data_type::Int2:		return 4u * 2u;
				case shader_data_type::Int3:		return 4u * 3u;
				case shader_data_type::Int4:		return 4u * 4u;
				case shader_data_type::Bool:		return 1u;
			}

			ARC_CORE_ASSERT(false, "UnknownShaderDataType");
			return 0;
		}
		static uint32_t get_component_count(shader_data_type type)
		{
			switch (type)
			{
				case shader_data_type::Float:	return 1u;
				case shader_data_type::Float2:	return 2u;
				case shader_data_type::Float3:	return 3u;
				case shader_data_type::Float4:	return 4u;
				case shader_data_type::Mat3:		return 3u * 3u;
				case shader_data_type::Mat4:		return 4u * 4u;
				case shader_data_type::Int:		return 1u;
				case shader_data_type::Int2:		return 2u;
				case shader_data_type::Int3:		return 3u;
				case shader_data_type::Int4:		return 4u;
				case shader_data_type::Bool:		return 1u;
			}

			ARC_CORE_ASSERT(false, "UnknownShaderDataType");
			return 0;
		}
	};

	struct buffer_element
	{
		std::string Name;
		shader_data_type Type;
		uint32_t Offset; 
		uint32_t size;
		uint8_t bNormalized : 1;

		buffer_element(shader_data_type type, const std::string& name, bool _bNormalized = false)
			: Name(name), Type(type), Offset(0u), size(shader_helper::get_size(type)), bNormalized(_bNormalized)
		{

		}
	};
	
	class buffer_layout
	{
	public:
		buffer_layout(){};
		buffer_layout(const std::initializer_list<buffer_element>& elem)
			: buffer_elements_(elem)
		{
			CalculateOffsetAndStride();
		};
		[[nodiscard]] inline const
		std::vector<buffer_element>& get_elements() const { return buffer_elements_; }
		[[nodiscard]] inline
		uint32_t get_stride() const { return stride_; }


		std::vector<buffer_element>::iterator begin() { return buffer_elements_.begin(); }
		std::vector<buffer_element>::iterator end() { return buffer_elements_.end(); }

		std::vector<buffer_element>::const_iterator begin() const { return buffer_elements_.begin(); }
		std::vector<buffer_element>::const_iterator end() const { return buffer_elements_.end(); }

	private:
		void CalculateOffsetAndStride() {
			uint32_t offset=0;
			stride_ = 0;
			for (auto& elem : buffer_elements_)
			{
				elem.Offset=offset;
				offset+=elem.size;
				stride_+=elem.size;
			}
		}

	private:
		std::vector<buffer_element> buffer_elements_;
		uint32_t stride_=0;
	};

	class vertex_buffer
	{
	public:
		virtual ~vertex_buffer() {};
		static ref<vertex_buffer> create(float* vertices, uint32_t size);
		static ref<vertex_buffer> create(uint32_t size);

		virtual void set_layout(const buffer_layout& layout) = 0;
		virtual const buffer_layout& get_layout() const = 0;
		
		virtual void set_data(const void* data, uint32_t size) = 0;

		virtual void bind() const = 0;
		virtual void UnBind() const = 0;
	};

	class index_buffer
	{
	public:
		virtual ~index_buffer() {};
		static ref<index_buffer> create(uint32_t* indices, uint32_t count);

		virtual void bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t get_count() const = 0;
	};
}