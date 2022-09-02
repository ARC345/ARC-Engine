#pragma once 
#include "ARC/Renderer/Layer.h"
#include "ARC/Scene/Component.h"

namespace ARC { class CScene; }

namespace ARC { class CEvent; }

namespace ARC
{

	class CAtomExp
	{

	public:
		void OnAttach(TRef<CScene>& pActiveScene);
		void OnDetach();
		void OnUpdate(float pDeltaTime);
		void OnGuiRender();
		void OnEvent(CEvent& pEvent);

	private:
		TRef<CScene> m_ActiveScene;
	};
}