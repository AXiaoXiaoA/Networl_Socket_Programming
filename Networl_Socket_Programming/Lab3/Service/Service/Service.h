/**
 * @file
 * @brief 定义了 Service 类，表示一个能够并发处理多个客户端连接的 TCP 服务器。
 * @author ChangeeShaw
 * @version 1.0
 * @date 2024.04.01
 */

#pragma once

#include <WinSock2.h>
#include <vector>

#pragma comment (lib,"ws2_32.lib")

 /**
  * @class Service
  * @brief 服务器类，能够接受客户端连接并并发处理多个客户端消息的 TCP 服务器。
  */
class Service {
public:
	/**
	 * @brief 构造函数，初始化服务器并准备开始监听指定端口上的客户端连接。
	 * @param port 服务器监听的端口号。
	 */
	Service(int port);
	/**
	 * @brief 析构函数，关闭服务器并清理资源。
	 */
	~Service();

	/**
	 * @brief 启动服务器，开始接受客户端连接并处理消息。
	 */
	void Start();

private:
	/**
	 * @brief 循环接受客户端连接，并使用多线程处理每个连接。
	 */
	void AcceptClient();

	/**
	 * @brief 处理单个已连接客户端的消息，运行在独立的线程中。
	 * @param clientSocket 进行通信的客户端套接字。
	 */
	void HandleClient(SOCKET clientSocket);

private:
	SOCKET serverSocket; ///< 服务器套接字
	std::vector<SOCKET> clientSockets; ///< 已连接的客户端套接字列表
};
