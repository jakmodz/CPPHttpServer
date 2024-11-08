#pragma once
#include <string>
#include <vector>
class RequestParser
{
public:
	static std::string GetMethod(const std::string& Request);
	static std::string GetPath(const std::string& Request);
	static std::string GetVersion(const std::string& Request);
private:
	static std::vector<std::string>GetData(const std::string& Request);
};

