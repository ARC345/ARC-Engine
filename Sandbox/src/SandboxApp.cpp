
#include "PCH\arc_pch.h"
//--------EntryPoint-----------------
#include "ARC/Core/EntryPoint.h"
//-----------------------------------
#include <ARC.h>
#include "ARC\Events\Event.h"
#include "SandboxApp.h"
#include "ARC\GUI\ImGuiLayer.h"
#include "ARC\Helpers\Helpers.h"
#include "ARC\Types\vector.h"
#include "imgui\imgui.h"
#include "ARC\Types\TybeBase.h"
#include "ARC\Events\KeyEvent.h"
#include "glm\glm\ext\matrix_transform.hpp"
#include "glm\glm\ext\matrix_float4x4.hpp"
#include "ARC\Renderer\Color.h"
#include "Platform\OpenGl\OpenGLShader.h"
#include "glm\glm\gtc\type_ptr.inl"
#include "ARC\Renderer\Texture.h"

#define Ecs
#ifdef SpaceWar
	#include "SpaceWar\SpaceWar.h"
#elif defined Ecs
	#include "ECSExample.h"
#else
	#include "Sandbox2D.h"
#endif

arc::Core::application* arc::Core::create_application()
{
	return new sandbox;
}

//-------------------[Layer]----------------------//
example_layer::example_layer() : 
	arc::layer("Example"),
	m_CameraController(1280.f / 780.f, true)
{
	//-------------------------------+[Code for triangle]+-------------------------------//
	{
		float triangle_verts[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		arc::ref<arc::vertex_buffer> triangle_vertex_buffer;
		triangle_vertex_buffer = arc::vertex_buffer::create(triangle_verts, sizeof(triangle_verts));
		arc::buffer_layout triangle_layout = {
			{ arc::shader_data_type::Float3, "a_Position" },
			{ arc::shader_data_type::Float4, "a_Colour" }
		};
		triangle_vertex_buffer->set_layout(triangle_layout);

		m_TriangleVertexArray = arc::vertex_array::create();
		m_TriangleVertexArray->add_vertex_buffer(triangle_vertex_buffer);

		unsigned int triangle_indices[] = {
			0,1,2
		};

		arc::ref<arc::index_buffer> triangle_index_buffer;
		triangle_index_buffer = arc::index_buffer::create(triangle_indices, sizeof(triangle_indices) / sizeof(uint32_t));
		m_TriangleVertexArray->set_index_buffer(triangle_index_buffer);

		//m_TriangleShader = arc::shader::create();
	}
	//-------------------------------~[Code for triangle]~-------------------------------//
	//--------------------------------+[Code for square]+--------------------------------//
	m_SquareVertexArray = arc::vertex_array::create();

	float square_verts[5 * 4] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
	};

	arc::ref<arc::vertex_buffer> square_vertex_buffer;
	square_vertex_buffer = arc::vertex_buffer::create(square_verts, sizeof(square_verts));

	arc::buffer_layout square_layout = { 
		{ arc::shader_data_type::Float3, "a_Position" },
		{ arc::shader_data_type::Float2, "a_TexCoord" }
	};
	square_vertex_buffer->set_layout(square_layout);

	m_SquareVertexArray->add_vertex_buffer(square_vertex_buffer);

	unsigned int square_indices[] = { 0, 1, 2, 2, 3, 0 };

	arc::ref<arc::index_buffer> square_index_buffer;
	square_index_buffer = arc::index_buffer::create(square_indices, sizeof(square_indices) / sizeof(uint32_t));
	m_SquareVertexArray->set_index_buffer(square_index_buffer);
	//--------------------------------~[Code for square]~--------------------------------//
	//---------------------------+[Code for FlatColor Shader]+---------------------------//
	const std::string flat_color_vertex_source = R"(
				#version 330 core

				layout(location = 0) in vec3 a_Position;
				
				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;

				out vec3 v_Position;

				void main()
				{
					v_Position = a_Position;
					gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.f);
				}			
			)";
	const std::string flat_color_fragment_source = R"(
				#version 330 core

				layout(location = 0) out vec4 o_Color;

				in vec3 v_Position;
				
				uniform vec4 u_Color;

				void main()
				{
					o_Color = u_Color;
				}			
			)";
	m_FlatColorShader = arc::shader::create("FlatColorShader", flat_color_vertex_source, flat_color_fragment_source);

	//---------------------------~[Code for FlatColor Shader]~---------------------------//
	//----------------------------+[Code for Texture Shader]+----------------------------//
	auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");
	//----------------------------~[Code for Texture Shader]~----------------------------//

	m_Texture = arc::texture_2d::create("assets/textures/Checkerboard.png");
	m_TestTexture = arc::texture_2d::create("assets/textures/ChernoLogo.png");

	textureShader->bind();
	textureShader->Set<int>("u_Texture", 0);
}

void example_layer::on_update(float delta_time)
{
	m_CameraController.on_update(delta_time);

	arc::render_command::set_clear_colour({ .1f, .1f, .1f, 1.f });
	arc::render_command::clear();

	arc::renderer::begin_scene(m_CameraController.get_camera());
	//SQ_Data.Scale = {0.1f, 0.1f, 0.1f};

	m_FlatColorShader->bind();
	m_FlatColorShader->Set<arc::CColor>("u_Color", SQ_Colour);

	//for (size_t x = 0; x < 20; x++)
	//	for (size_t y = 0; y < 20; y++)
	//	{
	//		glm::vec3 pos = { x * 0.11f, y * 0.11f, 0.f };
	//		pos += SQ_Data.Position;
	//		glm::mat4 transform = glm::scale(glm::translate(glm::mat4(1.0f), pos), SQ_Data.Scale);
	//		arc::renderer::Submit(m_FlatColorShader, m_SquareVertexArray, transform);
	//	}

	m_Texture->bind();
	arc::renderer::Submit(m_ShaderLibrary.get_shader("Texture"), m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	m_TestTexture->bind();
	arc::renderer::Submit(m_ShaderLibrary.get_shader("Texture"), m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	//arc::renderer::Submit(m_TriangleShader, m_TriangleVertexArray);
	arc::renderer::end_scene();

	get_cam().RecalculateViewProjectionMatrix();
}

void example_layer::on_event(arc::event& event)
{
	m_CameraController.on_event(event);
}

bool example_layer::OnKeyPressedEvent(arc::key_pressed_event& event)
{
	return false;
}

void example_layer::on_gui_render()
{
	ImGui::begin("Sandbox");
	if (ImGui::TreeNode("Camera"))
 	{
// 		ImGui::DragFloat3("CamLocation", &GetCam().Position[0]);
// 		ImGui::DragFloat("CamMoveSpeed", &CamMoveSpeed);
// 		ImGui::DragFloat("CamRotation", &GetCam().Rotation, 1.0f, 0, 360);
// 		ImGui::DragFloat("CamRotSpeed", &CamRotSpeed);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Squares"))
	{
		ImGui::ColorEdit4("SQ_Colour", SQ_Colour.Data());
		ImGui::TreePop();
	}
	ImGui::end();
}

//-------------------[Layer]----------------------//
sandbox::sandbox()
{
	#ifdef SpaceWar
		push_layer(new space_war());
	#elif defined Ecs
		push_layer(new arc::ecs_example());
	#else
		push_layer(new sandbox_2d());
	#endif
}
