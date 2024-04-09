#pragma once

#include <WinSock2.h>

#pragma comment (lib,"ws2_32.lib")

/// <summary>
/// ���ߣ�ChangeeShaw
/// �ļ����ܣ�������Service�࣬��ʾһ���ܹ����ܿͻ������Ӳ�������ϢTCP������
/// </summary>

/// <summary>
/// �������ࣺ�ܹ����ܿͻ������Ӳ�������Ϣ��TCP������
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
    /// ѭ�����ܿͻ�������
    /// </summary>
    void AcceptClient();

    /// <summary>
    /// ���������ӿͻ��˵���Ϣ
    /// </summary>
    /// <param name="clientSocket">����ͨ�ŵĿͻ����׽���</param>
    void HandleClient(SOCKET clientSocket);

private:
    SOCKET serverSocket; // �������׽���
};
