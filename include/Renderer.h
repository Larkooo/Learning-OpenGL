#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VertexArray.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "Camera.h"

#include <vector>

class Renderer
{
public:
	inline Renderer() {}
	inline ~Renderer() {};

	void Clear();
	void Render(const VertexArray& vao, const Shader& sProgram, const Camera& camera, std::vector<glm::vec3> cubes, float width, float height);
};

