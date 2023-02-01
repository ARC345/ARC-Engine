#include "arc_pch.h"
#include "ARC/Renderer/ParticleSystem2D.h"
#include "ARC/Renderer/ParticleModifier2D.h"
#include "ARC/Objects/Primitive2D.h"

namespace ARC {
	CParticleSystem2D::CParticleSystem2D(uint32_t _PoolSize)
	{
		m_ParticlePool.resize(_PoolSize);

		m_PoolIndex = _PoolSize-1;
	}

	CParticleSystem2D::~CParticleSystem2D()
	{
		m_Modifiers.clear();
	}

	void CParticleSystem2D::Emit()
	{
		CParticle2D& particle = m_ParticlePool[m_PoolIndex];
		particle = Defaults;
		particle.bIsActive = true;
		particle.bInitial = true;
		
		for (auto& i : m_Modifiers[EVariationApplicationTime::OnEmit])
		{
			i->ApplyModifier(particle);
		}
		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}

	void CParticleSystem2D::OnUpdate(float _DeltaTime)
	{
		for (auto& particle : m_ParticlePool)
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
				for (auto& i : m_Modifiers[EVariationApplicationTime::OnUpdate])
				{
					i->ApplyInitialModifier(particle);
				}
				particle.bInitial=false;
			}
			else
			{
				for (auto& i : m_Modifiers[EVariationApplicationTime::OnUpdate])
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
		for (auto& particle : m_ParticlePool)
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

			CRenderer2D::DrawQuad(Quad);
		}
	}
}