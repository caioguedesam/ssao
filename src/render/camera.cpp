#include "stdafx.h"
#include "render/camera.h"

namespace Ty
{
	namespace Graphics
	{
		void Camera::set_position(float x, float y, float z)
		{
			position = { x, y, z };
		}

		void Camera::set_front(Math::v3f f)
		{
			Math::v3f result = f;
			front = normalize(result);
			right = normalize(Math::cross(front, {0.f, 1.f, 0.f}));
		}

		void Camera::move(Math::v3f& dir, float dt)
		{
			position = position + (speed * dt) * dir;
		}

		void Camera::rotate(float angles_pitch, float angles_yaw, float dt)
		{
			angle_pitch += angles_pitch * dt;
			angle_yaw += angles_yaw * dt;

			Math::v3f rotation;
			rotation.x = cos(angle_yaw) * cos(angle_pitch);
			rotation.y = sin(angle_pitch);
			rotation.z = sin(angle_yaw) * cos(angle_pitch);
			rotation = normalize(rotation);

			set_front(rotation);
		}

		void Camera::set_perspective(float new_fov, float new_aspect)
		{
			fov = new_fov;
			aspect_ratio = new_aspect;
		}

		void Camera::init(float start_x, float start_y, float start_z, float start_fov, float start_aspect)
		{
			set_position(start_x, start_y, start_z);
			set_front({0, 0, -1});
			set_perspective(start_fov, start_aspect);
		}

		Math::m4f Camera::get_view_matrix()
		{
			return Math::look_at(position, position + front, { 0.f, 1.f, 0.f });
		}

		Math::m4f Camera::get_projection_matrix()
		{
			return Math::perspective(fov, aspect_ratio, 0.1f, 1000.f);
		}

		void Camera::update(float dt)
		{
			if (move_amounts.x != 0 || move_amounts.y != 0)
			{
				auto move_front = (float)move_amounts.y * front;
				auto move_side = (float)move_amounts.x * right;
				auto move_dir = normalize(move_front + move_side);
				move(move_dir, dt);
			}

			Input::MouseData mouse_data = Input::InputManager::mouse_data;
			if (mouse_data.offset.x != 0 || mouse_data.offset.y != 0)
			{
				rotate(-mouse_data.offset.y * rotation_speed, mouse_data.offset.x * rotation_speed, dt);
			}
		}
	}
}