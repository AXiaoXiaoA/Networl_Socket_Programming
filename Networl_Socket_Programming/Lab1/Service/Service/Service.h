#pragma once

#include <WinSock2.h>

#pragma comment (lib,"ws2_32.lib")

/// <summary>
/// 作者：ChangeeShaw
/// 文件功能：定义了Service类，表示一个能够接受客户端连接并处理消息TCP服务器
/// </summary>

/// <summary>
/// 服务器类：能够接受客户端连接并处理消息的TCP服务器
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
    /// 循环接受客户端连接
    /// </summary>
    void AcceptClient();

    /// <summary>
    /// 处理已连接客户端的消息
    /// </summary>
    /// <param name="clientSocket">进行通信的客户端套接字</param>
    void HandleClient(SOCKET clientSocket);

private:
    SOCKET serverSocket; // 服务器套接字
};
