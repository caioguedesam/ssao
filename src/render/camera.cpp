#include "stdafx.h"
#include "render/camera.h"
#include <glm/gtc/matrix_transform.hpp>

void Camera::SetPosition(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
}

void Camera::SetFront(glm::vec3 f)
{
	glm::vec3 result = f;
	front = glm::normalize(result);
	right = glm::normalize(glm::cross(front, glm::vec3(0.f, 1.f, 0.f)));
}

void Camera::Move(glm::vec3& dir, float dt)
{
	position += (speed * dt) * dir;
}

void Camera::Rotate(float anglesPitch, float anglesYaw, float dt)
{
	anglePitch += anglesPitch * dt;
	angleYaw += anglesYaw * dt;

	glm::vec3 rotationVector;
	rotationVector.x = cos(angleYaw) * cos(anglePitch);
	rotationVector.y = sin(anglePitch);
	rotationVector.z = sin(angleYaw) * cos(anglePitch);
	rotationVector = glm::normalize(rotationVector);

	SetFront(rotationVector);
}

void Camera::SetPerspective(float newFov, float newAspect)
{
	fov = newFov;
	aspectRatio = newAspect;
}

void Camera::Init(float startX, float startY, float startZ, float startFov, float startAspect)
{
	SetPosition(startX, startY, startZ);
	SetFront(glm::vec3(0, 0, -1));
	SetPerspective(startFov, startAspect);
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, position + front, glm::vec3(0.f, 1.f, 0.f));
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(fov, aspectRatio, 0.1f, 1000.f);
}

void Camera::Update(float dt)
{
	if (moveAmounts.x != 0 || moveAmounts.y != 0)
	{
		auto moveFront = (float)moveAmounts.y * front;
		auto moveSide = (float)moveAmounts.x * right;
		auto moveDir = glm::normalize(moveFront + moveSide);
		if (!glm::any(glm::isnan(moveDir)))
		{
			Move(moveDir, dt);
		}
	}

	MouseData mouseData = Input::mouseData;
	if (mouseData.offset.x != 0 || mouseData.offset.y != 0)
	{
		Rotate(-mouseData.offset.y * rotationSpeed, mouseData.offset.x * rotationSpeed, dt);
	}
}