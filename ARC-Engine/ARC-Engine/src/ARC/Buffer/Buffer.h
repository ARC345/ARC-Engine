#pragma once
#include <vector>
#include <string>

namespace ARC {
	enum class EShaderDataType : uint8_t
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	namespace HPR {
		static uint32_t GetSize(EShaderDataType _Type)
		{
			switch (_Type)
			{
				case EShaderDataType::Float:	return 4u;
				case EShaderDataType::Float2:	return 4u * 2u;
				case EShaderDataType::Float3:	return 4u * 3u;
				case EShaderDataType::Float4:	return 4u * 4u;
				case EShaderDataType::Mat3:		return 4u * 3u * 3u;
				case EShaderDataType::Mat4:		return 4u * 4u * 4u;
				case EShaderDataType::Int:		return 4u;	 
				case EShaderDataType::Int2:		return 4u * 2u;
				case EShaderDataType::Int3:		return 4u * 3u;
				case EShaderDataType::Int4:		return 4u * 4u;
				case EShaderDataType::Bool:		return 1u;
			}

			ARC_CORE_ASSERT(false, "UnknownShaderDataType")
			return 0;
		}
		static uint32_t GetComponentCount(EShaderDataType _Type)
		{
			switch (_Type)
			{
				case EShaderDataType::Float:	return 1u;
				case EShaderDataType::Float2:	return 2u;
				case EShaderDataType::Float3:	return 3u;
				case EShaderDataType::Float4:	return 4u;
				case EShaderDataType::Mat3:		return 3u * 3u;
				case EShaderDataType::Mat4:		return 4u * 4u;
				case EShaderDataType::Int:		return 1u;
				case EShaderDataType::Int2:		return 2u;
				case EShaderDataType::Int3:		return 3u;
				case EShaderDataType::Int4:		return 4u;
				case EShaderDataType::Bool:		return 1u;
			}

			ARC_CORE_ASSERT(false, "UnknownShaderDataType")
			return 0;
		}
	}

	struct SBufferElement
	{
		std::string Name;
		EShaderDataType Type;
		uint32_t Offset; 
		uint32_t Size;
		uint8_t bNormalized : 1;

		SBufferElement(EShaderDataType _Type, const std::string& _Name, bool _bNormalized = false)
			: Name(_Name), Type(_Type), Offset(0u), Size(HPR::GetSize(_Type)), bNormalized(_bNormalized)
		{

		}
	};
	
	class CBufferLayout
	{
	public:
		CBufferLayout(){};
		CBufferLayout(const std::initializer_list<SBufferElement>& _Elem)
			: m_BufferElements(_Elem)
		{
			CalculateOffsetAndStride();
		};
		inline const std::vector<SBufferElement>& GetElements() const { return m_BufferElements; }
		inline uint32_t GetStride() const { return m_Stride; }


		std::vector<SBufferElement>::iterator begin() { return m_BufferElements.begin(); }
		std::vector<SBufferElement>::iterator end() { return m_BufferElements.end(); }

		std::vector<SBufferElement>::const_iterator begin() const { return m_BufferElements.begin(); }
		std::vector<SBufferElement>::const_iterator end() const { return m_BufferElements.end(); }

	private:
		void CalculateOffsetAndStride() {
			uint32_t offset=0;
			m_Stride = 0;
			for (auto& elem : m_BufferElements)
			{
				elem.Offset=offset;
				offset+=elem.Size;
				m_Stride+=elem.Size;
			}
		}

	private:
		std::vector<SBufferElement> m_BufferElements;
		uint32_t m_Stride=0;
	};

	class CVertexBuffer
	{
	public:
		virtual ~CVertexBuffer() {};
		static CVertexBuffer* Create(float* _Vertices, uint32_t _Size);

		virtual void SetLayout(const CBufferLayout& _Layout) = 0;
		virtual const CBufferLayout& GetLayout() const = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};

	class CIndexBuffer
	{
	public:
		virtual ~CIndexBuffer() {};
		static CIndexBuffer* Create(uint32_t* _Indices, uint32_t _Count);

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetCount() const = 0;
	};
}