#include "Service.h"

#include <WS2tcpip.h>
#include <iostream>
#include <thread>
#include <fstream>

Service::Service(int port) {
	// 初始化 Winsock
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0) {
		std::cerr << "Winsock初始化失败：" << WSAGetLastError() << std::endl;
		throw std::runtime_error("Winsock初始化失败！");
	}

	// 创建服务器套接字
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		std::cerr << "服务器套接字创建失败：" << WSAGetLastError() << std::endl;
		WSACleanup();
		throw std::runtime_error("服务器套接字创建失败！");
	}

	// 设置服务器地址和端口
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 绑定服务器套接字到指定地址和端口
	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cerr << "服务器套接字绑定失败：" << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		throw std::runtime_error("服务器套接字绑定失败！");
	}

	// 监听客户端连接请求
	if (listen(serverSocket, 2) == SOCKET_ERROR) {
		closesocket(serverSocket);
		WSACleanup();
		throw std::runtime_error("服务器监听客户端连接请求失败！");
	}

	// 初始化客户端套接字列表
	clientSockets.resize(0);
}

Service::~Service() {
	// 关闭服务器套接字并清理 Winsock
	closesocket(serverSocket);
	for (auto clientSocket : clientSockets) {
		closesocket(clientSocket);
	}
	WSACleanup();
}

void Service::Start() {
	std::cout << "等待客户端连接..." << std::endl;
	while (true) {
		AcceptClient();
	}
}

void Service::AcceptClient() {
	SOCKADDR_IN clientAddr;
	int addrSize = sizeof(clientAddr);

	// 接受客户端连接，返回客户端套接字
	SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize);

	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "接受客户端连接失败，错误代码：" << WSAGetLastError() << std::endl;
		return;
	}
	else {
		// 获取客户端 IP 和端口信息
		char ipBuffer[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(clientAddr.sin_addr), ipBuffer, INET_ADDRSTRLEN);
		std::cout << "客户端（" << ipBuffer << "）通过端口（" << ntohs(clientAddr.sin_port) << "）连接成功，套接字号：" << clientSocket << std::endl;
		clientSockets.push_back(clientSocket);

		// 创建新线程处理客户端消息
		std::thread clientThread(&Service::HandleClient, this, clientSocket);
		clientThread.detach();
	}
}

void Service::HandleClient(SOCKET clientSocket) {
	int const CLIENT_MSG_SIZE = 128;
	char buffer[CLIENT_MSG_SIZE];

	while (true) {
		// 接收客户端消息
		int recvResult = recv(clientSocket, buffer, CLIENT_MSG_SIZE, 0);
		if (recvResult == SOCKET_ERROR || recvResult == 0) {
			std::cerr << "客户端（套接字：" << clientSocket << "）退出连接，对话中断" << std::endl;
			closesocket(clientSocket);
			// 从存储的客户端套接字列表中移除已断开连接的套接字
			auto it = std::find(clientSockets.begin(), clientSockets.end(), clientSocket);
			if (it != clientSockets.end()) {
				clientSockets.erase(it);
			}
			break;
		}
		std::cout << "客户端（套接字：" << clientSocket << "）命令：" << buffer << std::endl;

		// 处理客户端命令
		if (strcmp(buffer, "当前时间") == 0) {
			SendTime(clientSocket);
		}
		else if (strncmp(buffer, "文件传输：", 10) == 0) {
			SendFile(clientSocket, buffer + 10);
		}
		else if (strcmp(buffer, "退出") == 0) {
			std::cout << "客户端（套接字：" << clientSocket << "）成功退出连接" << std::endl;
			closesocket(clientSocket);
			// 从存储的客户端套接字列表中移除已断开连接的套接字
			auto it = std::find(clientSockets.begin(), clientSockets.end(), clientSocket);
			if (it != clientSockets.end()) {
				clientSockets.erase(it);
			}
			break;
		}
		else {
			const char* message = "无效命令";
			int sendResult = send(clientSocket, message, strlen(message), 0);
			if (sendResult == SOCKET_ERROR) {
				std::cerr << "发送数据给客户端（套接字：" << clientSocket << "）出错，错误代码：" << WSAGetLastError() << std::endl;
			}
		}
	}

	if (clientSockets.empty()) {
		std::cout << "等待客户端连接..." << std::endl;
	}
}

void Service::SendTime(SOCKET clientSocket) {
	int const BUFFER_SIZE = 128;
	char buffer[BUFFER_SIZE];
	
	// 获取系统当前时间
	SYSTEMTIME systime = { 0 };
	GetLocalTime(&systime);
	sprintf_s(buffer, BUFFER_SIZE, "%d-%02d-%02d %02d:%02d:%02d",
		systime.wYear, systime.wMonth, systime.wDay,
		systime.wHour, systime.wMinute, systime.wSecond);

	// 发送时间信息给客户端
	int sendResult = send(clientSocket, buffer, strlen(buffer), 0);
	if (sendResult == SOCKET_ERROR) {
		std::cerr << "发送数据给客户端（套接字：" << clientSocket << "）出错，错误代码：" << WSAGetLastError() << std::endl;
	}
}

void Service::SendFile(SOCKET clientSocket, const std::string& filename) {
	const int BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];

	std::ifstream inFile(filename, std::ios::binary);

	// 如果文件无法打开，向客户端发送文件不存在消息
	if (!inFile.is_open()) {
		const char* fileNotFoundMsg = "文件不存在";
		int msgLength = static_cast<int>(strlen(fileNotFoundMsg)) + 1;
		int sendResult = send(clientSocket, fileNotFoundMsg, msgLength, 0);

		if (sendResult == SOCKET_ERROR) {
			std::cerr << "向客户端（套接字：" << clientSocket << "）发送消息失败，错误代码：" << WSAGetLastError() << std::endl;
		}
		else {
			std::cerr << "客户端（套接字：" << clientSocket << "）指定文件不存在：" << filename << std::endl;
		}
		return;
	}

	// 循环读取文件数据并发送给客户端
	memset(buffer, 0, sizeof(buffer));
	while (!inFile.eof()) {
		inFile.read(buffer, BUFFER_SIZE);
		int bytesRead = static_cast<int>(inFile.gcount());
		if (bytesRead > 0) {
			int sendResult = send(clientSocket, buffer, bytesRead, 0);
			if (sendResult == SOCKET_ERROR) {
				std::cerr << "向客户端（套接字：" << clientSocket << "）发送文件数据失败，错误代码：" << WSAGetLastError() << std::endl;
				break;
			}
		}
	}

	inFile.close();

	// 检查文件是否正常发送完成
	if (inFile.eof()) {
		std::cout << "向客户端（套接字：" << clientSocket << "）传输文件完成：" << filename << std::endl;
	}
	else {
		std::cerr << "向客户端（套接字：" << clientSocket << "）传输文件过程中发生错误：" << filename << std::endl;
		remove(filename.c_str());
	}
}
