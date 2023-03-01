#pragma once
#include "ARC/Renderer/Layer.h"
#include "ARC/Scene/Component.h"

namespace ARC { class CScene; }
namespace ARC { class CEvent; }

namespace ARC {
	struct CNetForceComponent : public CComponentBase
	{
		FVec3 NetForce = FVec3::ZeroVector();
		virtual uint32_t GetFlags() override { return Flags; };

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags;
	};
}

namespace ARC
{
	class CLifeSim2D
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
