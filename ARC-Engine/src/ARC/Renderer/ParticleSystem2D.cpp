#include "arc_pch.h"
#include "ARC/Renderer/ParticleSystem2D.h"
#include "ARC/Renderer/ParticleModifier2D.h"
#include "ARC/Objects/Primitive2D.h"

namespace ARC {
	CParticleSystem2D::CParticleSystem2D(uint32_t _PoolSize)
	{
		mParticlePool.resize(_PoolSize);

		mPoolIndex = _PoolSize-1;
	}

	CParticleSystem2D::~CParticleSystem2D()
	{
		mModifiers.clear();
	}

	void CParticleSystem2D::Emit()
	{
		CParticle2D& particle = mParticlePool[mPoolIndex];
		particle = Defaults;
		particle.bIsActive = true;
		particle.bInitial = true;
		
		for (auto& i : mModifiers[EVariationApplicationTime::OnEmit])
		{
			i->ApplyModifier(particle);
		}
		mPoolIndex = --mPoolIndex % mParticlePool.size();
	}

	void CParticleSystem2D::OnUpdate(float _DeltaTime)
	{
		for (auto& particle : mParticlePool)
		{
			if (!particle.bIsActive)
				continue;

			if (particle.Life <= 0.0f)
			{
				particle.bIsActive = false;
				continue;
			}

			if (particle.bInitial)
			{
				for (auto& i : mModifiers[EVariationApplicationTime::OnUpdate])
				{
					i->ApplyInitialModifier(particle);
				}
				particle.bInitial=false;
			}
			else
			{
				for (auto& i : mModifiers[EVariationApplicationTime::OnUpdate])
				{
					i->ApplyModifier(particle);
				}
			}

			
			particle.Location += particle.Velocity * _DeltaTime;
			particle.Life -= _DeltaTime;
		}
	}

	void CParticleSystem2D::OnRender()
	{
		for (auto& particle : mParticlePool)
		{
			if (!particle.bIsActive)
				continue;
			
			CPrimitive2D Quad;
			Quad.Transform.Location = particle.Location;
			Quad.Transform.Rotation = particle.Rotation;
			Quad.Transform.Scale = particle.Scale;
			Quad.Color = particle.Color;
			Quad.Texture = particle.Texture;
			Quad.TextureScaling = { 1.f, 1.f };
			Quad.Transform.Rotation = SMath::Radians(particle.Rotation);

			SRenderer2D::DrawQuad(Quad);
		}
	}
}