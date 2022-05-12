#pragma once

#include "ARC\Types\Transform2D.h"
#include "ARC/Types/Color.h"
#include "SceneCamera.h"
#include "ARC/Types/Delegate.h"
#include "Controller.h"
#include "ARC\Objects\Ecs.h"

namespace ARC {
	class CEntity;

	#define ARCComponent(x) static constexpr uint32_t Flags = x;

	struct CComponentHelper
	{
		template<typename T>
		static constexpr const char* GetName()
		{
			if (T::Flags & CComponentBase::AutoName)
			{
				return ECS::ComponentName<T>::Get()+1;
			}
			else
			{
				return T::ComponentName;
			}
		}
		template<typename T>
		static constexpr const char* GetFlags()
		{
			return T::Flags;
		}

	};

	struct CComponentBase
	{
		enum ComponentFlags : uint32_t
		{
			ComponentFlagsNone = 0,
			ShowInPropertiesPanel = 1 << 0,
			AutoName = 1 << 1,
		};

		virtual void OnConstruct(CEntity* pOwningEntity){};
		virtual void DrawPropertiesUI(CEntity& pEntity) {};

		static constexpr uint32_t Flags = 0|ShowInPropertiesPanel|AutoName;
		static constexpr const char* ComponentName = "";
	};

	struct CTransform2DComponent : public CComponentBase
	{
		FTransform2D Transform;

		CTransform2DComponent() = default;
		CTransform2DComponent(const CTransform2DComponent&) = default;
		CTransform2DComponent(const FTransform2D& pTransform) : Transform(pTransform){};

		virtual void DrawPropertiesUI(CEntity& pEntity) override;

		operator auto& () { return Transform; }
		operator const auto& () const { return Transform; }
	};
	
	struct CSpriteRendererComponent : public CComponentBase
	{
		CColor Color = CColor::White;

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

		static constexpr uint32_t Flags = 0 & ~ShowInPropertiesPanel | AutoName;

		template<typename T>
		void BindController()
		{
			InstantiateDelegate.Bind([]() { return new T(); });
			DestroyInstancesDelegate.Bind([](CEntityController*& pInst) { delete pInst; pInst = nullptr; });
		}
	};
	
	RegisterComponent(CNameComponent);
	RegisterComponent(CTransform2DComponent);
	RegisterComponent(CNativeScriptComponent);
	RegisterComponent(CSpriteRendererComponent);
	RegisterComponent(CCameraComponent);
}