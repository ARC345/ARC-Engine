#include "arc_pch.h"
#include "SceneCamera.h"
#include "glm\ext\matrix_clip_space.inl"

namespace arc {

	scene_camera::scene_camera()
	{
		RecalculateProjection();
	}

	void scene_camera::set_orthographic(float pSize, float pNearClip, float pFarClip)
	{
		current_projection_type_ = projection_type::Orthographic;
		orthographic_size_ = pSize;
		orthographic_near_ = pNearClip;
		orthographic_far_ = pFarClip;
		RecalculateProjection();
	}

	void scene_camera::set_perspective(float pFOV, float pNearClip, float pFarClip)
	{
		current_projection_type_ = projection_type::Perspective;
		perspective_fov_ = pFOV;
		perspective_near_ = pNearClip;
		perspective_far_ = pFarClip;
		RecalculateProjection();
	}

	void scene_camera::set_viewport_size(vec2<uint32_t> pSize)
	{
		aspect_ratio_ = (float)pSize.x / (float)pSize.y;
		RecalculateProjection();
	}

	void scene_camera::RecalculateProjection()
	{
		switch (current_projection_type_)
		{
		case projection_type::Orthographic:
		{
			float orthoLeft = -orthographic_size_ * aspect_ratio_ * 0.5f;
			float orthoRight = orthographic_size_ * aspect_ratio_ * 0.5f;
			float orthoBottom = -orthographic_size_ * 0.5f;
			float orthoTop = orthographic_size_ * 0.5f;
			projection_ = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, orthographic_near_, orthographic_far_);
			break;
		}
		case projection_type::Perspective:
		{
			projection_ = glm::perspective(perspective_fov_, aspect_ratio_, perspective_near_, perspective_far_);
			break;
		}
		default:
			break;
		}
	}
}