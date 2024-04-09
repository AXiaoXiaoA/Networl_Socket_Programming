#pragma once

#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

/// <summary>
/// 作者：ChangeeShaw
/// 文件功能：定义了Client类，表示一个用于与服务器建立连接并进行通信的客户端
/// </summary>

/// <summary>
/// 客户端类：用于与服务器建立连接并进行通信的客户端
/// </summary>
class Client {
public:
    /// <summary>
    /// 构造函数：初始化客户端对象，并指定要连接的服务器IP和端口号
    /// </summary>
    /// <param name="serverIP">服务器的IP地址</param>
    /// <param name="serverPort">服务器的端口号</param>
    Client(const char* serverIP, int serverPort);

    /// <summary>
    /// 析构函数：关闭客户端并释放资源
    /// </summary>
    ~Client(); // 关闭客户端

    /// <summary>
    /// 连接服务器
    /// </summary>
    /// <returns>连接是否成功</returns>
    bool Connect();

    /// <summary>
    /// 运行客户端：连接服务器并开始处理通信
    /// </summary>
    void Run();

private:
    SOCKET clientSocket; // 客户端套接字标识符
    SOCKADDR_IN serverAddr; // 服务器地址信息
};
