#include "Parse.h"
#include <sstream>

namespace Utils
{
    namespace Parse
    {
        //Splits a string into tokens
        std::vector<std::string> Split(std::string& input, char delim)
        {
            std::vector<std::string> output;
            std::stringstream stream(input);
            std::string item;
            while (std::getline(stream, item, delim))
            {
                output.push_back(item);
            }
            return output;
        }

        int ToInt(std::string& input)
        {
            return atoi(input.c_str());
        }

        float ToFloat(std::string& input)
        {
            return (float)atof(input.c_str());
        }
    }
}
