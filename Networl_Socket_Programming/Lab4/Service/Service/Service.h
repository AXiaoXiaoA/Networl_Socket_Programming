/**
 * @file
 * @brief ������ Service �࣬��ʾһ���ܹ������������ͻ������ӵ� TCP ��������
 * @author ChangeeShaw
 * @version 1.0
 * @date 2024.04.01
 */

#pragma once

#include <WinSock2.h>
#include <vector>
#include <string>

#pragma comment (lib,"ws2_32.lib")

 /**
  * @class Service
  * @brief �������࣬�ܹ����ܿͻ������Ӳ������������ͻ�����Ϣ�� TCP ��������
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
	 * @brief ѭ�����ܿͻ������ӣ���ʹ�ö��̴߳���ÿ�����ӡ�
	 */
	void AcceptClient();

	/**
	 * @brief �����������ӿͻ��˵���Ϣ�������ڶ������߳��С�
	 * @param clientSocket ����ͨ�ŵĿͻ����׽��֡�
	 */
	void HandleClient(SOCKET clientSocket);

	/**
	 * @brief ��ָ���ͻ����׽��ַ��͵�ǰϵͳʱ�䡣
	 * @param clientSocket ����ͨ�ŵĿͻ����׽��֡�
	 */
	void SendTime(SOCKET clientSocket);
	/**
	 * @brief ��ָ���ͻ����׽��ַ���ָ���ļ������ݡ�
	 * @param clientSocket ����ͨ�ŵĿͻ����׽��֡�
	 * @param filename Ҫ���͵��ļ�����
	 */
	void SendFile(SOCKET clientSocket, const std::string& filename);

private:
	SOCKET serverSocket; ///< �������׽���
	std::vector<SOCKET> clientSockets; ///< �����ӵĿͻ����׽����б�
};
