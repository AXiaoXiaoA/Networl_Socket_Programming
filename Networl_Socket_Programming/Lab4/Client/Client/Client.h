/**
 * @file
 * @brief 定义了 Client 类，表示一个用于与服务器建立连接并进行通信的客户端。
 * @author ChangeeShaw
 * @version 1.0
 * @date 2024.04.01
 */

#pragma once

#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

 /**
  * @class Client
  * @brief 客户端类，用于与服务器建立连接并进行通信的客户端。
  */
class Client {
public:
	/**
	 * @brief 构造函数，初始化客户端对象，并指定要连接的服务器IP和端口号。
	 * @param serverIP 服务器的IP地址。
	 * @param serverPort 服务器的端口号。
	 */
	Client(const char* serverIP, int serverPort);
	/**
	 * @brief 析构函数，关闭客户端并释放资源。
	 */
	~Client();

	/**
	 * @brief 连接服务器。
	 * @return 连接是否成功。
	 */
	bool Connect();
	/**
	 * @brief 运行客户端，连接服务器并开始处理通信。
	 */
	void Run();

private:
	/**
	 * @brief 接收服务器端发送的时间信息并显示在客户端控制台上。
	 */
	void ReceiveTime();
	/**
	 * @brief 从服务器端接收指定文件并保存到本地文件系统。
	 */
	void ReceiveFile();

private:
	SOCKET clientSocket; ///< 客户端套接字标识符
	SOCKADDR_IN serverAddr; ///< 服务器地址信息
};
