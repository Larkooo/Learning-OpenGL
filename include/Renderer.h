#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "VertexArray.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "Camera.h"
#include "World.h"

#include <vector>

class Renderer
{
	std::unique_ptr<World> m_World;
public:
	inline Renderer(World* worldptr) : m_World(std::unique_ptr<World>(worldptr)) {};
	inline ~Renderer() {};

	void Clear();
	void Render();
};

