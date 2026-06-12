#include "arc_pch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace arc {
	orthographic_camera::orthographic_camera(float left, float right, float bottom, float top) :
		projection_matrix_(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
		view_matrix_(1.0f)
	{
		view_projection_matrix_ = projection_matrix_ * view_matrix_;
	}

	void orthographic_camera::set_projection(float left, float right, float bottom, float top)
	{
		projection_matrix_ = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		
		view_projection_matrix_ = projection_matrix_ * view_matrix_;
	}

	void orthographic_camera::RecalculateViewProjectionMatrix()
	{
		mat4 transform = glm::translate(mat4(1.0f), Position) *
			glm::rotate(mat4(1.0f), glm::radians(Rotation), glm::vec3(0, 0, 1));

		view_matrix_ = glm::inverse(transform);
		view_projection_matrix_ = projection_matrix_ * view_matrix_;
	}		
}