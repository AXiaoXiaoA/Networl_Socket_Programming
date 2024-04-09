#include "Client.h"

#include <WS2tcpip.h>
#include <iostream>

Client::Client(const char* serverIP, int serverPort) {
    // 初始化 Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("Winsock初始化失败！");
    }

    // 创建客户端套接字
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        throw std::runtime_error("客户端套接字创建失败！");
    }

    // 设置服务器地址信息
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIP, &serverAddr.sin_addr);
}

Client::~Client() {
    // 关闭服务器套接字并清理 Winsock
    closesocket(clientSocket);
    WSACleanup();
}

bool Client::Connect() {
    // 尝试连接服务器
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
        std::cerr << "连接服务器失败！" << std::endl;
        return false;
    }
    else {
        std::cout << "连接服务器成功！" << std::endl;
        return true;
    }
}

void Client::Run() {
    const int BUFFER_SIZE = 128;
    char buffer[BUFFER_SIZE];

    while (true) {
        std::cout << "请输入命令：";
        std::cin >> buffer;

        // 发送命令到服务器
        int sendResult = send(clientSocket, buffer, sizeof(buffer), 0);
        if (sendResult == SOCKET_ERROR) {
            std::cerr << "发送数据给服务器端出错，错误代码：" << WSAGetLastError() << std::endl;
        }

        // 如果输入命令是退出，则退出循环
        if (strcmp(buffer, "退出") == 0) {
            break;
        }

        // 接收服务器响应
        memset(buffer, 0, sizeof(buffer));
        int recvResult = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (recvResult == SOCKET_ERROR) {
            std::cerr << "服务器断开连接，对话中断" << std::endl;
            closesocket(clientSocket);
            return;
        }
        std::cout << "服务器端应答：" << buffer << std::endl;
    }
}
