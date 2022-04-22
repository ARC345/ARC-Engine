#include "PCH/arc_pch.h"
#include "ECSExample.h"
#include "imgui/imgui.h"
#include "ARC/Renderer/Renderer2D.h"
#include "ARC/Renderer/RenderCommand.h"
#include "ARC/Input/Input.h"
#include "ARC/Input/KeyCodes.h"

namespace ARC {
	
	using namespace ECS;

	EntityIndex_t CECSExample::mkPlayer()
	{
		auto e(mgr.CreateIndex());
		mgr.AddTag<TPlayer>(e);

		auto& pos(mgr.AddComponent<CPosition>(e).value);
		auto& vel(mgr.AddComponent<CVelocity>(e).value);
		auto& hitbox(mgr.AddComponent<CHitbox>(e).Primitive);
		auto& shape(mgr.AddComponent<CRender>(e).Primitive);
		auto& cooldown(mgr.AddComponent<CCooldown>(e));

		pos = { 0.32f / 2.f, 0.17f };
		vel = { 0.f, 0.f };
		shape.Color = CColor::Green;
		FVec2 size{ 0.4f, 0.4f };
		shape.Transform.Scale = size;
		hitbox.Transform.Scale = size;
		cooldown.value = cooldown.maxValue = 1.6f;

		return e;
	}

	EntityIndex_t CECSExample::mkBoss()
	{
		auto e(mgr.CreateIndex());
		mgr.AddTag<TBoss>(e);
		
		auto& pos(mgr.AddComponent<CPosition>(e).value);
		auto& vel(mgr.AddComponent<CVelocity>(e).value);
		auto& hitbox(mgr.AddComponent<CHitbox>(e).Primitive);
		auto& shape(mgr.AddComponent<CRender>(e).Primitive);
		auto& health(mgr.AddComponent<CHealth>(e).value);
		auto& cooldown(mgr.AddComponent<CCooldown>(e));

		pos = { 0.32f / 2.f, 0.040f };
		vel = { 0.f, 0.f };
		shape.Color = CColor::Red;
		FVec2 size{ 0.6f, 0.6f };
		shape.Transform.Scale = size;
		hitbox.Transform.Scale = size;
		health = 10.f;
		cooldown.value = cooldown.maxValue = 7.f;

		return e;
	}

	EntityIndex_t CECSExample::mkBullet(const FVec2& mPos)
	{
		auto e(mgr.CreateIndex());
		mgr.AddTag<TBullet>(e);

		auto& pos(mgr.AddComponent<CPosition>(e).value);
		auto& vel(mgr.AddComponent<CVelocity>(e).value);
		auto& acc(mgr.AddComponent<CAcceleration>(e).value);
		auto& hitbox(mgr.AddComponent<CHitbox>(e).Primitive);
		auto& shape(mgr.AddComponent<CRender>(e).Primitive);
		auto& life(mgr.AddComponent<CLife>(e).value);

		pos = mPos;
		vel = { 0.f, -0.6f };
		acc = { HPR::Random::Float(-0.01f, 0.01f), HPR::Random::Float(-0.01f, 0.02f) };
		shape.Color = CColor::Red;
		shape.Transform.Scale = 0.011f;
		hitbox.Transform.Scale = 0.011f;
		life = 200.f;

		return e;
	}

	EntityIndex_t CECSExample::mkPlayerBullet(const FVec2& mPos)
	{
		auto e(mgr.CreateIndex());
		mgr.AddTag<TPlayerBullet>(e);

		auto& pos(mgr.AddComponent<CPosition>(e).value);
		auto& vel(mgr.AddComponent<CVelocity>(e).value);
		auto& acc(mgr.AddComponent<CAcceleration>(e).value);
		auto& hitbox(mgr.AddComponent<CHitbox>(e).Primitive);
		auto& shape(mgr.AddComponent<CRender>(e).Primitive);
		auto& life(mgr.AddComponent<CLife>(e).value);

		pos = mPos;
		vel = { 0.f, 0.6f };
		acc = { 0.f, 0.f };
		shape.Color = CColor(0, 1, 1, 1);
		shape.Transform.Scale = 0.011f;
		hitbox.Transform.Scale = 0.011f;
		life = 200.f;

		return e;
	}

