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

}
