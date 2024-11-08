#include "HttpServer.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <thread>
#include <cstdlib>
#include "RequestParser.h"
const int BUFFER_SIZE = 4096;
HttpServer::HttpServer(const std::string& Host, int Port):Host(Host),Port(Port),Socket(),WsaData(),ClientSocket()
{
	SocketAddr.sin_family = AF_INET;
	SocketAddr.sin_port = htons(Port);
	SocketAddr.sin_addr.s_addr = inet_addr(Host.c_str());
}

HttpServer::~HttpServer()
{
	closesocket(Socket);
	WSACleanup();
}

void HttpServer::StartServer()
{
	
	if (WSAStartup(MAKEWORD(2, 0), &WsaData) != 0)
	{
		std::cout << "Wsastartup fail" << std::endl;
		return;
	}
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (bind(Socket,(sockaddr*)&SocketAddr,sizeof(SocketAddr))  == SOCKET_ERROR)
	{
		std::cout << "Bind failed: " << WSAGetLastError() << std::endl;
		closesocket(Socket);
		WSACleanup();
		return;
	}
	if (listen(Socket, SOMAXCONN) == SOCKET_ERROR)
	{
		std::cout << "Listen failed: " << WSAGetLastError() << std::endl;
		closesocket(Socket);
		WSACleanup();
		return;
	}
	std::cout << "Server is listeting on " << Host << ":" << Port<<std::endl;
	Listen();
}

std::string HttpServer::ContentGet(const std::string& Path)
{
	std::string content;
	std::fstream file("." +Path);
	content.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	return content;
}

std::string HttpServer::ContentPost(const std::string& Path)
{
	if (Path == "Random")
	{
		return std::to_string(rand() % 10000);
	}
	return std::string();
}

std::string HttpServer::GetResponse(const std::string& Request)
{
	std::string Method = RequestParser::GetMethod(Request);
	std::string Path = RequestParser::GetPath(Request);
	std::ostringstream response;
	if (Method == "GET")
	{
		std::string content = ContentGet(Path);
		if (content.empty())
		{
			response << "HTTP/1.1 404 Not Found\r\n"
				<< "Content-Type: text/html\r\n"
				<< "Content-Length: 24\r\n"
				<< "\r\n"
				<< "404 Not Found";
		}
		else
		{
			response << "HTTP/1.1 200 OK\r\n"
				<< "Content-Type: text/html\r\n"
				<< "Content-Length: " << content.size() << "\r\n"
				<< "\r\n"
				<< content;
		}
		
	}
	else if (Method == "POST")
	{
		std::string content = ContentPost(Path);
		response << "HTTP/1.1 200 OK\r\n"
			<< "\r\n"
			<< content;
		
	}
	else
	{
		response << "HTTP/1.1 404 Not Found\r\n"
			<< "Content-Type: text/html\r\n"
			<< "Content-Length: 24\r\n"
			<< "\r\n"
			<< "404 Not Found";
	}
	return response.str();
}

void HttpServer::SendResposne(SOCKET& ClientSocket, const std::string& Response)
{
	send(ClientSocket, Response.c_str(), Response.size(), 0);
}

void HttpServer::Listen()
{
	while (true)
	{
		int size = sizeof(sockaddr);
		ClientSocket = accept(Socket, (sockaddr*)&SocketAddr, &size);

		if (ClientSocket == INVALID_SOCKET)
		{
			closesocket(ClientSocket);
			return;
		}
		std::thread thread(&HttpServer::OnConnection, this, ClientSocket); 
		thread.detach();
	}
}

void HttpServer::OnConnection(SOCKET ClientSocket)
{
	
	char Buffer[BUFFER_SIZE] = { 0 };
	int BytesReceived = recv(ClientSocket, Buffer, BUFFER_SIZE, 0);
	if (BytesReceived == SOCKET_ERROR)
	{
		std::cerr << "Receive failed: " << WSAGetLastError() << std::endl;
		closesocket(ClientSocket);
		return;
	}
	std::string Request(Buffer, BytesReceived);
	std::cout << "Received request:\n" << Request << "\n";
	std::string Response = GetResponse(Request);
	SendResposne(ClientSocket, Response);
	
	closesocket(ClientSocket);
}
