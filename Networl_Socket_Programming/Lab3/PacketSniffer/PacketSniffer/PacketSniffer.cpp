#include "PacketSniffer.h"

#include "headers.h"

#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define SIO_RCVALL _WSAIOW(IOC_VENDOR,1)

PacketSniffer::PacketSniffer(const char* ipAddress) : sock(INVALID_SOCKET) {
    // ��ʼ�� Winsock
    WSADATA wsd;
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0) {
        std::cerr << "WSAStartupʧ�ܣ�" << WSAGetLastError() << std::endl;
        throw std::runtime_error("WSAStartupʧ�ܣ�");
    }

    // ����ԭʼ�׽���
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_IP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "ԭʼ�׽��ִ���ʧ�ܣ�" << WSAGetLastError() << std::endl;
        WSACleanup();
        throw std::runtime_error("ԭʼ�׽��ִ���ʧ�ܣ�");
    }

    // ����IP_HDRINCLѡ���԰������յ����ݰ��е�IPͷ��
    BOOL optVal = TRUE;
    if (setsockopt(sock, IPPROTO_IP, IP_HDRINCL, (char*)&optVal, sizeof(optVal)) == SOCKET_ERROR) {
        std::cerr << "ԭʼ�׽���ѡ������ʧ�ܣ�" << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        throw std::runtime_error("ԭʼ�׽���ѡ������ʧ�ܣ�");
    }

    // ��ԭʼ�׽��ְ󶨵�ָ����IP��ַ
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(0);
    if (inet_pton(AF_INET, ipAddress, &addr.sin_addr) <= 0) {
        std::cerr << "IP��ַ��ȡʧ�ܣ�" << ipAddress << std::endl;
        closesocket(sock);
        WSACleanup();
        throw std::runtime_error("IP��ַ��ȡʧ�ܣ�");
    }
    if (bind(sock, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "ԭʼ�׽��ְ�ʧ�ܣ�" << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        throw std::runtime_error("ԭʼ�׽��ְ�ʧ�ܣ�");
    }

    // ����������Ϊ����ģʽ��������������
    DWORD dwBufferInLen = 1;
    if (ioctlsocket(sock, SIO_RCVALL, &dwBufferInLen) == SOCKET_ERROR) {
        std::cerr << "����Ϊ����ģʽʧ�ܣ�" << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        throw std::runtime_error("����Ϊ����ģʽʧ�ܣ�");
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
        // �������ݰ�
        bytesRecv = recvfrom(sock, buffer, sizeof(buffer), 0, (SOCKADDR*)&from, &fromSize);
        if (bytesRecv == SOCKET_ERROR) {
            std::cerr << "�������ݰ�ʧ��: " << WSAGetLastError() << std::endl;
            continue;
        }

        ProcessPacket(buffer, bytesRecv);
    }
}

void PacketSniffer::ProcessPacket(char* buffer, int bytesRecv) {
    IPHeader* ipHeader = (IPHeader*)buffer;
    if (bytesRecv < sizeof(IPHeader)) {
        std::cerr << "���յ������ݰ����Ȳ����԰���IPͷ��" << std::endl;
        std::cout << std::endl;
        return;
    }

    if (ipHeader->protocol == IPPROTO_TCP) {
        // ����IPͷ������
        int ipHeaderLen = (ipHeader->headLen & 0x0F) * 4;
        TCPHeader* tcpHeader = (TCPHeader*)(buffer + ipHeaderLen);

        if (bytesRecv < ipHeaderLen + sizeof(TCPHeader)) {
            std::cerr << "���յ������ݰ����Ȳ����԰���TCPͷ��" << std::endl;
            std::cout << std::endl;
            return;
        }

        // ��ӡӦ�ò�����
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

        // ��ӡ�������Ϣ
        std::cout << "Transportation++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
        std::cout << "Src Port: " << ntohs(tcpHeader->sourcePort) << std::endl;
        std::cout << "Dest Port: " << ntohs(tcpHeader->destinPort) << std::endl;
        
        // ��ӡ�������Ϣ
        std::cout << "Network+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
        std::cout << "Src IP: " << inet_ntop(AF_INET, &(ipHeader->sourceAddr), buffer, INET_ADDRSTRLEN) << std::endl;
        std::cout << "Dest IP: " << inet_ntop(AF_INET, &(ipHeader->destinAddr), buffer, INET_ADDRSTRLEN) << std::endl;
        std::cout << std::endl;
    }
}
