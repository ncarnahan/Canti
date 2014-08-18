#ifndef UTILS_FILE_H
#define UTILS_FILE_H

#include <string>
#include <memory>

namespace Utils
{
    namespace File
    {
        std::unique_ptr<std::string> ReadAllText(std::string fileName);
    }
}

#endif
