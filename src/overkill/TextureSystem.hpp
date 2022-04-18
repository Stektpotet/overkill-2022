#pragma once
#include <glad/glad.h>

#include <vector>
#include <string>
#include <unordered_map>

#include <overkill/graphics_internal/Texture.hpp>

namespace OK
{

    class TextureSystem
    {
    private:
        //static std::unordered_map<std::string, Texture2D> textures;
        //static std::unordered_map<std::string, Texture2DArray> arrayTextures;
        static std::unordered_map<std::string, Texture2D> textures;
        static void push2D(const char* tag, const std::string& filepath);

    public:
        static constexpr const char DEFAULT[] = "__DEFAULT__";
        static constexpr const char WHITE[] = "__WHITE__";
        // <summary> Load all texture data onto GPU memory. 
        //           Keep model metadata on CPU </summary>
        static void load(const char* directory);
        static void clean();
        static auto push2D(const char* tag, Texture2D texture) -> bool;
        static auto get2D(const char* textureTag) -> const Texture2D;
        static auto makeTexture2D(const std::string& filepath, Texture2D& outTexture) -> int;

        //static auto reserveTexture2D(const GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type)->TextureBase*;
    };

}
