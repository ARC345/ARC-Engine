#pragma once
#include <vector>
#include "ARC/Renderer/OrthographicCameraController.h"
#include <functional>
#include "ARC/Objects/Trial.h"

namespace ARC { class CShader; }

namespace ARC { class CVertexArray; }

namespace ARC { class CEvent; }

namespace ARC {
	using MyComponents = ECS::IMPL::ClassList<HPR::CustomCounter<ECS::ComponentCounterId>::Next()-1>::type;
	//using MyComponents = ECS::ComponentList<CPosition, CVelocity, CAcceleration, CRender, CHealth, CLife, CHitbox, CCooldown>;
	using MyTags = ECS::TagList<TPlayer, TBoss, TBullet, TPlayerBullet>;

	using SApplyVelocity = ECS::Signature<CPosition, CVelocity>; 
	using SApplyAcceleration = ECS::Signature<CVelocity, CAcceleration>;
	using SLife = ECS::Signature<CLife>;
	using SHealth = ECS::Signature<CHealth>;

	using SRender = ECS::Signature<CPosition, CRender>;
	using SRenderHitbox = ECS::Signature<CPosition, CHitbox>;

	using SPlayer = ECS::Signature<TPlayer, CPosition, CVelocity, CRender, CHitbox, CCooldown>;
	using SBoss = ECS::Signature<TBoss, CPosition, CVelocity, CRender, CHitbox, CHealth, CCooldown>;
	using SBullet = ECS::Signature<TBullet, CPosition, CHitbox>;
	using SPlayerBullet = ECS::Signature<TPlayerBullet, CPosition, CHitbox>;

	using MySignatures = ECS::SignatureList<SApplyVelocity, SApplyAcceleration, SLife, SHealth, SRender, SRenderHitbox, SPlayer, SBoss, SBullet, SPlayerBullet>;
	
	using MySettings = ECS::SSettings<MyComponents, MyTags, MySignatures>;
	using MyManager = ECS::CManager<MySettings>;

    class CECSExample :
        public CLayer
    {
	public:
		CECSExample();
		virtual ~CECSExample() = default;

	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float _DeltaTime) override;
		virtual void OnGuiRender() override;
		virtual void OnEvent(CEvent& _) override;

		void Render(ECS::EntityIndex_t _, CPosition& _0, CRender& _1);

		void updatePlayerInput(float mFT, CPosition& pPos, CVelocity& pVel, CCooldown& pCD);

		ECS::EntityIndex_t mkPlayer();
		ECS::EntityIndex_t mkBoss();
		ECS::EntityIndex_t mkBullet(const FVec2& mPos);
		ECS::EntityIndex_t mkPlayerBullet(const FVec2& mPos);

		TRef<CShader> m_Shader;
		TRef<CVertexArray> m_VertexArray;

	private:
		COrthographicCameraController m_CameraController;
		MyManager mgr;
	};

	static std::vector<std::function<void()>> beforeRefresh;
}