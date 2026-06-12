#include "arc_pch.h"
#include "Renderer.h"
#include "RenderCommand.h"
#include "ARC/Renderer/OrthographicCamera.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Platform/OpenGl/OpenGLShader.h"
#include "ARC/Types/Vector.h"
#include "Renderer2D.h"

namespace arc {
	renderer::scene_data* renderer::scene_data_ = new renderer::scene_data;

	void renderer::init()
	{
		ARC_PROFILE_FUNCTION();

		render_command::init();
		renderer_2d::init();
	}

	void renderer::on_window_resize(vec2<uint32_t> dimentions)
	{
		render_command::set_viewport(vec2<uint32_t>::ZeroVector(), dimentions);
	}

	void renderer::begin_scene(orthographic_camera& cam)
	{
		// Submit View Projection Matrix
		scene_data_->ViewProjectionMatrix = cam.get_view_projection_matrix();
	}

	void renderer::end_scene()
	{

	}

	void renderer::Submit(const ref<shader>& shader, const ref<vertex_array>& vertex_array, const mat4& transform)
	{
		shader->bind();
		shader->Set<mat4>("u_ViewProjection", scene_data_->ViewProjectionMatrix);
		shader->Set<mat4>("u_Transform", transform);
		vertex_array->bind();
		render_command::draw_indexed(vertex_array);
	}

}