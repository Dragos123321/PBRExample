#pragma once

#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../utils.h"

class Model {
    private:
        std::vector<Mesh> m_meshes;
        std::vector<Texture> m_loaded_textures;
        std::string m_directory;
        bool m_gamma;

        void loadModel(const std::string& path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* material, aiTextureType type, const std::string& typeName);

    public:
        Model(const std::string& path, bool gamma = false) : m_gamma{ gamma } {
            loadModel(path);
        }

        void Draw(Shader& shader);

        std::vector<Mesh>& meshes() { return m_meshes; }
        std::vector<Texture>& loaded_textures() { return m_loaded_textures; }
};