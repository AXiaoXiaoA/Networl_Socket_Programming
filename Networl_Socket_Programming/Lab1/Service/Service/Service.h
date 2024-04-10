/**
 * @file
 * @brief ������ Service �࣬��ʾһ���ܹ����ܿͻ������Ӳ�������Ϣ�� TCP ��������
 * @author ChangeeShaw
 * @version 1.0
 * @date 2024.04.01
 */

#pragma once

#include <WinSock2.h>

#pragma comment (lib,"ws2_32.lib")

 /**
  * @class Service
  * @brief �������࣬�ܹ����ܿͻ������Ӳ�������Ϣ�� TCP ��������
  */
class Service {
public:
	/**
	 * @brief ���캯������ʼ����������׼����ʼ����ָ���˿��ϵĿͻ������ӡ�
	 * @param port �����������Ķ˿ںš�
	 */
	Service(int port);
	/**
	 * @brief �����������رշ�������������Դ��
	 */
	~Service();

	/**
	 * @brief ��������������ʼ���ܿͻ������Ӳ�������Ϣ��
	 */
	void Start();

private:
	/**
	 * @brief ѭ�����ܿͻ������ӡ�
	 */
	void AcceptClient();

	/**
	 * @brief ���������ӿͻ��˵���Ϣ��
	 * @param clientSocket ����ͨ�ŵĿͻ����׽��֡�
	 */
	void HandleClient(SOCKET clientSocket);

private:
	SOCKET serverSocket; ///< �������׽���
};
