 #include "arc_pch.h"
// #include "ECSExample.h"
// #include "imgui/imgui.h"
// #include "ARC/Renderer/Renderer2D.h"
// #include "ARC/Renderer/RenderCommand.h"
// #include "ARC/Input/Input.h"
// #include "ARC/Input/KeyCodes.h"
// 
// namespace arc {
// 	
// 	using namespace ECS;
// 
// 	EntityIndex_t ecs_example::mkPlayer()
// 	{
// 		auto e(mgr.CreateIndex());
// 		mgr.AddTag<TPlayer>(e);
// 
// 		auto& pos(mgr.add_component<CPosition>(e).value);
// 		auto& vel(mgr.add_component<CVelocity>(e).value);
// 		auto& hitbox(mgr.add_component<CHitbox>(e).Primitive);
// 		auto& shape(mgr.add_component<CRender>(e).Primitive);
// 		auto& cooldown(mgr.add_component<CCooldown>(e));
// 
// 		pos = { 0.32f / 2.f, 0.17f };
// 		vel = { 0.f, 0.f };
// 		shape.Color = CColor::Green;
// 		vec2 size{ 0.4f, 0.4f };
// 		shape.Transform.Scale = size;
// 		hitbox.Transform.Scale = size;
// 		cooldown.value = cooldown.maxValue = 1.6f;
// 
// 		return e;
// 	}
// 
// 	EntityIndex_t ecs_example::mkBoss()
// 	{
// 		auto e(mgr.CreateIndex());
// 		mgr.AddTag<TBoss>(e);
// 		
// 		auto& pos(mgr.add_component<CPosition>(e).value);
// 		auto& vel(mgr.add_component<CVelocity>(e).value);
// 		auto& hitbox(mgr.add_component<CHitbox>(e).Primitive);
// 		auto& shape(mgr.add_component<CRender>(e).Primitive);
// 		auto& health(mgr.add_component<CHealth>(e).value);
// 		auto& cooldown(mgr.add_component<CCooldown>(e));
// 
// 		pos = { 0.32f / 2.f, 0.040f };
// 		vel = { 0.f, 0.f };
// 		shape.Color = CColor::Red;
// 		vec2 size{ 0.6f, 0.6f };
// 		shape.Transform.Scale = size;
// 		hitbox.Transform.Scale = size;
// 		health = 10.f;
// 		cooldown.value = cooldown.maxValue = 7.f;
// 
// 		return e;
// 	}
// 
// 	EntityIndex_t ecs_example::mkBullet(const vec2& pos_)
// 	{
// 		auto e(mgr.CreateIndex());
// 		mgr.AddTag<TBullet>(e);
// 
// 		auto& pos(mgr.add_component<CPosition>(e).value);
// 		auto& vel(mgr.add_component<CVelocity>(e).value);
// 		auto& acc(mgr.add_component<CAcceleration>(e).value);
// 		auto& hitbox(mgr.add_component<CHitbox>(e).Primitive);
// 		auto& shape(mgr.add_component<CRender>(e).Primitive);
// 		auto& life(mgr.add_component<CLife>(e).value);
// 
// 		pos = pos_;
// 		vel = { 0.f, -0.6f };
// 		acc = { HPR::Random::Float(-0.01f, 0.01f), HPR::Random::Float(-0.01f, 0.02f) };
// 		shape.Color = CColor::Red;
// 		shape.Transform.Scale = 0.011f;
// 		hitbox.Transform.Scale = 0.011f;
// 		life = 200.f;
// 
// 		return e;
// 	}
// 
// 	EntityIndex_t ecs_example::mkPlayerBullet(const vec2& pos_)
// 	{
// 		auto e(mgr.CreateIndex());
// 		mgr.AddTag<TPlayerBullet>(e);
// 
// 		auto& pos(mgr.add_component<CPosition>(e).value);
// 		auto& vel(mgr.add_component<CVelocity>(e).value);
// 		auto& acc(mgr.add_component<CAcceleration>(e).value);
// 		auto& hitbox(mgr.add_component<CHitbox>(e).Primitive);
// 		auto& shape(mgr.add_component<CRender>(e).Primitive);
// 		auto& life(mgr.add_component<CLife>(e).value);
// 
// 		pos = pos_;
// 		vel = { 0.f, 0.6f };
// 		acc = { 0.f, 0.f };
// 		shape.Color = CColor(0, 1, 1, 1);
// 		shape.Transform.Scale = 0.011f;
// 		hitbox.Transform.Scale = 0.011f;
// 		life = 200.f;
// 
// 		return e;
// 	}
// 
// 	auto IsCollision(const CPosition& p0_, const CHitbox& c0_,
// 		const CPosition& p1_, const CHitbox& c1_)
// 	{
// 		auto dist(vec2::Dist(p0_.value, p1_.value));
// 		return dist <= (c0_.Primitive.GetScale().x + c1_.Primitive.GetScale().x)/2; // @temp
// 	}
// 
// 	template <typename TFunction>
// 	void CooldownAction(float delta_time, CCooldown& cd_, TFunction&& function_)
// 	{
// 		cd_.value -= delta_time;
// 		if (cd_.value > 0.f) return;
// 
// 		beforeRefresh.emplace_back(function_);
// 		cd_.value = cd_.maxValue;
// 	}
// 	
// 	void ecs_example::updatePlayerInput(
// 		float ft_, CPosition& pPos, CVelocity& pVel, CCooldown& pCD)
// 	{
// 		constexpr float speed{ 2.f };
// 		vec2 xyInput = {0.f, 0.f};
// 		bool shoot;
// 
// 		using ARCK = arc::CInput;
// 
// 		// Get horizontal movement.
// 		if (ARCK::is_key_pressed(ARC_KEY_LEFT))
// 			xyInput.x = -1;
// 		else if (ARCK::is_key_pressed(ARC_KEY_RIGHT))
// 			xyInput.x = 1;
// 
// 		// Get vertical movement.
// 		if (ARCK::is_key_pressed(ARC_KEY_UP))
// 			xyInput.y = 1;
// 		else if (ARCK::is_key_pressed(ARC_KEY_DOWN))
// 			xyInput.y = -1;
// 
// 		// Get shooting state.
// 		shoot = ARCK::is_key_pressed(ARC_KEY_Z);
// 
// 		// calculate velocity vector.
// 		auto radians = Math::Atan2(xyInput.y, xyInput.x);
// 		
// 		auto dir = vec2(Math::Cos(radians), Math::Sin(radians));
// 		pVel.value = dir * speed * xyInput.Length();
// 
// 		// Enqueue shooting action.
// 		CooldownAction(ft_, pCD, [this, shoot, pPos]
// 			{
// 				if (!shoot) return;
// 				mkPlayerBullet(pPos.value);
// 			});
// 	}
// 
// 	ecs_example::ecs_example() :
// 		layer("ECSExample"),
// 		m_CameraController(1280.f / 780.f, true)
// 	{
// 		ARC_CORE_INFO("ecs_example: {}", __COUNTER__);
// 	}
// 
// 	void ecs_example::on_attach()
// 	{
// 		mkBoss();
// 		mkPlayer();
// 		m_CameraController.SetZoomLevel(1.f);
// 	}
// 
// 	void ecs_example::on_detach()
// 	{
// 	
// 	}
// 
// 	void ecs_example::on_update(float delta_time)
// 	{
// 		{
// 			ARC_PROFILE_SCOPE("Stuff");
// 			m_CameraController.on_update(delta_time);
// 
// 			render_command::SetClearColour({ .1f, .1f, .1f, 1.f });
// 			render_command::clear();
// 		}
// 
// 		arc::CRenderer2D::ResetStats();
// 
// 		mgr.UpdateMatchingEntities<SApplyVelocity>(
// 			[delta_time](auto cEnt, auto& cPosition, auto& cVelocity)
// 			{
// 				cPosition.value += cVelocity.value * delta_time;
// 			});
// 
// 		mgr.UpdateMatchingEntities<SApplyAcceleration>(
// 			[delta_time](auto, auto& cVelocity, auto& cAcceleration)
// 			{
// 				cVelocity.value += cAcceleration.value * delta_time;
// 			});
// 
// 		mgr.UpdateMatchingEntities<SRender>(
// 			[](auto, auto& cPosition, auto& cRender)
// 			{
// 				auto& s(cRender.Primitive);
// 				s.Transform.Location = cPosition.value;
// 			});
// 		mgr.UpdateMatchingEntities<SRenderHitbox>(
// 			[](auto, auto& cPosition, auto& cRender)
// 			{
// 				auto& s(cRender.Primitive);
// 				s.Transform.Location = cPosition.value;
// 			});
// 
// 		mgr.UpdateMatchingEntities<SLife>([this, delta_time](auto e, auto& cLife)
// 			{
// 				cLife.value -= delta_time;
// 
// 				if (cLife.value <= 0)
// 				{
// 					mgr.Kill(e);
// 				}
// 			});
// 
// 		mgr.UpdateMatchingEntities<SPlayer>([this, delta_time](
// 			auto pI, auto& pPos, auto& pVel, auto&, auto& pHitbox, auto& pCD)
// 			{
// 				updatePlayerInput(delta_time, pPos, pVel, pCD);
// 		
// 				// Player/Boss logic.
// 				// If there is a player and a boss, the boss will move
// 				// towards the player and try to shoot.
// 				mgr.UpdateMatchingEntities<SBoss>([this, delta_time, &pPos](auto,
// 					auto& bssPos, auto& bssVel, auto&, auto&, auto&, auto& bssCD)
// 					{
// 						constexpr auto speed(0.7f);
// 						auto left(pPos.value.x < bssPos.value.x);
// 						bssVel.value.x = left ? -speed : speed;
// 
// 						CooldownAction(delta_time, bssCD, [this, bssPos]
// 							{
// 								mkBullet(bssPos.value);
// 							});
// 					});
// 		
// 				// Player/Bullet logic.
// 				// Checks collisions between the player and enemy bullets.
// 				mgr.UpdateMatchingEntities<SBullet>([this, pI, &pPos, &pHitbox]
// 						(auto bltI, auto& bltPos, auto& bltHitbox)
// 					{
// 						bltHitbox.Primitive.Transform.ZOrder = 0.1;
// 						if (IsCollision(
// 							pPos, pHitbox, bltPos, bltHitbox))
// 							{
// 								mgr.Kill(bltI);
// 								mgr.Kill(pI);
// 							}
// 					});
// 			});
// 
// 		// PlayerBullet logic.
// 		mgr.UpdateMatchingEntities<SPlayerBullet>(
// 			[this](auto pbI, auto& pbPos, auto& pbHitbox)
// 			{
// 				// PlayerBullet/Boss logic.
// 				// Checks collisions between the boss and player bullets.
// 
// 				mgr.UpdateMatchingEntities<SBoss>(
// 					[this, pbI, &pbPos, &pbHitbox](auto bssI,
// 					auto& bssPos, auto&, auto&, auto& bssHitbox, auto& bssHealth, auto&)
// 					{
// 						if (IsCollision(pbPos, pbHitbox, bssPos, bssHitbox))
// 						{
// 							bssHealth.value -= 1;
// 
// 							if (bssHealth.value <= 0.f)
// 							{
// 								mgr.Kill(bssI);
// 							}
// 							mgr.Kill(pbI);
// 						}
// 					});
// 			});
// 		
// 		for (auto& f : beforeRefresh) f();
// 		beforeRefresh.clear();
// 
// 		mgr.Refresh();
//                     
// 		CRenderer2D::begin_scene(m_CameraController.GetCamera());
// 		
// 		mgr.UpdateMatchingEntities<SRender>([](auto, auto&, auto& cRender)
// 			{
// 				CRenderer2D::draw_quad(cRender.Primitive);
// 			});
// 		mgr.UpdateMatchingEntities<SRenderHitbox>([](auto, auto&, auto& cRender)
// 			{
// 				CRenderer2D::draw_quad(cRender.Primitive);
// 			});
// 
// 		CRenderer2D::end_scene();
// 	}
// 
// 	void ecs_example::on_gui_render()
// 	{
// 		ImGui::begin("ECSExample");
// 		ImGui::Text("Entities: %i", mgr.get_entity_count());
// 		mgr.UpdateMatchingEntities<SBoss>([](auto,
// 			auto&, auto&, auto&, auto&, auto& Health, auto&)
// 			{
// 				ImGui::Text("-Boss:\n--Health: %f", Health.value);
// 			});
// 		ImGui::end();
// 	}
// 
// 	void ecs_example::on_event(arc::event& event)
// 	{
// 		ARC_PROFILE_FUNCTION();
// 		m_CameraController.on_event(event);
// 	}
// 
// 	void ecs_example::Render(ECS::EntityIndex_t _, CPosition& _0, CRender& _1)
// 	{
// 
// 	}
// }