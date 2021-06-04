#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, glm::vec3 rotation, float fov) : m_Front(glm::vec3(0.0f, 0.0f, -1.0f))
{
	this->position = position;
	m_WorldUp = up;
	this->rotation = rotation;

	this->fov = fov;

	UpdateVectors();
}

glm::mat4 Camera::GetView() const
{
	return glm::lookAt(position, position + m_Front, m_Up);
}

void Camera::UpdatePosition(Movement::Direction direction, float deltaTime)
{
	float velocity = Movement::Speed * deltaTime;
	switch (direction)
	{
	case Movement::Direction::FORWARD:
		position += m_Front * velocity;
		break;
	case Movement::Direction::BACKWARD:
		position -= m_Front * velocity;
		break;
	case Movement::Direction::LEFT:
		position -= m_Right * velocity;
		break;
	case Movement::Direction::RIGHT:
		position += m_Right * velocity;
		break;
	}
}

void Camera::UpdateRotation(glm::vec2 mouseOffset)
{
	// TODO : IMPLEMENT SENSITIVIT
	mouseOffset.x *= 0.3f;
	mouseOffset.y *= 0.3f;

	// Update rotation
	rotation.x += mouseOffset.x, rotation.y += mouseOffset.y;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (rotation.y > 89.0f)
		rotation.y = 89.0f;
	else if (rotation.y < -89.0f)
		rotation.y = -89.0f;
	

	// update Front, Right and Up Vectors using the updated Euler angles
	UpdateVectors();
}

void Camera::UpdateVectors()
{
	glm::vec3 tempFront;
	tempFront.x = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
	tempFront.y = sin(glm::radians(rotation.y));
	tempFront.z = sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
	
	m_Front = glm::normalize(tempFront);

	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
