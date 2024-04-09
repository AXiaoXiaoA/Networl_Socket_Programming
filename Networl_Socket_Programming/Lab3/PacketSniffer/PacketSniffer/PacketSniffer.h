#pragma once

#include <WinSock2.h>

/// <summary>
/// 作者：ChangeeShaw
/// 文件功能：定义了PacketSniffer类，用于捕获指定IP地址的数据包并处理
/// </summary>

/// <summary>
/// PacketSniffer类：用于捕获指定IP地址的数据包并处理
/// </summary>
class PacketSniffer {
public:
    /// <summary>
    /// 构造函数：初始化PacketSniffer对象并指定要捕获数据包的IP地址。
    /// </summary>
    /// <param name="ipAddress">要捕获数据包的目标IP地址</param>
    PacketSniffer(const char* ipAddress);
    
    /// <summary>
    /// 析构函数：关闭Socket并释放资源。
    /// </summary>
    ~PacketSniffer();

    /// <summary>
    /// 启动数据包捕获和处理。
    /// </summary>
    void Start();

private:
    /// <summary>
    /// 处理接收到的数据包。
    /// </summary>
    /// <param name="buffer">接收到的数据包缓冲区</param>
    /// <param name="bytesRecv">接收到的字节数</param>
    void ProcessPacket(char* buffer, int bytesRecv);

private:
    SOCKET sock; // Socket对象用于数据包捕获
};
