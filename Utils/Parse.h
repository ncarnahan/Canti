#ifndef UTILS_PARSE_H
#define UTILS_PARSE_H

#include <vector>

namespace Utils
{
    namespace Parse
    {
        //Splits a string into tokens
        std::vector<std::string> Split(std::string& input, char delim = ' ');

        int ToInt(std::string& input);
        float ToFloat(std::string& input);
    }
}

#endif
