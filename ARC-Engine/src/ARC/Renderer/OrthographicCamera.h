#pragma once
#include "ARC/Wrappers/Glm.h"

namespace arc {
	class orthographic_camera
	{
	public:
		orthographic_camera(float left, float right, float bottom, float top);
		void set_projection(float left, float right, float bottom, float top);
		
		inline const mat4& get_view_matrix() const { return view_matrix_; }
		inline const mat4& get_view_projection_matrix() const { return view_projection_matrix_; }
		void RecalculateViewProjectionMatrix();

	public:
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
		float Rotation = 0.0f;
	private:
		mat4 projection_matrix_;
		mat4 view_matrix_;
		mat4 view_projection_matrix_;
	};
}