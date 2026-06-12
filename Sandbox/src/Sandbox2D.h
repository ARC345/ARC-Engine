#pragma once
#include "ARC/Core/Application.h"
#include "ARC/Renderer/Layer.h"
#include "ARC/Renderer/OrthographicCameraController.h"
#include "ARC/Renderer/ParticleSystem2D.h"
#include "ARC/Objects/Primitive2D.h"

namespace arc { class framebuffer; }

namespace arc { class sub_texture_2d; }

namespace arc { class texture_2d; }

namespace arc { class vertex_array; }

namespace arc { class shader; }

namespace arc { class event; }

class sandbox_2d : public arc::layer
{
public:
	sandbox_2d();
	virtual ~sandbox_2d() = default;

	virtual void on_attach();
	virtual void on_detach();

	virtual void on_update(float delta_time) override;
	virtual void on_event(arc::event& event) override;
	virtual void on_gui_render() override;

	virtual void ___Print___(const char* _);

	float prev = 0.f;
protected:
	arc::ref<arc::shader> m_Shader;
	arc::ref<arc::vertex_array> m_VertexArray;
	arc::ref<arc::texture_2d> m_CheckerboardTexture;
	arc::ref<arc::texture_2d> m_Spritesheet;
	arc::ref<arc::sub_texture_2d> m_Tree;
	arc::ref<arc::particle_system_2d> ParticleSystem;
	arc::ref<arc::particle_system_2d> Smoke;

	arc::CColor SQ_Colour = arc::CColor::Blue;

private:
	arc::orthographic_camera_controller m_CameraController;
	std::unordered_map<char, arc::ref<arc::sub_texture_2d>> s_textureMap;
};