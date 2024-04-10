/**
 * @file
 * @brief 定义了 PacketSniffer 类，用于捕获指定IP地址的数据包并处理。
 * @author ChangeeShaw
 * @version 1.0
 * @date 2024.04.01
 */

#pragma once

#include <WinSock2.h>

 /**
  * @class PacketSniffer
  * @brief PacketSniffer 类，用于捕获指定IP地址的数据包并处理。
  */
class PacketSniffer {
public:
	/**
	 * @brief 构造函数，初始化 PacketSniffer 对象并指定要捕获数据包的IP地址。
	 * @param ipAddress 要捕获数据包的目标IP地址。
	 */
	PacketSniffer(const char* ipAddress);
	/**
	 * @brief 析构函数，关闭Socket并释放资源。
	 */
	~PacketSniffer();

	/**
	 * @brief 启动数据包捕获和处理。
	 */
	void Start();

private:
	/**
	 * @brief 处理接收到的数据包。
	 * @param buffer 接收到的数据包缓冲区。
	 * @param bytesRecv 接收到的字节数。
	 */
	void ProcessPacket(char* buffer, int bytesRecv);

private:
	SOCKET sock; ///< Socket对象用于数据包捕获
};