	auto IsCollision(const CPosition& mP0, const CHitbox& mC0,
		const CPosition& mP1, const CHitbox& mC1)
	{
		auto dist(FVec2::Dist(mP0.value, mP1.value));
		return dist <= (mC0.Primitive.GetScale().x + mC1.Primitive.GetScale().x)/2; // @temp
	}

	template <typename TFunction>
	void CooldownAction(float _DeltaTime, CCooldown& mCD, TFunction&& mFunction)
	{
		mCD.value -= _DeltaTime;
		if (mCD.value > 0.f) return;

		beforeRefresh.emplace_back(mFunction);
		mCD.value = mCD.maxValue;
	}
	
	void CECSExample::updatePlayerInput(
		float mFT, CPosition& pPos, CVelocity& pVel, CCooldown& pCD)
	{
		constexpr float speed{ 2.f };
		FVec2 xyInput = {0.f, 0.f};
		bool shoot;

		using ARCK = ARC::CInput;

		// Get horizontal movement.
		if (ARCK::IsKeyPressed(ARC_KEY_LEFT))
			xyInput.x = -1;
		else if (ARCK::IsKeyPressed(ARC_KEY_RIGHT))
			xyInput.x = 1;

		// Get vertical movement.
		if (ARCK::IsKeyPressed(ARC_KEY_UP))
			xyInput.y = 1;
		else if (ARCK::IsKeyPressed(ARC_KEY_DOWN))
			xyInput.y = -1;

		// Get shooting state.
		shoot = ARCK::IsKeyPressed(ARC_KEY_Z);

		// Calculate velocity vector.
		auto radians = Math::Atan2(xyInput.y, xyInput.x);
		
		auto dir = FVec2(Math::Cos(radians), Math::Sin(radians));
		pVel.value = dir * speed * xyInput.Length();

		// Enqueue shooting action.
		CooldownAction(mFT, pCD, [this, shoot, pPos]
			{
				if (!shoot) return;
				mkPlayerBullet(pPos.value);
			});
	}

	CECSExample::CECSExample() :
		CLayer("ECSExample"),
		m_CameraController(1280.f / 780.f, true)
	{
		ARC_CORE_INFO("CECSExample: {}", __COUNTER__);
	}

	void CECSExample::OnAttach()
	{
		mkBoss();
		mkPlayer();
		m_CameraController.SetZoomLevel(1.f);
	}

	void CECSExample::OnDetach()
	{
	
	}

