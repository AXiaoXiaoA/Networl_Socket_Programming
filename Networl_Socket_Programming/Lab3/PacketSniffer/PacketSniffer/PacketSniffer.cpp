#include "PacketSniffer.h"

#include "headers.h"

#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1)

PacketSniffer::PacketSniffer(const char* ipAddress) : sock(INVALID_SOCKET) {
    // 初始化 Winsock
    WSADATA wsd;
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0) {
        std::cerr << "WSAStartup失败：" << WSAGetLastError() << std::endl;
        throw std::runtime_error("WSAStartup失败！");
    }

    // 创建原始套接字
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "原始套接字创建失败：" << WSAGetLastError() << std::endl;
        WSACleanup();
        throw std::runtime_error("原始套接字创建失败！");
    }

    // 启用IP_HDRINCL选项以包含接收的数据包中的IP头部
    BOOL optVal = TRUE;
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char*)&optVal, sizeof(optVal)) == SOCKET_ERROR) {
        std::cerr << "原始套接字选项设置失败：" << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        throw std::runtime_error("原始套接字选项设置失败！");
    }

    // 将原始套接字绑定到指定的IP地址
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(0);
    if (inet_pton(AF_INET, ipAddress, &addr.sin_addr) <= 0) {
        std::cerr << "IP地址获取失败：" << ipAddress << std::endl;
        closesocket(sock);
        WSACleanup();
        throw std::runtime_error("IP地址获取失败！");
    }
    if (bind(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "原始套接字绑定失败：" << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        throw std::runtime_error("原始套接字绑定失败！");
    }

    // 将网卡设置为混听模式，接收所有数据
    DWORD dwBufferInLen = 1;
    if (ioctlsocket(sock, SIO_RCVALL, &dwBufferInLen) == SOCKET_ERROR) {
        std::cerr << "设置为混听模式失败：" << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        throw std::runtime_error("设置为混听模式失败！");
    }
}

PacketSniffer::~PacketSniffer() {
    closesocket(sock);
    WSACleanup();
}

void PacketSniffer::Start() {
    char buffer[65535];
    int bytesRecv;
    SOCKADDR_IN from;
    int fromSize = sizeof(from);

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        // 接收数据包
        bytesRecv = recvfrom(sock, buffer, sizeof(buffer), 0, (SOCKADDR*)&from, &fromSize);
        if (bytesRecv == SOCKET_ERROR) {
            std::cerr << "接收数据包失败: " << WSAGetLastError() << std::endl;
            continue;
        }

        ProcessPacket(buffer, bytesRecv);
    }
}

void PacketSniffer::ProcessPacket(char* buffer, int bytesRecv) {
    IPHeader* ipHeader = (IPHeader*)buffer;
    if (bytesRecv < sizeof(IPHeader)) {
        std::cerr << "接收到的数据包长度不足以包含IP头部" << std::endl;
        std::cout << std::endl;
        return;
    }

    if (ipHeader->protocol == IPPROTO_TCP) {
        // 计算IP头部长度
        int ipHeaderLen = (ipHeader->headLen & 0x0F) * 4;
        TCPHeader* tcpHeader = (TCPHeader*)(buffer + ipHeaderLen);

        if (bytesRecv < ipHeaderLen + sizeof(TCPHeader)) {
            std::cerr << "接收到的数据包长度不足以包含TCP头部" << std::endl;
            std::cout << std::endl;
            return;
        }

        // 打印应用层数据
        std::cout << "Applications++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
        std::cout << "Data: ";
        char* tcpData = buffer + 5 + 4 * ((tcpHeader->headLen & 0xf0) >> 4);
        int tcpDataSize = bytesRecv - 5 - 4 * ((tcpHeader->headLen & 0xf0) >> 4);
        for (int i = 0; i < tcpDataSize; i++) {
            if (tcpData[i] >= 32 && tcpData[i] < 127) {
                std::cout << tcpData[i];
            }
            else {
                std::cout << ".";
            }
        }
        std::cout << std::endl;

        // 打印传输层信息
        std::cout << "Transportation++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
        std::cout << "Src Port: " << ntohs(tcpHeader->sourcePort) << std::endl;
        std::cout << "Dest Port: " << ntohs(tcpHeader->destinPort) << std::endl;
        
        // 打印网络层信息
        std::cout << "Network+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
        std::cout << "Src IP: " << inet_ntop(AF_INET, &(ipHeader->sourceAddr), buffer, INET_ADDRSTRLEN) << std::endl;
        std::cout << "Dest IP: " << inet_ntop(AF_INET, &(ipHeader->destinAddr), buffer, INET_ADDRSTRLEN) << std::endl;
        std::cout << std::endl;
    }
}
