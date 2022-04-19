#include <overkill/TextureSystem.hpp>
#include <iostream>
#include <filesystem>
#include <assert.h>
#include <stb_image.h>

namespace OK
{
    std::unordered_map<std::string, Texture2D> TextureSystem::textures;
    std::unordered_map<std::string, Texture3D> TextureSystem::textures3D;

    auto TextureSystem::get2D(const char* tag) -> const Texture2D
    {
        auto& found = textures.find(tag);
        if (found != textures.end())
            return found->second;

        std::cout << "WARN: [TextureSystem] unable to find \"" << tag << "\", loading default instead..." << std::endl;
        return TextureSystem::textures[DEFAULT];
    }

    void TextureSystem::push2D(const char* tag, const std::string& filepath)
    {
        std::cout << "INFO:\tTexture \"" << tag << "\" from file : " << filepath.data() << std::endl;

        Texture2D texture;
        auto err = TextureSystem::makeTexture2D(filepath, texture);
        if (err) {
            std::cout << "WARN:\tTextureSystem::makeTexture2D\n\t\t\tAttempted making texture out of \"" << filepath.data() << "\"..." << std::endl;
            return;
        }
        TextureSystem::textures.emplace(tag, texture);
    }

    void TextureSystem::load(const char* directory)
    {
        bool found_directory = std::filesystem::exists(directory);
        if (found_directory)
        {
            for (const auto& entry : std::filesystem::directory_iterator(directory))
            {
                auto path = entry.path();
                if (!path.has_stem())
                    continue;
                TextureSystem::push2D(path.stem().string().data(), path.string());
            }
        }
        else
        {
            std::cout << "WARN: [TextureSystem] Was unable to load textures from " << directory << std::endl;
        }
        // TODO: Load these from the file system somehow
        std::uint8_t white_pixels[64];
        std::fill_n(white_pixels, 64, 255u);
        Texture2D white_tex(RawTexture(4u, 4u, 4u, white_pixels));
        TextureSystem::push2D(WHITE, white_tex);
        TextureSystem::push2D(DEFAULT, white_tex);
    }

    void TextureSystem::clean()
    {
        for (auto& [_, texture] : textures)
        {
            texture.clean();
        }
        textures.clear();
    }

    auto TextureSystem::push2D(const char* tag, Texture2D texture) -> bool {
        auto& found = textures.find(tag);
        if (found != textures.end())
        {
            if (found->second.getID() == texture.getID())
                return false;
            found->second.clean(); 
        }
        TextureSystem::textures.emplace(tag, texture);
        std::cout << "INFO:\tTexture \"" << tag << "\" from memory" << std::endl;
        return true;
    }

    auto TextureSystem::makeTexture2D(const std::string& filepath, Texture2D& outTexture) -> int
    {
        stbi_set_flip_vertically_on_load(true);
        int width, height, comp;
        unsigned char* rawPixels = stbi_load(filepath.c_str(), &width, &height, &comp, STBI_rgb_alpha);

        OK::RawTexture raw(width, height, 4u, rawPixels);
        stbi_image_free(rawPixels);

        auto texture = Texture2D(raw);

        GLenum err = glGetError();
        if (err) {
            std::cout << "WARN:\tglTexImage2D: Failed to buffer texture to GPU" << std::endl;
            texture.clean();
            return 1;
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        outTexture = texture;
        return 0;
    }

    auto TextureSystem::push3D(const char* tag, Texture3D texture) -> bool {
        auto& found = textures3D.find(tag);
        if (found != textures3D.end())
        {
            if (found->second.getID() == texture.getID())
                return false;
            found->second.clean();
        }
        TextureSystem::textures3D.emplace(tag, texture);
        std::cout << "INFO:\tTexture \"" << tag << "\" from memory" << std::endl;
        return true;
    }

    auto TextureSystem::get3D(const char* tag) -> const Texture3D
    {
        auto& found = textures3D.find(tag);
        if (found != textures3D.end())
            return found->second;

        std::cout << "WARN: [TextureSystem] unable to find \"" << tag << "\", loading default instead..." << std::endl;
        std::cout << "ERR: [TextureSystem] 3D default is not implemented yet!" << std::endl;
        return TextureSystem::textures3D[DEFAULT];
    }
}
