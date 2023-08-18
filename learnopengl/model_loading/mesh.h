#pragma once

#include <vector>
#include <string>

#include "../shader.h"

#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tangent;
};

struct Texture {
    unsigned int id;
    std::string type;    
    std::string path;
};

class Mesh {
    private:
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        std::vector<Texture> m_textures;

        unsigned int VBO;
        unsigned int EBO;

        void setup();
        
    public:
        unsigned int VAO;
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures); 

        void Draw(Shader& shader);

        std::vector<Vertex>& Vertices() {return m_vertices;}
        std::vector<unsigned int>& Indices() {return m_indices;}
        std::vector<Texture>& Textures() {return m_textures;}
};