#pragma once
#include <string>

namespace ARC {
	class CEvent;

	class ARC_API CLayer
	{
	public:
		CLayer(const std::string& _name="Layer");
		virtual ~CLayer();

		virtual void OnAttach(){}
		virtual void OnDetach(){}
		virtual void OnUpdate(float _DeltaTime){}
		virtual void OnGuiRender(){}
		virtual void OnEvent(CEvent& _event){}

		inline const std::string& GetName() const { return mDebugName; }
	protected:
		std::string mDebugName;
	};
}