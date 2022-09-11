#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <DirectXMath.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace _assimpA
{
    struct Vertex
    {
        DirectX::XMFLOAT3 Position;
        DirectX::XMFLOAT3 Normal;
        DirectX::XMFLOAT2 TexCoords;
    };

    struct Texture
    {
        unsigned int id;
        std::string type;
    };

    class Mesh {
    public:
        /*  Mesh ������  */
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        /*  �Լ�         */
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader shader);
    private:
        /*  ���� ������  */
        unsigned int VAO, VBO, EBO;
        /*  �Լ�         */
        void setupMesh();
    };
}

class Model
{
public:
    Model(char* path)
    {
        loadModel(path);
    }
    void Draw(_assimpA::Shader shader);

private:
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    _assimpA::Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<_assimpA::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        std::string typeName);

private:
    std::vector<_assimpA::Mesh> meshes;
    std::string directory;

};