#pragma once
#include "ARC\Renderer\Layer.h"

namespace ARC {
	class ARC_API CImGuiLayer : public CLayer
	{
	public:
		CImGuiLayer();
		~CImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnGuiRender() override;
		virtual void OnEvent(CEvent& _e) override;

		void Begin();
		void End();

		void SetBlockEvents(bool _) {m_bBlockEvents = _; };
		void SetDarkThemeColors();
	private:
		uint8_t m_bBlockEvents : 1;
		float m_Time = 0.f;
	};
}