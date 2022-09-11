#include "HEADER/PUBLIC/Mesh/AssimpManager.h"

void Model::loadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // 노드의 모든 mesh들을 처리(만약 있다면)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // 그런 다음 각 자식들에게도 동일하게 적용
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

_assimpA::Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<_assimpA::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<_assimpA::Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        _assimpA::Vertex vertex;
        
        mesh->mVertices[i];
        
        vertices.push_back(vertex);
    }
    // indices 처리
    // 
        // material 처리
        if (mesh->mMaterialIndex >= 0)
        {

        }

    return _assimpA::Mesh(vertices, indices, textures);
}

std::vector<_assimpA::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<_assimpA::Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // 텍스처가 이미 불러와져있지 않다면 불러옵니다.
            _assimpA::Texture texture;
            texture.id = TextureFromFile(str.C_Str(), directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // 불러온 텍스처를 삽입합니다.
        }
    }
    return textures;
}