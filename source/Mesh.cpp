#include "Mesh.h"
#include "..\include\Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<MeshTexture> textures)
{
	m_Vertices = vertices;
	m_Indices = indices;
	m_Textures = textures;

	/* m_VAO = VertexArray();
	m_VAO.Bind();
	m_VBO = VertexBuffer(m_Vertices.data(), m_Vertices.size() * sizeof(Vertex));
	m_IBO = IndexBuffer(indices.data(), indices.size());*/
    m_VAO = std::unique_ptr<VertexArray>(new VertexArray());

    m_VAO->Bind();

    m_VBO = std::unique_ptr<VertexBuffer>(new VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex)));
    m_IBO = std::unique_ptr<IndexBuffer>(new IndexBuffer(indices.data(), indices.size()));

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

void Mesh::Render(Shader& shader) const
{	
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
   // std::cout << m_Textures[1].id << std::endl;
    for (unsigned int i = 0; i < m_Textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = m_Textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);

        shader.Set(("uMaterial." + name + number).c_str(), glm::ivec1(i));
    }

    // draw mesh
    m_VAO->Bind();
    glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
    //std::cout << glGetError() << std::endl;
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}
