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

		static constexpr uint32_t Flags = 0;
	};
	struct CElectricSignComponent : public CComponentBase
	{
		enum EElectricSign { Negative = -1, Neutral = 0, Positive = 1 };
		int8_t Sign = 0;

		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;
		virtual uint32_t GetFlags() override { return Flags; };

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};
}

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
