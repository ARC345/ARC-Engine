#pragma once
#include <string>

namespace ARC {
	class CEvent;

	class ARC_API Layer
	{
	public:
		Layer(const std::string& _name="Layer");
		virtual ~Layer();

		virtual void OnAttach(){}
		virtual void OnDeattach(){}
		virtual void OnUpdate(){}
		virtual void OnEvent(CEvent& _event){}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}