	void CECSExample::OnUpdate(float _DeltaTime)
	{
		{
			ARC_PROFILE_SCOPE("Stuff");
			m_CameraController.OnUpdate(_DeltaTime);

			CRenderCommand::SetClearColour({ .1f, .1f, .1f, 1.f });
			CRenderCommand::Clear();
		}

		ARC::CRenderer2D::ResetStats();

		mgr.UpdateMatchingEntities<SApplyVelocity>(
			[_DeltaTime](auto cEnt, auto& cPosition, auto& cVelocity)
			{
				cPosition.value += cVelocity.value * _DeltaTime;
			});

		mgr.UpdateMatchingEntities<SApplyAcceleration>(
			[_DeltaTime](auto, auto& cVelocity, auto& cAcceleration)
			{
				cVelocity.value += cAcceleration.value * _DeltaTime;
			});

		mgr.UpdateMatchingEntities<SRender>(
			[](auto, auto& cPosition, auto& cRender)
			{
				auto& s(cRender.Primitive);
				s.Transform.Location = cPosition.value;
			});
		mgr.UpdateMatchingEntities<SRenderHitbox>(
			[](auto, auto& cPosition, auto& cRender)
			{
				auto& s(cRender.Primitive);
				s.Transform.Location = cPosition.value;
			});

		mgr.UpdateMatchingEntities<SLife>([this, _DeltaTime](auto e, auto& cLife)
			{
				cLife.value -= _DeltaTime;

				if (cLife.value <= 0)
				{
					mgr.Kill(e);
				}
			});

		mgr.UpdateMatchingEntities<SPlayer>([this, _DeltaTime](
			auto pI, auto& pPos, auto& pVel, auto&, auto& pHitbox, auto& pCD)
			{
				updatePlayerInput(_DeltaTime, pPos, pVel, pCD);
		
				// Player/Boss logic.
				// If there is a player and a boss, the boss will move
				// towards the player and try to shoot.
				mgr.UpdateMatchingEntities<SBoss>([this, _DeltaTime, &pPos](auto,
					auto& bssPos, auto& bssVel, auto&, auto&, auto&, auto& bssCD)
					{
						constexpr auto speed(0.7f);
						auto left(pPos.value.x < bssPos.value.x);
						bssVel.value.x = left ? -speed : speed;

						CooldownAction(_DeltaTime, bssCD, [this, bssPos]
							{
								mkBullet(bssPos.value);
							});
					});
		
				// Player/Bullet logic.
				// Checks collisions between the player and enemy bullets.
				mgr.UpdateMatchingEntities<SBullet>([this, pI, &pPos, &pHitbox]
						(auto bltI, auto& bltPos, auto& bltHitbox)
					{
						bltHitbox.Primitive.Transform.ZOrder = 0.1;
						if (IsCollision(
							pPos, pHitbox, bltPos, bltHitbox))
							{
								mgr.Kill(bltI);
								mgr.Kill(pI);
							}
					});
			});

		// PlayerBullet logic.
		mgr.UpdateMatchingEntities<SPlayerBullet>(
			[this](auto pbI, auto& pbPos, auto& pbHitbox)
			{
				// PlayerBullet/Boss logic.
				// Checks collisions between the boss and player bullets.

				mgr.UpdateMatchingEntities<SBoss>(
					[this, pbI, &pbPos, &pbHitbox](auto bssI,
					auto& bssPos, auto&, auto&, auto& bssHitbox, auto& bssHealth, auto&)
					{
						if (IsCollision(pbPos, pbHitbox, bssPos, bssHitbox))
						{
							bssHealth.value -= 1;

							if (bssHealth.value <= 0.f)
							{
								mgr.Kill(bssI);
							}
							mgr.Kill(pbI);
						}
					});
			});
		
		for (auto& f : beforeRefresh) f();
		beforeRefresh.clear();

		mgr.Refresh();
                    
		CRenderer2D::BeginScene(m_CameraController.GetCamera());
		
		mgr.UpdateMatchingEntities<SRender>([](auto, auto&, auto& cRender)
			{
				CRenderer2D::DrawQuad(cRender.Primitive);
			});
		mgr.UpdateMatchingEntities<SRenderHitbox>([](auto, auto&, auto& cRender)
			{
				CRenderer2D::DrawQuad(cRender.Primitive);
			});

		CRenderer2D::EndScene();
	}

	void CECSExample::OnGuiRender()
	{
		ImGui::Begin("ECSExample");
		ImGui::Text("Entities: %i", mgr.GetEntityCount());
		mgr.UpdateMatchingEntities<SBoss>([](auto,
			auto&, auto&, auto&, auto&, auto& Health, auto&)
			{
				ImGui::Text("-Boss:\n--Health: %f", Health.value);
			});
		ImGui::End();
	}

	void CECSExample::OnEvent(ARC::CEvent& _Event)
	{
		ARC_PROFILE_FUNCTION();
		m_CameraController.OnEvent(_Event);
	}

	void CECSExample::Render(ECS::EntityIndex_t _, CPosition& _0, CRender& _1)
	{

	}
}