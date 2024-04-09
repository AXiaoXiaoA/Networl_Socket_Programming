#include "Client.h"

#include <WS2tcpip.h>
#include <iostream>

Client::Client(const char* serverIP, int serverPort) {
    // ��ʼ�� Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("Winsock��ʼ��ʧ�ܣ�");
    }

    // �����ͻ����׽���
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        throw std::runtime_error("�ͻ����׽��ִ���ʧ�ܣ�");
    }

    // ���÷�������ַ��Ϣ
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIP, &serverAddr.sin_addr);
}

Client::~Client() {
    // �رշ������׽��ֲ����� Winsock
    closesocket(clientSocket);
    WSACleanup();
}

bool Client::Connect() {
    // �������ӷ�����
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
        std::cerr << "���ӷ�����ʧ�ܣ�" << std::endl;
        return false;
    }
    else {
        std::cout << "���ӷ������ɹ���" << std::endl;
        return true;
    }
}

void Client::Run() {
    const int BUFFER_SIZE = 128;
    char buffer[BUFFER_SIZE];

    while (true) {
        std::cout << "���������";
        std::cin >> buffer;

        // �������������
        int sendResult = send(clientSocket, buffer, sizeof(buffer), 0);
        if (sendResult == SOCKET_ERROR) {
            std::cerr << "�������ݸ��������˳���������룺" << WSAGetLastError() << std::endl;
        }

        // ��������������˳������˳�ѭ��
        if (strcmp(buffer, "�˳�") == 0) {
            break;
        }

        // ���շ�������Ӧ
        memset(buffer, 0, sizeof(buffer));
        int recvResult = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (recvResult == SOCKET_ERROR) {
            std::cerr << "�������Ͽ����ӣ��Ի��ж�" << std::endl;
            closesocket(clientSocket);
            return;
        }
        std::cout << "��������Ӧ��" << buffer << std::endl;
    }
}
