#pragma once

#include <glad/glad.h>s

#include <glm/glm.hpp>

#include "Material.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoords;
};

class Mesh
{
	std::vector<Vertex> m_Vertices;
	std::vector<uint32_t> m_Indices;
	Material m_Material;
	
	VertexArray m_VAO;
	VertexBuffer m_VBO;
	IndexBuffer m_IBO;

public:
	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Material material);

	void Render(Shader& shader) const;

	inline std::vector<Vertex> GetVertices() const { return m_Vertices; }
	inline std::vector<uint32_t> GetIndices() const { return m_Indices; }
	inline Material GetMaterial() const { return m_Material; }

	inline void SetMaterial(Material material) { m_Material = material; }
};

