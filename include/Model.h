#pragma once

#include <glad/glad.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"

#include <vector>

class Model
{
	std::vector<Mesh> m_Meshes;
	std::string m_Directory;
	std::vector<MeshTexture> m_LoadedTextures;

	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
		std::string typeName);
public:
	Model(const std::string path);
	void Render(Shader& shader) const;
};

