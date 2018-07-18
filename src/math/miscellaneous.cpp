#include "miscellaneous.h"
#include "Constants.h"
#include <cmath>
#include <string>
#include <vector>

namespace Nova
{
float toDegrees(const float value) { return value * (180.0f / NOVA_PI); }
float toRadians(const float value) { return value * (NOVA_PI / 180.0f); }
bool isEqual(float valueA, float valueB)
{
    // if ((abs(valueA) - abs(valueB)) <= NOVA_LAMBDA)
    if ((std::abs(valueA - valueB)) <= NOVA_LAMBDA)
    {
        return true;
    }
    return false;
}

void replaceAllInPlace(std::string &string1, const std::string &string2, const std::string &string3)
{
    size_t pos = 0;
    while ((pos = string1.find(string2, pos)) != std::string::npos)
    {
        string1.replace(pos, string2.length(), string3);
        pos += string3.length();
    }
}
std::vector<std::string> split(const std::string &str, const std::string &delimiter)
{
    std::vector<std::string> result;
    std::string s(str);
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);
        result.push_back(token);
        // LOG_DEBUG(token);
        s.erase(0, pos + delimiter.length());
    }
    result.push_back(s);
    // LOG_DEBUG(s);
    return result;
}
} // namespace Nova