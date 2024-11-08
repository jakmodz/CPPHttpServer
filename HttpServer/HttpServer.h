#pragma once
#include <winsock.h>
#include <string>
#include "HttpRequestHandler.h"
class HttpServer : public HttpRequestHandler
{
public:
	HttpServer(const std::string& Host, int Port);
	~HttpServer();

	void StartServer();
	std::string ContentGet(const std::string& Path) override;
	std::string ContentPost(const std::string& Path) override;
private:
	std::string Host;
	int Port;
	SOCKET Socket;
	sockaddr_in SocketAddr;
	WSADATA WsaData;
	SOCKET ClientSocket;
	std::string GetResponse(const std::string&  Request);
	void SendResposne(SOCKET& ClientSocket, const std::string& Response);
	void Listen();
	void OnConnection(SOCKET ClientSocket);
};

