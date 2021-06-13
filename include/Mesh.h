#pragma once

#include <glad/glad.h>s

#include <glm/glm.hpp>

#include "Material.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include <vector>

struct MeshTexture 
{
	uint32_t id;
	std::string type;
	std::string path;  // we store the path of the texture to compare with other textures
};

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
	std::vector<MeshTexture> m_Textures;
	
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<IndexBuffer> m_IBO;

public:
	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<MeshTexture> textures);

	void Render(Shader& shader) const;

	inline std::vector<Vertex> GetVertices() const { return m_Vertices; }
	inline std::vector<uint32_t> GetIndices() const { return m_Indices; }
	// inline Material GetMaterial() const { return m_Material; }

	// inline void SetMaterial(Material material) { m_Material = material; }
};

