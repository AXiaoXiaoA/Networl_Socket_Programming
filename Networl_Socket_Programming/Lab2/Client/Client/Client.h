#pragma once

#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib")

/// <summary>
/// ���ߣ�ChangeeShaw
/// �ļ����ܣ�������Client�࣬��ʾһ��������������������Ӳ�����ͨ�ŵĿͻ���
/// </summary>

/// <summary>
/// �ͻ����ࣺ������������������Ӳ�����ͨ�ŵĿͻ���
/// </summary>
class Client {
public:
    /// <summary>
    /// ���캯������ʼ���ͻ��˶��󣬲�ָ��Ҫ���ӵķ�����IP�Ͷ˿ں�
    /// </summary>
    /// <param name="serverIP">��������IP��ַ</param>
    /// <param name="serverPort">�������Ķ˿ں�</param>
    Client(const char* serverIP, int serverPort);

    /// <summary>
    /// �����������رտͻ��˲��ͷ���Դ
    /// </summary>
    ~Client(); // �رտͻ���

    /// <summary>
    /// ���ӷ�����
    /// </summary>
    /// <returns>�����Ƿ�ɹ�</returns>
    bool Connect();

    /// <summary>
    /// ���пͻ��ˣ����ӷ���������ʼ����ͨ��
    /// </summary>
    void Run();

private:
    SOCKET clientSocket; // �ͻ����׽��ֱ�ʶ��
    SOCKADDR_IN serverAddr; // ��������ַ��Ϣ
};
