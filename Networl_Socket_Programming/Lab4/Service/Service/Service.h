#pragma once

#include <WinSock2.h>
#include <vector>
#include <string>

#pragma comment (lib,"ws2_32.lib")

/// <summary>
/// 作者：ChangeeShaw
/// 文件功能：定义了Service类，表示一个能够并发处理多个客户端连接的TCP服务器
/// </summary>

/// <summary>
/// 服务器类：能够接受客户端连接并并发处理多个客户端消息的TCP服务器
/// </summary>
class Service {
public:
    /// <summary>
    /// 构造函数：初始化服务器并准备开始监听指定端口上的客户端连接
    /// </summary>
    /// <param name="port">服务器监听的端口号</param>
    Service(int port);

    /// <summary>
    /// 析构函数：关闭服务器并清理资源
    /// </summary>
    ~Service();

    /// <summary>
    /// 启动服务器，开始接受客户端连接并处理消息
    /// </summary>
    void Start();

private:
    /// <summary>
    /// 循环接受客户端连接，并使用多线程处理每个连接
    /// </summary>
    void AcceptClient();

    /// <summary>
    /// 处理单个已连接客户端的消息，运行在独立的线程中
    /// </summary>
    /// <param name="clientSocket">进行通信的客户端套接字</param>
    void HandleClient(SOCKET clientSocket);

    /// <summary>
    /// 向指定客户端套接字发送当前系统时间
    /// </summary>
    /// <param name="clientSocket">进行通信的客户端套接字</param>
    void SendTime(SOCKET clientSocket);

    /// <summary>
    /// 向指定客户端套接字发送指定文件的内容
    /// </summary>
    /// <param name="clientSocket">进行通信的客户端套接字</param>
    /// <param name="filename">要发送的文件名</param>
    void SendFile(SOCKET clientSocket, const std::string& filename);

private:
    SOCKET serverSocket; // 服务器套接字
    std::vector<SOCKET> clientSockets;  // 已连接的客户端套接字列表
};
