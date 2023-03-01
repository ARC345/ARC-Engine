#pragma once
#include <vector>
#include <string>
#include "..\Core\Core.h"

namespace ARC {
	enum class EShaderDataType : uint8_t
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	struct SShaderHelper {
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

			ARC_CORE_ASSERT(false, "UnknownShaderDataType");
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

			ARC_CORE_ASSERT(false, "UnknownShaderDataType");
			return 0;
		}
	};

	struct CBufferElement
	{
		std::string Name;
		EShaderDataType Type;
		uint32_t Offset; 
		uint32_t Size;
		uint8_t bNormalized : 1;

		CBufferElement(EShaderDataType _Type, const std::string& _Name, bool _bNormalized = false)
			: Name(_Name), Type(_Type), Offset(0u), Size(SShaderHelper::GetSize(_Type)), bNormalized(_bNormalized)
		{

		}
	};
	
	class CBufferLayout
	{
	public:
		CBufferLayout(){};
		CBufferLayout(const std::initializer_list<CBufferElement>& _Elem)
			: mBufferElements(_Elem)
		{
			CalculateOffsetAndStride();
		};
		[[nodiscard]] inline const
		std::vector<CBufferElement>& GetElements() const { return mBufferElements; }
		[[nodiscard]] inline
		uint32_t GetStride() const { return mStride; }


		std::vector<CBufferElement>::iterator begin() { return mBufferElements.begin(); }
		std::vector<CBufferElement>::iterator end() { return mBufferElements.end(); }

		std::vector<CBufferElement>::const_iterator begin() const { return mBufferElements.begin(); }
		std::vector<CBufferElement>::const_iterator end() const { return mBufferElements.end(); }

	private:
		void CalculateOffsetAndStride() {
			uint32_t offset=0;
			mStride = 0;
			for (auto& elem : mBufferElements)
			{
				elem.Offset=offset;
				offset+=elem.Size;
				mStride+=elem.Size;
			}
		}

	private:
		std::vector<CBufferElement> mBufferElements;
		uint32_t mStride=0;
	};

	class CVertexBuffer
	{
	public:
		virtual ~CVertexBuffer() {};
		static TRef<CVertexBuffer> Create(float* _Vertices, uint32_t _Size);
		static TRef<CVertexBuffer> Create(uint32_t _Size);

		virtual void SetLayout(const CBufferLayout& _Layout) = 0;
		virtual const CBufferLayout& GetLayout() const = 0;
		
		virtual void SetData(const void* _Data, uint32_t _Size) = 0;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
	};

	class CIndexBuffer
	{
	public:
		virtual ~CIndexBuffer() {};
		static TRef<CIndexBuffer> Create(uint32_t* _Indices, uint32_t _Count);

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual uint32_t GetCount() const = 0;
	};
}