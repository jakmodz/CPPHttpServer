#pragma once
#include <string>

class HttpRequestHandler
{
public:
	virtual std::string ContentGet(const std::string& Path) = 0;
	virtual  std::string ContentPost(const std::string& Path) = 0;

private:

};

