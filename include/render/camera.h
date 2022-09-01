#pragma once
#include "stdafx.h"

#include "input/input.h"

namespace Ty
{
	namespace Graphics
	{
		class Camera
		{
		public:
			glm::vec3 position = glm::vec3(0.f);
			glm::vec3 front = glm::vec3(0.f);
			glm::vec3 right = glm::vec3(0.f);

			float speed = 10.f;
			float angle_pitch = 0.f;
			float angle_yaw = glm::radians(-90.f);
			float rotation_speed = glm::radians(20.f);
			float fov = 0.f;
			float aspect_ratio = 0.f;

			glm::ivec2 move_amounts = glm::vec2(0);
			glm::ivec2 rotate_amounts = glm::vec2(0);

			void set_position(float x, float y, float z);
			void set_front(glm::vec3 f);

			void move(glm::vec3& dir, float dt);
			void rotate(float angles_pitch, float angles_yaw, float dt);

			void set_perspective(float new_fov, float new_aspect);

			void init(float start_x, float start_y, float start_z, float start_fov, float start_aspect);

			glm::mat4 get_view_matrix();
			glm::mat4 get_projection_matrix();

			void update(float dt);
		};
	}
}