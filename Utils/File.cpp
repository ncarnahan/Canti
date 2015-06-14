#include "File.h"
#include <cstdio>

namespace Utils
{
    namespace File
    {
        std::string ReadAllText(const char* fileName)
        {
            FILE* f;
            fopen_s(&f, fileName, "rb");

            // Determine file size
            fseek(f, 0, SEEK_END);
            size_t size = ftell(f);

            std::string text;
            text.resize(size + 1);

            rewind(f);
            fread(&text[0], sizeof(char), size, f);
            text[size] = '\0';
            fclose(f);

            return text;
        }
    }
}
