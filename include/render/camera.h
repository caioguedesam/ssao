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
			float anglePitch = 0.f;
			float angleYaw = glm::radians(-90.f);
			float rotationSpeed = glm::radians(20.f);
			float fov = 0.f;
			float aspectRatio = 0.f;

			glm::ivec2 moveAmounts = glm::vec2(0);
			glm::ivec2 rotateAmounts = glm::vec2(0);

			void SetPosition(float x, float y, float z);
			void SetFront(glm::vec3 f);

			void Move(glm::vec3& dir, float dt);
			void Rotate(float anglesPitch, float anglesYaw, float dt);

			void SetPerspective(float newFov, float newAspect);

			void Init(float startX, float startY, float startZ, float startFov, float startAspect);

			glm::mat4 GetViewMatrix();
			glm::mat4 GetProjectionMatrix();

			void Update(float dt);
		};
	}
}