#include "stdafx.h"
#include "render/camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Ty
{
	namespace Graphics
	{
		void Camera::set_position(float x, float y, float z)
		{
			position = glm::vec3(x, y, z);
		}

		void Camera::set_front(glm::vec3 f)
		{
			glm::vec3 result = f;
			front = glm::normalize(result);
			right = glm::normalize(glm::cross(front, glm::vec3(0.f, 1.f, 0.f)));
		}

		void Camera::move(glm::vec3& dir, float dt)
		{
			position += (speed * dt) * dir;
		}

		void Camera::rotate(float angles_pitch, float angles_yaw, float dt)
		{
			angle_pitch += angles_pitch * dt;
			angle_yaw += angles_yaw * dt;

			glm::vec3 rotation;
			rotation.x = cos(angle_yaw) * cos(angle_pitch);
			rotation.y = sin(angle_pitch);
			rotation.z = sin(angle_yaw) * cos(angle_pitch);
			rotation = glm::normalize(rotation);

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
			set_front(glm::vec3(0, 0, -1));
			set_perspective(start_fov, start_aspect);
		}

		glm::mat4 Camera::get_view_matrix()
		{
			return glm::lookAt(position, position + front, glm::vec3(0.f, 1.f, 0.f));
		}

		glm::mat4 Camera::get_projection_matrix()
		{
			return glm::perspective(fov, aspect_ratio, 0.1f, 1000.f);
		}

		void Camera::update(float dt)
		{
			if (move_amounts.x != 0 || move_amounts.y != 0)
			{
				auto move_front = (float)move_amounts.y * front;
				auto move_side = (float)move_amounts.x * right;
				auto move_dir = glm::normalize(move_front + move_side);
				if (!glm::any(glm::isnan(move_dir)))
				{
					move(move_dir, dt);
				}
			}

			Input::MouseData mouse_data = Input::InputManager::mouse_data;
			if (mouse_data.offset.x != 0 || mouse_data.offset.y != 0)
			{
				rotate(-mouse_data.offset.y * rotation_speed, mouse_data.offset.x * rotation_speed, dt);
			}
		}
	}
}