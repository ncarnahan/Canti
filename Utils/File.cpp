#include "File.h"
#include <cstdio>

namespace Utils
{
    namespace File
    {
        std::unique_ptr<std::string> ReadAllText(std::string fileName)
        {
            FILE* f;
            fopen_s(&f, fileName.c_str(), "rb");

            // Determine file size
            fseek(f, 0, SEEK_END);
            size_t size = ftell(f);

            char* text = new char[size + 1];

            rewind(f);
            fread(text, sizeof(char), size, f);
            text[size] = '\0';
            fclose(f);

            auto ptr = std::unique_ptr<std::string>(new std::string(text, size));
            delete text;

            return ptr;
        }
    }
}
