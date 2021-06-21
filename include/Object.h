#pragma once

#include "Model.h"
#include "Shader.h"
#include <glm/glm.hpp>

class Object
{
	std::unique_ptr<Shader> m_Shader = nullptr;
	std::unique_ptr<Model> m_Model = nullptr;
	glm::vec3 m_Position;

public:
	inline Object(Shader* shader, Model* model, glm::vec3 position) : m_Shader(shader), m_Model(model), m_Position(position) {}
	inline Object() {}

	void Render();
};

