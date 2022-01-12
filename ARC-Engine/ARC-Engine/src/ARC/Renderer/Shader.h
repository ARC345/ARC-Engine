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