#include "arc_pch.h"
#include "ColliderComponents.h"
#include "Entity.h"
#include "imgui.h"
#include "Scene.h"
#include "ARC/GUI/ImGuiHelper.h"

namespace ARC {
	void CCircleColliderComponent::DrawPropertiesUI(CEntity& pEntity)
	{
		SGuiHelper::DrawGuiControl(Location, "Translation", 100.f, FVec3::ZeroVector());
		SGuiHelper::DrawGuiControl(*((FVec1*)&Radius), "Radius", 100.f, FVec1::OneVector());
		SGuiHelper::DrawGuiControl(*((FVec1*)&Restitution), "Restitution", 100.f, FVec1::OneVector());
		bool b_ = bIgnoreOwningEntity;
		ImGui::Checkbox("bIgnoreOwningEntity", &b_);
		bIgnoreOwningEntity = b_;
	}

	void CCircleColliderComponent::Serialize(YAML::Emitter& pOut)
	{
		pOut <<
			YAML::Key << "Location" << YAML::Value << Location <<
			YAML::Key << "Radius" << YAML::Value << Radius <<
			YAML::Key << "Restitution" << YAML::Value << Restitution <<
			YAML::Key << "IgnoreOwningEntity" << YAML::Value << bIgnoreOwningEntity;
	}

	void CCircleColliderComponent::Deserialize(YAML::Node& pData)
	{
		Location = pData["Location"].as<FVec3>();
		Radius = pData["Radius"].as<float>();
		Restitution = pData["Restitution"].as<float>();
		bIgnoreOwningEntity = pData["bIgnoreOwningEntity"].as<bool>();
	}

	void CPolyColliderComponent::DrawPropertiesUI(CEntity& pEntity)
	{
		int i = 0;
		for (auto& _ : Vertices)
		{
			SGuiHelper::DrawGuiControl(_, std::to_string(i++).c_str(), 100.f, FVec3::ZeroVector());
		}

		if (ImGui::Button("Add Vertex")) {
			Vertices.push_back(FVec3::ZeroVector());
		}

		bool b_ = bIgnoreOwningEntity;
		ImGui::Checkbox("bIgnoreOwningEntity", &b_);
		bIgnoreOwningEntity = b_;
	}

	void CPolyColliderComponent::Serialize(YAML::Emitter& pOut)
	{
		pOut << YAML::Key << "Vertices" << YAML::Value << Vertices <<
			YAML::Key << "IgnoreOwningEntity" << YAML::Value << bIgnoreOwningEntity;

	}

	void CPolyColliderComponent::Deserialize(YAML::Node& pData)
	{
		Vertices = pData.as<std::vector<FVec3>>();
		bIgnoreOwningEntity = pData["bIgnoreOwningEntity"].as<bool>();
	}

}