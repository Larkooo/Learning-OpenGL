#pragma once

#include <glad/glad.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

#include <vector>

class Model
{
	std::vector<Mesh> m_Meshes;
	std::string m_Directory;

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Material> LoadMaterial(aiMaterial* mat, aiTextureType type,
		std::string typeName);
public:
	Model(const std::string path);
	void Render(Shader& shader);
};

