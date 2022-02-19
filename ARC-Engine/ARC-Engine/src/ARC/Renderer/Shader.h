#pragma once
#include <string>
#include <unordered_map>
#include "ARC\Types\Pointer.h"

namespace ARC {
	class CShader
	{
	public:
		virtual ~CShader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const std::string& GetName() const = 0;

		static TRef<CShader> Create(const std::string& _Name, const std::string& _VertexSrc, const std::string& _FragmentSrc);
		static TRef<CShader> Create(const std::string& _Path);

		template<typename T>
		inline void Set(const std::string& _Name, const T& _Value);
		template<typename T>
		inline void SetArray(const std::string& _Name, const T* _Value, uint32_t _Count);

	protected:
		virtual void SetInt(const std::string& _Name, const int* _Value) = 0;
		virtual void SetInt2(const std::string& _Name, const int* _Value) = 0;
		virtual void SetInt3(const std::string& _Name, const int* _Value) = 0;
		virtual void SetInt4(const std::string& _Name, const int* _Value) = 0;
		virtual void SetMat3(const std::string& _Name, const float* _Value) = 0;
		virtual void SetMat4(const std::string& _Name, const float* _Value) = 0;
		virtual void SetFloat(const std::string& _Name, const float* _Value) = 0;
		virtual void SetFloat2(const std::string& _Name, const float* _Value) = 0;
		virtual void SetFloat3(const std::string& _Name, const float* _Value) = 0;
		virtual void SetFloat4(const std::string& _Name, const float* _Value) = 0;
		
		virtual void SetIntArray(const std::string& _Name, const int* _Values, uint32_t _Count) = 0;
		virtual void SetFloatArray(const std::string& _Name, const float* _Values, uint32_t _Count) = 0;
	};

	class CShaderLibrary
	{
	public:
		void Add(const TRef<CShader>& _Shader);
		void Add(const std::string& _Name, const TRef<CShader>& _Shader);
		TRef<CShader> Load(const std::string& _Path, const std::string& _Name = "");
	public:
		TRef<CShader> GetShader(const std::string& _Name); 
		bool Exists(const std::string& _Name) const; 
	private:
		std::unordered_map<std::string, TRef<CShader>> m_Shaders;
	};
}