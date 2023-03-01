#include "arc_pch.h"
#include "ColliderComponents.h"
#include "Entity.h"
#include "imgui.h"
#include "Scene.h"
#include "ARC/GUI/ImGuiHelper.h"

namespace ARC {

	void CCircleColliderComponent::OnConstruct(CEntity& pOwningEntity)
	{
		//if (pOwningEntity.HasComponent<CTransform2DComponent>()) {
		//	auto relativeLoc = pOwningEntity.GetComponent<CTransform2DComponent>().Location - Location;
		//}
	}

	void CCircleColliderComponent::DrawPropertiesUI(CEntity& pEntity)
	{
		SGuiHelper::DrawGuiControl(Location, "Translation", 100.f, FVec3::ZeroVector());
		SGuiHelper::DrawGuiControl( *((FVec1*)&Radius), "Radius", 100.f, FVec1::OneVector());
	}

	void CCircleColliderComponent::Serialize(YAML::Emitter& pOut)
	{
		pOut << 
			YAML::Key << "Location" << YAML::Value << Location <<
			YAML::Key << "Radius" << YAML::Value << Radius;
	}

	void CCircleColliderComponent::Deserialize(YAML::Node& pData)
	{
		Location = pData["Location"].as<FVec3>();
		Radius = pData["Radius"].as<float>();
	}
}