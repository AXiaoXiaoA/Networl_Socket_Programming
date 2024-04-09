#include "Client.h"

#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <fstream>

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
        std::cin.getline(buffer, BUFFER_SIZE);

        // 发送命令到服务器
        int sendResult = send(clientSocket, buffer, strlen(buffer) + 1, 0);
        if (sendResult == SOCKET_ERROR) {
            std::cerr << "发送数据给服务器端出错，错误代码：" << WSAGetLastError() << std::endl;
            break;
        }

        // 如果输入命令是退出，则退出循环
        if (strcmp(buffer, "退出") == 0) {
            break;
        }

        // 接收服务器响应
        if (strcmp(buffer, "当前时间") == 0) {
            ReceiveTime();
        }
        else if (strncmp(buffer, "文件传输：", 10) == 0) {
            ReceiveFile();
        } 
        else {
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
}

void Client::ReceiveTime() {
    const int BUFFER_SIZE = 128;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    // 接收服务器端发送的时间信息
    int recvResult = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (recvResult == SOCKET_ERROR) {
        std::cerr << "接收服务器时间信息出错，错误代码：" << WSAGetLastError() << std::endl;
        return;
    }

    if (recvResult == 0) {
        std::cerr << "服务器断开连接，无法获取时间信息" << std::endl;
        return;
    }

    // 显示服务器返回的时间
    std::cout << "服务器当前时间：" << buffer << std::endl;
}

void Client::ReceiveFile() {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    // 检查文件是否存在
    int recvResult = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (recvResult == SOCKET_ERROR || recvResult == 0) {
        std::cerr << "接收文件数据出错或连接关闭，错误代码：" << WSAGetLastError() << std::endl;
        return;
    }
    if (strcmp(buffer, "文件不存在") == 0) {
        std::cerr << "指定文件不存在" << std::endl;
        return;
    }

    std::string filename;
    std::cout << "请输入文件保存位置：";
    std::getline(std::cin, filename);

    // 打开本地文件用于接收数据
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "无法打开文件以保存接收的数据：" << filename << std::endl;
        return;
    }

    // 循环接收数据直到接收完整文件
    while (true) {
        // 写入当前接收到的数据块
        outFile.write(buffer, recvResult);

        // 判断是否接收完整文件，根据实际情况进行处理
        if (recvResult < BUFFER_SIZE) {
            break;
        }

        // 接收数据块
        recvResult = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (recvResult == SOCKET_ERROR || recvResult == 0) {
            std::cerr << "接收文件数据出错或连接关闭，错误代码：" << WSAGetLastError() << std::endl;
            outFile.close();
            remove(filename.c_str());
            return;
        }
    }

    // 检查文件接收过程中是否出现错误
    if (!outFile.good()) {
        std::cerr << "写入文件数据出错" << std::endl;
        outFile.close();
        remove(filename.c_str());
        return;
    }

    // 关闭本地文件
    outFile.close();
    std::cout << "文件接收完成：" << filename << std::endl;
}
