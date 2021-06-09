#include "Mesh.h"
#include "..\include\Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, Material material)
{
	m_Vertices = vertices;
	m_Indices = indices;
	m_Material = material;

	m_VAO = VertexArray();
	m_VAO.Bind();
	m_VBO = VertexBuffer(m_Vertices.data(), sizeof(m_Vertices));
	m_IBO = IndexBuffer(indices.data(), indices.size());

	// vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	// textures coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureCoords));
}

void Mesh::Render(Shader& shader)
{
	shader.Bind();
	
	// set textures
	shader.Set("uMaterial.diffuse", glm::ivec1(m_Material.GetDiffuse()));
	shader.Set("uMaterial.specular", glm::ivec1(m_Material.GetSpecular()));
	shader.Set("uMaterial.shininess", glm::vec1(m_Material.GetShininess()));

	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
	

}
