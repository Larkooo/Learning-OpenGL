#include "Model.h"

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    Material material;

    // vertices
    for (uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vertex.textureCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        vertices.push_back(vertex);
    }

    // indices
    for (uint32_t i = 0; i < mesh->mNumFaces; i++)
        for (uint32_t k = 0; k < mesh->mFaces[i].mNumIndices; k++)
            indices.push_back(mesh->mFaces[i].mIndices[k]);
}

Model::Model(const std::string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    m_Directory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);
}

void Model::Render(Shader& shader)
{
	for (const Mesh& mesh : m_Meshes)
	{
		mesh.Render(shader);
	}
}
