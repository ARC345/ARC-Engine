#pragma once
#include "ARC\Renderer\Camera.h"

namespace arc
{
	struct scene_camera : public camera
	{
	public:
		enum class projection_type { Perspective, Orthographic };

		scene_camera();
		virtual ~scene_camera() = default;

		projection_type get_projection_type() const { return current_projection_type_; }
		void set_viewport_size(vec2<uint32_t> new_size);

		void set_orthographic(float pSize, float pNearClip, float pFarClip);
		void set_perspective(float pFOV, float pNearClip, float pFarClip);

		float get_orthographic_size() const { return orthographic_size_; }
		float get_orthographic_near_clip() const { return orthographic_near_; }
		float get_orthographic_far_clip() const { return orthographic_far_; }
		
		float get_perspective_fov() const { return perspective_fov_; }
		float get_perspective_near_clip() const { return perspective_near_; }
		float get_perspective_far_clip() const { return perspective_far_; }
		
		void set_projection_type(projection_type pProjectionType) { current_projection_type_ = pProjectionType; RecalculateProjection(); }

		void set_orthographic_size(float _) { orthographic_size_ = _;	RecalculateProjection(); }
		void set_orthographic_near_clip(float _) { orthographic_near_ = _;	RecalculateProjection(); }
		void set_orthographic_far_clip(float _) { orthographic_far_ = _; RecalculateProjection(); }
		
		void set_perspective_fov(float _) { perspective_fov_ = _; RecalculateProjection(); }
		void set_perspective_near_clip(float _) { perspective_near_ = _; RecalculateProjection(); }
		void set_perspective_far_clip(float _) { perspective_far_ = _; RecalculateProjection(); }

	private:
		void RecalculateProjection();
	private:
		projection_type current_projection_type_ = projection_type::Orthographic;

		float orthographic_size_ = 16.0f;
		float orthographic_near_ = -1.0f;
		float orthographic_far_ = 1.0f;

		float perspective_fov_ = math::Conv<rot_type::Degrees, rot_type::Radians>(45.f);
		float perspective_near_ = 0.01f;
		float perspective_far_ = 1.f;

		float aspect_ratio_ = 0.f;
	};
}