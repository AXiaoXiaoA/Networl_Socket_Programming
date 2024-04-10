/**
 * @file
 * @brief ������ Client �࣬��ʾһ��������������������Ӳ�����ͨ�ŵĿͻ��ˡ�
 * @author ChangeeShaw
 * @version 1.0
 * @date 2024.04.01
 */

#pragma once

#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

 /**
  * @class Client
  * @brief �ͻ����࣬������������������Ӳ�����ͨ�ŵĿͻ��ˡ�
  */
class Client {
public:
	/**
	 * @brief ���캯������ʼ���ͻ��˶��󣬲�ָ��Ҫ���ӵķ�����IP�Ͷ˿ںš�
	 * @param serverIP ��������IP��ַ��
	 * @param serverPort �������Ķ˿ںš�
	 */
	Client(const char* serverIP, int serverPort);
	/**
	 * @brief �����������رտͻ��˲��ͷ���Դ��
	 */
	~Client();

	/**
	 * @brief ���ӷ�������
	 * @return �����Ƿ�ɹ���
	 */
	bool Connect();
	/**
	 * @brief ���пͻ��ˣ����ӷ���������ʼ����ͨ�š�
	 */
	void Run();

private:
	SOCKET clientSocket; ///< �ͻ����׽��ֱ�ʶ��
	SOCKADDR_IN serverAddr; ///< ��������ַ��Ϣ
};
