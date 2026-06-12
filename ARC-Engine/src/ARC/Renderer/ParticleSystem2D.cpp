#include "arc_pch.h"
#include "ARC/Renderer/ParticleSystem2D.h"
#include "ARC/Renderer/ParticleModifier2D.h"
#include "ARC/Objects/Primitive2D.h"

namespace arc {
	particle_system_2d::particle_system_2d(uint32_t pool_size)
	{
		particle_pool_.resize(pool_size);

		pool_index_ = pool_size-1;
	}

	particle_system_2d::~particle_system_2d()
	{
		modifiers_.clear();
	}

	void particle_system_2d::Emit()
	{
		particle_2d& particle = particle_pool_[pool_index_];
		particle = Defaults;
		particle.bIsActive = true;
		particle.bInitial = true;
		
		for (auto& i : modifiers_[variation_application_time::OnEmit])
		{
			i->ApplyModifier(particle);
		}
		pool_index_ = --pool_index_ % particle_pool_.size();
	}

	void particle_system_2d::on_update(float delta_time)
	{
		for (auto& particle : particle_pool_)
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
				for (auto& i : modifiers_[variation_application_time::on_update])
				{
					i->ApplyInitialModifier(particle);
				}
				particle.bInitial=false;
			}
			else
			{
				for (auto& i : modifiers_[variation_application_time::on_update])
				{
					i->ApplyModifier(particle);
				}
			}

			
			particle.Location += particle.Velocity * delta_time;
			particle.Life -= delta_time;
		}
	}

	void particle_system_2d::OnRender()
	{
		for (auto& particle : particle_pool_)
		{
			if (!particle.bIsActive)
				continue;
			
			primitive_2d Quad;
			Quad.Transform.Location = particle.Location;
			Quad.Transform.Rotation = particle.Rotation;
			Quad.Transform.Scale = particle.Scale;
			Quad.Color = particle.Color;
			Quad.Texture = particle.Texture;
			Quad.TextureScaling = { 1.f, 1.f };
			Quad.Transform.Rotation = math::Radians(particle.Rotation);

			renderer_2d::draw_quad(Quad);
		}
	}
}