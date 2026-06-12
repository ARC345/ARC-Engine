#pragma once
#include <string>
#include "ARC/Core/Application.h"
#include "ARC/Renderer/Layer.h"
#include "ARC/Renderer/OrthographicCameraController.h"
#include "ARC/Objects/Primitive2D.h"
#include "ARC/Renderer/Color.h"

namespace arc { class texture_2d; }
namespace arc { class key_pressed_event; }
namespace arc { class event; }
namespace arc { class vertex_array; }
namespace arc { class index_buffer; }
namespace arc { class vertex_buffer; }
namespace arc { class shader; }


class example_layer : public arc::layer
{
public:
	example_layer(); 
	virtual void on_update(float delta_time) override;
	virtual void on_event(arc::event& _event) override;

	bool OnKeyPressedEvent(arc::key_pressed_event& event);

	virtual void on_gui_render() override;

	inline arc::COrthographicCameraBase& get_cam() { return m_CameraController.get_camera(); }

public:
	float SQ_MoveSpeed = 3.f;
	float SQ_RotSpeed = 180.f;

	arc::CColor SQ_Colour = arc::CColor::Red;

private:
	arc::shader_library m_ShaderLibrary;

	arc::ref<arc::shader> m_TriangleShader;
	arc::ref<arc::vertex_array> m_TriangleVertexArray;
	
	arc::ref<arc::shader> m_FlatColorShader;
	arc::ref<arc::vertex_array> m_SquareVertexArray;
	arc::ref<arc::texture_2d> m_Texture;
	arc::ref<arc::texture_2d> m_TestTexture;

	arc::orthographic_camera_controller m_CameraController;
};

class sandbox : public arc::Core::application
{
public:
	sandbox();
	~sandbox() {};

	inline virtual std::string get_app_name() override { return "Sandbox"; };
};
