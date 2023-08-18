#ifndef __UTILS__
#define __UTILS__

#include "stb_image.h"

#include <string>
#include <iostream>

inline unsigned int textureFromFile(const char* path, const std::string& directory, bool gamma = false) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, channels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

    if (data) {
        GLenum format;
        GLenum internalFormat;
        switch (channels) {
            case 1:
                format = GL_RED;
                internalFormat = GL_RED;
                break;
            case 2:
                format = GL_RG;
                internalFormat = GL_RG;
            case 3:
                format = GL_RGB;
                if (gamma) {
                    internalFormat = GL_SRGB;
                } 
                else {
                    internalFormat = GL_RGB;
                }
                break;
            case 4:
                format = GL_RGBA;
                if (gamma) {
                    internalFormat = GL_SRGB_ALPHA;
                }
                else {
                    internalFormat = GL_RGBA;
                }
                break;
            default:
                format = GL_RGB;
                internalFormat = GL_RGB;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cout << "ERROR: Failed to load texture: " << path << "\n";

        stbi_image_free(data);
    }

    return textureID;
}
#endif