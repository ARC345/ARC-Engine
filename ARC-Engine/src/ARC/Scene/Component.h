#pragma once

#include "ARC\Types\Transform2D.h"
#include "ARC/Types/Color.h"
#include "SceneCamera.h"
#include "ARC/Types/Delegate.h"
#include "Controller.h"
#include "ARC\Objects\Ecs.h"

namespace ARC {
	class CEntity;

	struct CComponentBase
	{
		enum ComponentFlags : uint32_t
		{
			ComponentFlagsNone = 0,
			ShowInPropertiesPanel = 1 << 0,
		};

		virtual void OnConstruct(CEntity* pOwningEntity){};
		virtual void DrawPropertiesUI(CEntity& pEntity) {};

		static constexpr uint32_t Flags = 0|ShowInPropertiesPanel;
		static constexpr const char* Name = "Unnamed Component";
	};

	struct CTransform2DComponent : public CComponentBase
	{
		FTransform2D Transform;

		CTransform2DComponent() = default;
		CTransform2DComponent(const CTransform2DComponent&) = default;
		CTransform2DComponent(const FTransform2D& pTransform) : Transform(pTransform){};

		virtual void DrawPropertiesUI(CEntity& pEntity) override;

		static constexpr uint32_t Flags = CComponentBase::Flags & ~ShowInPropertiesPanel;

		operator auto& () { return Transform; }
		operator const auto& () const { return Transform; }
	};
	
	struct CSpriteRendererComponent : public CComponentBase
	{
		CColor Color;

		CSpriteRendererComponent() = default;
		CSpriteRendererComponent(const CSpriteRendererComponent&) = default;
		CSpriteRendererComponent(const CColor& pColor) : Color(pColor){};

		virtual void DrawPropertiesUI(CEntity& pEntity) override;

		operator auto& () { return Color; }
		operator const auto& () const { return Color; }
	};
	
	struct CNameComponent : public CComponentBase
	{
		TString Name;

		CNameComponent() = default;
		CNameComponent(const CNameComponent&) = default;
		CNameComponent(const TString& pName) : Name(pName){};

		virtual void DrawPropertiesUI(CEntity& pEntity) override;

		operator auto& () { return Name; }
		operator const auto& () const { return Name; }
	};
	
	struct CCameraComponent : public CComponentBase
	{
		CSceneCamera Camera;
		virtual ~CCameraComponent() = default;

		uint8_t bPrimary : 1;
		uint8_t bFixedAspectRatio : 1;

		CCameraComponent() = default;
		CCameraComponent(const CCameraComponent&) = default;

		virtual void DrawPropertiesUI(CEntity& pEntity) override;

		operator auto& () { return Camera; }
		operator const auto& () const { return Camera; }
	};

	struct CNativeScriptComponent : public CComponentBase
	{
		CEntity* OwningEntity = nullptr;
		CEntityController* Controller = nullptr;

		CDelegate<CEntityController*()> InstantiateDelegate;
		CDelegate<void(CEntityController*&)> DestroyInstancesDelegate;

		virtual void OnConstruct(CEntity* pOwningEntity);

		template<typename T>
		void BindController()
		{
			InstantiateDelegate.Bind([]() { return new T(); });
			DestroyInstancesDelegate.Bind([](CEntityController*& pInst) { delete pInst; pInst = nullptr; });
		}
	};
	
	RegisterComponent(CNativeScriptComponent);
	RegisterComponent(CTransform2DComponent);
	RegisterComponent(CSpriteRendererComponent);
	RegisterComponent(CNameComponent);
	RegisterComponent(CCameraComponent);
}