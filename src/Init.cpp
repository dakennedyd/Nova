#include <string>
#include <vector>
#include "Init.h"

namespace Nova {
	void replaceAllInPlace(std::string & string1, const std::string & string2, const std::string & string3)
	{
		size_t pos = 0;
		while ((pos = string1.find(string2, pos)) != std::string::npos) {
			string1.replace(pos, string2.length(), string3);
			pos += string3.length();
		}
	}
	std::vector<std::string> split(const std::string & str, const std::string & delimiter)
	{
		std::vector<std::string> result;
		std::string s(str);
		size_t pos = 0;
		std::string token;
		while ((pos = s.find(delimiter)) != std::string::npos) {
			token = s.substr(0, pos);
			result.push_back(token);
			//LOG_DEBUG(token);
			s.erase(0, pos + delimiter.length());
		}
		result.push_back(s);
		//LOG_DEBUG(s);
		return result;
	}
}