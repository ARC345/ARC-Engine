#include "arc_pch.h"
#include "Component.h"
#include "Entity.h"
#include "imgui.h"
#include "Scene.h"

namespace ARC
{
	void CNativeScriptComponent::OnConstruct(CEntity* pOwningEntity)
	{
		OwningEntity = pOwningEntity;
	}

	void CTransform2DComponent::DrawPropertiesUI(CEntity& pEntity)
	{
		FVec3 _ = {Transform.Location.x, Transform.Location.y, Transform.ZOrder};
		_.DrawGuiControl("Translation", 100.f, FVec3::ZeroVector);
		Transform.Location = {_.x, _.y};
		Transform.ZOrder = _.z;
		FVec1 _2 = Math::Conv<Radians, Degrees>(Transform.Rotation);
		_2.DrawGuiControl("Rotation", 100.f);
		Transform.Rotation = Math::Conv<Degrees, Radians>(_2.x);
		Transform.Scale.DrawGuiControl("Scale", 100.f, FVec2::OneVector);
	}

	void CCameraComponent::DrawPropertiesUI(CEntity& pEntity)
	{
		static const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
		const char* currentProjectionTypeString = projectionTypeStrings[(int)Camera.GetProjectionType()];

		if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
		{
			for (int i = 0; i < 2; i++)
			{
				bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
				if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
				{
					currentProjectionTypeString = projectionTypeStrings[i];
					Camera.SetProjectionType(CSceneCamera::EProjectionType(i));
				}
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		if (Camera.GetProjectionType() == CSceneCamera::EProjectionType::Orthographic)
		{
			float orthoSize = Camera.GetOrthographicSize();
			if (ImGui::DragFloat("Size", &orthoSize))
				Camera.SetOrthographicSize(orthoSize);

			float orthoNearClip = Camera.GetOrthographicNearClip();
			if (ImGui::DragFloat("NearClip", &orthoNearClip))
				Camera.SetOrthographicNearClip(orthoNearClip);

			float orthoFarClip = Camera.GetOrthographicFarClip();
			if (ImGui::DragFloat("FarClip", &orthoFarClip))
				Camera.SetOrthographicFarClip(orthoFarClip);
		}
		if (Camera.GetProjectionType() == CSceneCamera::EProjectionType::Perspective)
		{
			float perspectiveFOV = Math::Conv<Radians, Degrees>(Camera.GetPerspectiveFOV());
			if (ImGui::DragFloat("FOV", &perspectiveFOV))
				Camera.SetPerspectiveFOV(perspectiveFOV);

			float perspectiveNearClip = Camera.GetPerspectiveNearClip();
			if (ImGui::DragFloat("NearClip", &perspectiveNearClip))
				Camera.SetPerspectiveNearClip(perspectiveNearClip);

			float perspectiveFarClip = Camera.GetPerspectiveFarClip();
			if (ImGui::DragFloat("FarClip", &perspectiveFarClip))
				Camera.SetPerspectiveFarClip(perspectiveFarClip);
		}
	}

	void CNameComponent::DrawPropertiesUI(CEntity& pEntity)
	{
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), Name.c_str());
		if (ImGui::InputText("Name", buffer, sizeof(buffer)))
			Name = buffer;
	}

	void CSpriteRendererComponent::DrawPropertiesUI(CEntity& pEntity)
	{
		ImGui::ColorEdit4("Color", Color.Data());
	}
}
