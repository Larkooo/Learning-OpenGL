#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

class Camera
{
private:	
	glm::vec3 m_Front;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	glm::vec3 m_WorldUp;

public:
	glm::vec3 position;
	glm::vec3 rotation;

	float fov;

public:
	static const struct Movement
	{
		inline static float Speed = 2.0f;
		enum class Direction
		{
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT
		};
	};

public:
	Camera(glm::vec3 position = { 0.0f, 0.0f, 0.0f }, glm::vec3 up = { 0.0f, 1.0f, 0.0f }, glm::vec3 rotation = {-90.0f, 0.0f, 0.0f}, float fov = 45.0f);
	// ~Camera();

	/* inline glm::vec3 GetPosition() const { return position; }
	inline glm::vec3 GetRotation() const { return rotation; }
	inline float GetFov() const { return fov; }
	inline void SetFov(float fov) { fov = fov; } */
	glm::mat4 GetView() const;

	void UpdatePosition(Movement::Direction direction, float deltaTime);
	void UpdateRotation(glm::vec2 mouseOffset);

private:
	void UpdateVectors();
};

