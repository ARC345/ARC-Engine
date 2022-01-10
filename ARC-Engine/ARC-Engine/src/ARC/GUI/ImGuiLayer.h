#pragma once
#include "ARC\Layers\Layer.h"

namespace ARC {
	class ARC_API CImGuiLayer : public CLayer
	{
	public:
		CImGuiLayer();
		~CImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnGuiRender() override;

		void Begin();
		void End();

	private:
		float m_Time = 0.f;
	};
}