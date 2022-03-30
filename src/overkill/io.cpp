#include "io.hpp"

namespace OK::IO
{
    
    bool fileToString(const std::string& filePath, std::string* outString)
    {
        std::ifstream infile(filePath);

        if (!infile) {
            return false;
        }
        infile.seekg(0, std::ios::end);
        outString->resize(infile.tellg());
        infile.seekg(0, std::ios::beg);
        infile.read(outString->data(), outString->size());
        infile.close();
        return true;
    }

    namespace Shader 
    {

    ShaderFiles enlistShaderFiles(const char * root)
    {
        ShaderFiles files;
        for (const auto & entry : std::filesystem::directory_iterator(root))
        {
            auto& path = entry.path();
            if (path.has_filename() && path.has_extension())
            {
                auto ext = path.extension();

                if (ext == ".vert")
                    files.vertex_shaders.emplace(path.stem().string(), path.string());
                else if (ext == ".frag")
                    files.fragment_shaders.emplace(path.stem().string(), path.string());
                else if (ext == ".geom")
                    files.geometry_shaders.emplace(path.stem().string(), path.string());
                else if (ext == ".tesc")
                    files.tess_ctrl_shaders.emplace(path.stem().string(), path.string());
                else if (ext == ".tese")
                    files.tess_eval_shaders.emplace(path.stem().string(), path.string());
                else if (ext == ".comp")
                    files.compute_shaders.emplace(path.stem().string(), path.string());
            }
        }
        return files;
    }

    }

}
