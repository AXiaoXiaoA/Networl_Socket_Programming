#include "Service.h"

#include <WS2tcpip.h>
#include <iostream>

Service::Service(int port) {
	// ��ʼ�� Winsock
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0) {
		std::cerr << "Winsock��ʼ��ʧ�ܣ�" << WSAGetLastError() << std::endl;
		throw std::runtime_error("Winsock��ʼ��ʧ�ܣ�");
	}

	// �����������׽���
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		std::cerr << "�������׽��ִ���ʧ�ܣ�" << WSAGetLastError() << std::endl;
		WSACleanup();
		throw std::runtime_error("�������׽��ִ���ʧ�ܣ�");
	}

	// ���÷�������ַ�Ͷ˿�
	SOCKADDR_IN serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// �󶨷������׽��ֵ�ָ����ַ�Ͷ˿�
	if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		std::cerr << "�������׽��ְ�ʧ�ܣ�" << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		throw std::runtime_error("�������׽��ְ�ʧ�ܣ�");
	}

	// �����ͻ�����������
	if (listen(serverSocket, 2) == SOCKET_ERROR) {
		std::cerr << "�����������ͻ�����������ʧ�ܣ�" << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		WSACleanup();
		throw std::runtime_error("�����������ͻ�����������ʧ�ܣ�");
	}
}

Service::~Service() {
	// �رշ������׽��ֲ����� Winsock
	closesocket(serverSocket);
	WSACleanup();
}

void Service::Start() {
	while (true) {
		std::cout << "�ȴ��ͻ�������..." << std::endl;
		AcceptClient();
	}
}

void Service::AcceptClient() {
	SOCKADDR_IN clientAddr;
	int addrSize = sizeof(clientAddr);

	// ���ܿͻ������ӣ����ؿͻ����׽���
	SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize);

	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "���ܿͻ�������ʧ�ܣ�������룺" << WSAGetLastError() << std::endl;
		closesocket(clientSocket);
		return;
	}
	else {
		// ��ȡ�ͻ��� IP �Ͷ˿���Ϣ
		char ipBuffer[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &(clientAddr.sin_addr), ipBuffer, INET_ADDRSTRLEN);
		std::cout << "�ͻ��ˣ�" << ipBuffer << "��ͨ���˿ڣ�" << ntohs(clientAddr.sin_port) << "�����ӳɹ�" << std::endl;
		
		// ����ͻ�����Ϣ
		HandleClient(clientSocket);
	}
}

void Service::HandleClient(SOCKET clientSocket) {
	int const CLIENT_MSG_SIZE = 128;
	char inMSG[CLIENT_MSG_SIZE];
	char outMSG[CLIENT_MSG_SIZE];

	while (true) {
		// ���տͻ�����Ϣ
		int recvResult = recv(clientSocket, inMSG, CLIENT_MSG_SIZE, 0);
		if (recvResult == SOCKET_ERROR) {
			std::cerr << "�Ի��жϣ�������룺" << WSAGetLastError() << std::endl;
			closesocket(clientSocket);
			return;
		}
		std::cout << "�ͻ������" << inMSG << std::endl;

		// ����ͻ�������
		if (strcmp(inMSG, "��ǰʱ��") == 0) {
			// ��ȡϵͳ��ǰʱ��
			SYSTEMTIME systime = { 0 };
			GetLocalTime(&systime);
			sprintf_s(outMSG, CLIENT_MSG_SIZE, "%d-%02d-%02d %02d:%02d:%02d",
				systime.wYear, systime.wMonth, systime.wDay,
				systime.wHour, systime.wMinute, systime.wSecond);

			// ����ʱ����Ϣ���ͻ���
			int sendResult = send(clientSocket, outMSG, strlen(outMSG), 0);
			if (sendResult == SOCKET_ERROR) {
				std::cerr << "�������ݸ��ͻ��˳���������룺" << WSAGetLastError() << std::endl;
			}
		}
		else if (strcmp(inMSG, "�˳�") == 0) {
			std::cout << "�ͻ��˳ɹ��˳�����" << std::endl;
			closesocket(clientSocket);
			return;
		}
		else {
			const char* message = "��Ч����";
			int sendResult = send(clientSocket, message, strlen(message), 0);
			if (sendResult == SOCKET_ERROR) {
				std::cerr << "�������ݸ��ͻ��˳���������룺" << WSAGetLastError() << std::endl;
			}
		}
	}
}
