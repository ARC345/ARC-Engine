#include "arc_pch.h"
#include "OpenGLRendererAPI.h"
#include <glad/glad.h>
#include "ARC/Renderer/VertexArray.h"

namespace arc {

	void opengl_renderer_api::init()
	{
		ARC_PROFILE_FUNCTION();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_DEPTH_TEST);
	}

	void opengl_renderer_api::set_viewport(const vec2<u32> bottem_left_coord, const vec2<u32> dimentions)
	{
		glViewport(bottem_left_coord.x, bottem_left_coord.y, dimentions.x, dimentions.y);
	}

	void opengl_renderer_api::set_clear_colour(const color4 colour)
	{
		glClearColor(colour.r, colour.g, colour.b, colour.a);
	}

	void opengl_renderer_api::clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void opengl_renderer_api::draw_indexed(const ref<vertex_array>& vertex_array, u32 count)
	{
		ARC_PROFILE_FUNCTION();
		vertex_array->bind();
		u32 count = count ? count : vertex_array->get_index_buffer()->get_count();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void opengl_renderer_api::draw_line(const ref<vertex_array>& vertex_array, u32 count)
	{
		ARC_PROFILE_FUNCTION();
		vertex_array->bind();
		glDrawArrays(GL_LINES, 0, count);
	}

	void opengl_renderer_api::set_line_thickness(float pThickness)
	{
		glLineWidth(pThickness);
	}
}