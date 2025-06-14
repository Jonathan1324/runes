#include "string.hpp"

std::string parseString(const std::string& s)
{
    size_t start = s.find('"');
    if (start == std::string::npos)
        return "";

    start++;
    std::string result;
    for (size_t i = start; i < s.size(); i++)
    {
        if (s[i] == '\\')
        {
            i++;
            if (i >= s.size()) break;

            switch (s[i])
            {
                case 'n':
                    result += '\n';
                    break;
                case '\\':
                case '"':
                default:
                    result += s[i];
                    break;
            }
            continue;
        }
        if (s[i] == '"')
            break;
        result += s[i];
    }

    return result;
}

std::vector<std::string> split(std::string str, char delimiter)
{
  // Using str in a string stream
    std::stringstream ss(str);
    std::vector<std::string> res;
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        res.push_back(token);
    }
    return res;
}