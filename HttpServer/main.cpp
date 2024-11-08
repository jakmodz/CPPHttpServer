#include <iostream>
#include "HttpServer.h"
using namespace std;

int main(int argc, char** argv)
{
	HttpServer("127.0.0.1", 8080).StartServer();
	return 0;
}
