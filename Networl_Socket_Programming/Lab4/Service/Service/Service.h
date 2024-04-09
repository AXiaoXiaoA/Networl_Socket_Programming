#pragma once

#include <WinSock2.h>
#include <vector>
#include <string>

#pragma comment (lib,"ws2_32.lib")

/// <summary>
/// ���ߣ�ChangeeShaw
/// �ļ����ܣ�������Service�࣬��ʾһ���ܹ������������ͻ������ӵ�TCP������
/// </summary>

/// <summary>
/// �������ࣺ�ܹ����ܿͻ������Ӳ������������ͻ�����Ϣ��TCP������
/// </summary>
class Service {
public:
    /// <summary>
    /// ���캯������ʼ����������׼����ʼ����ָ���˿��ϵĿͻ�������
    /// </summary>
    /// <param name="port">�����������Ķ˿ں�</param>
    Service(int port);

    /// <summary>
    /// �����������رշ�������������Դ
    /// </summary>
    ~Service();

    /// <summary>
    /// ��������������ʼ���ܿͻ������Ӳ�������Ϣ
    /// </summary>
    void Start();

private:
    /// <summary>
    /// ѭ�����ܿͻ������ӣ���ʹ�ö��̴߳���ÿ������
    /// </summary>
    void AcceptClient();

    /// <summary>
    /// �����������ӿͻ��˵���Ϣ�������ڶ������߳���
    /// </summary>
    /// <param name="clientSocket">����ͨ�ŵĿͻ����׽���</param>
    void HandleClient(SOCKET clientSocket);

    /// <summary>
    /// ��ָ���ͻ����׽��ַ��͵�ǰϵͳʱ��
    /// </summary>
    /// <param name="clientSocket">����ͨ�ŵĿͻ����׽���</param>
    void SendTime(SOCKET clientSocket);

    /// <summary>
    /// ��ָ���ͻ����׽��ַ���ָ���ļ�������
    /// </summary>
    /// <param name="clientSocket">����ͨ�ŵĿͻ����׽���</param>
    /// <param name="filename">Ҫ���͵��ļ���</param>
    void SendFile(SOCKET clientSocket, const std::string& filename);

private:
    SOCKET serverSocket; // �������׽���
    std::vector<SOCKET> clientSockets;  // �����ӵĿͻ����׽����б�
};
