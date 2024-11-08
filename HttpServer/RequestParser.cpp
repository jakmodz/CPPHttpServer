#include "RequestParser.h"
#include <fstream>
#include <iostream>
#include <sstream>
std::string RequestParser::GetMethod(const std::string& Request)
{
    return GetData(Request)[0];
}

std::string RequestParser::GetPath(const std::string& Request)
{
    return GetData(Request)[1];
}

std::string RequestParser::GetVersion(const std::string& Request)
{
    return GetData(Request)[2];
}

std::vector<std::string> RequestParser::GetData(const std::string& Request)
{
	std::stringstream stream(Request);
	std::string line;
	std::string path,method,version;
	stream >> method >> path >> version;
	return std::vector<std::string>{method,path,version};
}


