#pragma once
#include "Component.h"

namespace ARC {
	struct CCircleColliderComponent : public CComponentBase
	{
		FVec3 Location; // World
		float Radius;

		CCircleColliderComponent() = default;
		CCircleColliderComponent(const CCircleColliderComponent&) = default;
		CCircleColliderComponent(const FVec3& pLocation, float pRadius) : Location(pLocation), Radius(pRadius) {};

		virtual void OnConstruct(CEntity& pOwningEntity) override;
		virtual void DrawPropertiesUI(CEntity& pEntity) override;
		virtual void Serialize(YAML::Emitter& pOut) override;
		virtual void Deserialize(YAML::Node& pData) override;
		virtual uint32_t GetFlags() override { return Flags; };

		static constexpr uint32_t Flags = ECF::DefaultComponentFlags | ECF::Serializable;
	};
}