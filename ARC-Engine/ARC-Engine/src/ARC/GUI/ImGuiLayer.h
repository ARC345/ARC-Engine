#pragma once
#include "ARC\Layers\Layer.h"

namespace ARC {
	class ARC_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDeattach() override;
		void OnUpdate() override;
		void OnEvent(CEvent& _event) override;
	private:
		float m_Time = 0.f;
	};
}