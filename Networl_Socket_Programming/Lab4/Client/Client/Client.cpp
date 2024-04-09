#include "Client.h"

#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <fstream>

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
        std::cin.getline(buffer, BUFFER_SIZE);

        // �������������
        int sendResult = send(clientSocket, buffer, strlen(buffer) + 1, 0);
        if (sendResult == SOCKET_ERROR) {
            std::cerr << "�������ݸ��������˳���������룺" << WSAGetLastError() << std::endl;
            break;
        }

        // ��������������˳������˳�ѭ��
        if (strcmp(buffer, "�˳�") == 0) {
            break;
        }

        // ���շ�������Ӧ
        if (strcmp(buffer, "��ǰʱ��") == 0) {
            ReceiveTime();
        }
        else if (strncmp(buffer, "�ļ����䣺", 10) == 0) {
            ReceiveFile();
        } 
        else {
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
}

void Client::ReceiveTime() {
    const int BUFFER_SIZE = 128;
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    // ���շ������˷��͵�ʱ����Ϣ
    int recvResult = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (recvResult == SOCKET_ERROR) {
        std::cerr << "���շ�����ʱ����Ϣ����������룺" << WSAGetLastError() << std::endl;
        return;
    }

    if (recvResult == 0) {
        std::cerr << "�������Ͽ����ӣ��޷���ȡʱ����Ϣ" << std::endl;
        return;
    }

    // ��ʾ���������ص�ʱ��
    std::cout << "��������ǰʱ�䣺" << buffer << std::endl;
}

void Client::ReceiveFile() {
    const int BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    // ����ļ��Ƿ����
    int recvResult = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (recvResult == SOCKET_ERROR || recvResult == 0) {
        std::cerr << "�����ļ����ݳ�������ӹرգ�������룺" << WSAGetLastError() << std::endl;
        return;
    }
    if (strcmp(buffer, "�ļ�������") == 0) {
        std::cerr << "ָ���ļ�������" << std::endl;
        return;
    }

    std::string filename;
    std::cout << "�������ļ�����λ�ã�";
    std::getline(std::cin, filename);

    // �򿪱����ļ����ڽ�������
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "�޷����ļ��Ա�����յ����ݣ�" << filename << std::endl;
        return;
    }

    // ѭ����������ֱ�����������ļ�
    while (true) {
        // д�뵱ǰ���յ������ݿ�
        outFile.write(buffer, recvResult);

        // �ж��Ƿ���������ļ�������ʵ��������д���
        if (recvResult < BUFFER_SIZE) {
            break;
        }

        // �������ݿ�
        recvResult = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (recvResult == SOCKET_ERROR || recvResult == 0) {
            std::cerr << "�����ļ����ݳ�������ӹرգ�������룺" << WSAGetLastError() << std::endl;
            outFile.close();
            remove(filename.c_str());
            return;
        }
    }

    // ����ļ����չ������Ƿ���ִ���
    if (!outFile.good()) {
        std::cerr << "д���ļ����ݳ���" << std::endl;
        outFile.close();
        remove(filename.c_str());
        return;
    }

    // �رձ����ļ�
    outFile.close();
    std::cout << "�ļ�������ɣ�" << filename << std::endl;
}
