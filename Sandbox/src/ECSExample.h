// #pragma once
// #include <vector>
// #include "ARC/Renderer/OrthographicCameraController.h"
// #include <functional>
// #include "ARC/Objects/Trial.h"
// 
// namespace arc { class shader; }
// 
// namespace arc { class vertex_array; }
// 
// namespace arc { class event; }
// 
// namespace arc {
// 	using MyComponents = ECS::IMPL::ITRL_ComponentList<HPR::CustomCounter<ECS::ComponentCounterId>::Next()-1>::type;
// 	//using MyComponents = ECS::ComponentList<CPosition, CVelocity, CAcceleration, CRender, CHealth, CLife, CHitbox, CCooldown>;
// 	using MyTags = ECS::TagList<TPlayer, TBoss, TBullet, TPlayerBullet>;
// 
// 	using SApplyVelocity = ECS::Signature<CPosition, CVelocity>; 
// 	using SApplyAcceleration = ECS::Signature<CVelocity, CAcceleration>;
// 	using SLife = ECS::Signature<CLife>;
// 	using SHealth = ECS::Signature<CHealth>;
// 
// 	using SRender = ECS::Signature<CPosition, CRender>;
// 	using SRenderHitbox = ECS::Signature<CPosition, CHitbox>;
// 
// 	using SPlayer = ECS::Signature<TPlayer, CPosition, CVelocity, CRender, CHitbox, CCooldown>;
// 	using SBoss = ECS::Signature<TBoss, CPosition, CVelocity, CRender, CHitbox, CHealth, CCooldown>;
// 	using SBullet = ECS::Signature<TBullet, CPosition, CHitbox>;
// 	using SPlayerBullet = ECS::Signature<TPlayerBullet, CPosition, CHitbox>;
// 
// 	using MySignatures = ECS::SignatureList<SApplyVelocity, SApplyAcceleration, SLife, SHealth, SRender, SRenderHitbox, SPlayer, SBoss, SBullet, SPlayerBullet>;
// 	
// 	using MySettings = ECS::SSettings<MyComponents, MyTags, MySignatures>;
// 	using MyManager = ECS::CManager<MySettings>;
// 
//     class ecs_example :
//         public layer
//     {
// 	public:
// 		ecs_example();
// 		virtual ~ecs_example() = default;
// 
// 	public:
// 		virtual void on_attach() override;
// 		virtual void on_detach() override;
// 		virtual void on_update(float delta_time) override;
// 		virtual void on_gui_render() override;
// 		virtual void on_event(event& _) override;
// 
// 		void Render(ECS::EntityIndex_t _, CPosition& _0, CRender& _1);
// 
// 		void updatePlayerInput(float ft_, CPosition& pPos, CVelocity& pVel, CCooldown& pCD);
// 
// 		ECS::EntityIndex_t mkPlayer();
// 		ECS::EntityIndex_t mkBoss();
// 		ECS::EntityIndex_t mkBullet(const vec2& pos_);
// 		ECS::EntityIndex_t mkPlayerBullet(const vec2& pos_);
// 
// 		ref<shader> m_Shader;
// 		ref<vertex_array> m_VertexArray;
// 
// 	private:
// 		orthographic_camera_controller m_CameraController;
// 		MyManager mgr;
// 	};
// 
// 	static std::vector<std::function<void()>> beforeRefresh;
// }