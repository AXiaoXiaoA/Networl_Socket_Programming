#include "Service.h"

#include <WS2tcpip.h>
#include <iostream>
#include <thread>

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
	char inMSG[CLIENT_MSG_SIZE];
	char outMSG[CLIENT_MSG_SIZE];

	while (true) {
		// 接收客户端消息
		int recvResult = recv(clientSocket, inMSG, CLIENT_MSG_SIZE, 0);
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
		std::cout << "客户端（套接字：" << clientSocket << "）命令：" << inMSG << std::endl;

		// 处理客户端命令
		if (strcmp(inMSG, "当前时间") == 0) {
			// 获取系统当前时间
			SYSTEMTIME systime = { 0 };
			GetLocalTime(&systime);
			sprintf_s(outMSG, CLIENT_MSG_SIZE, "%d-%02d-%02d %02d:%02d:%02d",
				systime.wYear, systime.wMonth, systime.wDay,
				systime.wHour, systime.wMinute, systime.wSecond);

			// 发送时间信息给客户端
			int sendResult = send(clientSocket, outMSG, strlen(outMSG), 0);
			if (sendResult == SOCKET_ERROR) {
				std::cerr << "发送数据给客户端（套接字：" << clientSocket << "）出错，错误代码：" << WSAGetLastError() << std::endl;
			}
		}
		else if (strcmp(inMSG, "退出") == 0) {
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
