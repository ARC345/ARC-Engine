#pragma once
#include <unordered_map>
#include "ARC\Types\Pointer.h"

namespace ARC {
	class CShader
	{
	public:
		virtual ~CShader() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const TString& GetName() const = 0;

		static TRef<CShader> Create(const TString& _Name, const TString& _VertexSrc, const TString& _FragmentSrc);
		static TRef<CShader> Create(const TString& _Path);

		template<typename T>
		inline void Set(const TString& _Name, const T& _Value);
		template<typename T>
		inline void SetArray(const TString& _Name, const T* _Value, uint32_t _Count);

	protected:
		virtual void SetInt(const TString& _Name, const int* _Value) = 0;
		virtual void SetInt2(const TString& _Name, const int* _Value) = 0;
		virtual void SetInt3(const TString& _Name, const int* _Value) = 0;
		virtual void SetInt4(const TString& _Name, const int* _Value) = 0;
		virtual void SetMat3(const TString& _Name, const float* _Value) = 0;
		virtual void SetMat4(const TString& _Name, const float* _Value) = 0;
		virtual void SetFloat(const TString& _Name, const float* _Value) = 0;
		virtual void SetFloat2(const TString& _Name, const float* _Value) = 0;
		virtual void SetFloat3(const TString& _Name, const float* _Value) = 0;
		virtual void SetFloat4(const TString& _Name, const float* _Value) = 0;
		
		virtual void SetIntArray(const TString& _Name, const int* _Values, uint32_t _Count) = 0;
		virtual void SetFloatArray(const TString& _Name, const float* _Values, uint32_t _Count) = 0;
	};

	class CShaderLibrary
	{
	public:
		void Add(const TRef<CShader>& _Shader);
		void Add(const TString& _Name, const TRef<CShader>& _Shader);
		TRef<CShader> Load(const TString& _Path, const TString& _Name = "");
	public:
		TRef<CShader> GetShader(const TString& _Name); 
		bool Exists(const TString& _Name) const; 
	private:
		std::unordered_map<TString, TRef<CShader>> mShaders;
	};
}