#include "File.h"

#include <stdexcept>
#include <fstream>
#include <iostream>

namespace SimEngine
{
    std::string File::ReadFile(const std::string& filePath)
    {
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);
        if (!file.is_open())
        {
            throw std::runtime_error("Can't open file " + filePath);
        }

        size_t fileSize = file.tellg();
        std::string buffer(fileSize, '\0');

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        return buffer;
    }
}
