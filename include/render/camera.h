#pragma once
#include "stdafx.h"

#include "core/math.h"
#include "input/input.h"

namespace Ty
{
	namespace Graphics
	{
		class Camera
		{
		public:
			Math::v3f position;
			Math::v3f front;
			Math::v3f right;

			float speed = 10.f;
			float angle_pitch = 0.f;
			float angle_yaw = Math::to_rad(-90.f);
			float rotation_speed = Math::to_rad(20.f);
			float fov = 0.f;
			float aspect_ratio = 0.f;

			Math::v2f move_amounts;
			Math::v2f rotate_amounts;

			void set_position(float x, float y, float z);
			void set_front(Math::v3f f);

			void move(Math::v3f& dir, float dt);
			void rotate(float angles_pitch, float angles_yaw, float dt);

			void set_perspective(float new_fov, float new_aspect);

			void init(float start_x, float start_y, float start_z, float start_fov, float start_aspect);

			Math::m4f get_view_matrix();
			Math::m4f get_projection_matrix();

			void update(float dt);
		};
	}
}