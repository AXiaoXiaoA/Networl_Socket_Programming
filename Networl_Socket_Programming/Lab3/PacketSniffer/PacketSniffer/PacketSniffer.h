#pragma once

#include <WinSock2.h>

/// <summary>
/// ���ߣ�ChangeeShaw
/// �ļ����ܣ�������PacketSniffer�࣬���ڲ���ָ��IP��ַ�����ݰ�������
/// </summary>

/// <summary>
/// PacketSniffer�ࣺ���ڲ���ָ��IP��ַ�����ݰ�������
/// </summary>
class PacketSniffer {
public:
    /// <summary>
    /// ���캯������ʼ��PacketSniffer����ָ��Ҫ�������ݰ���IP��ַ��
    /// </summary>
    /// <param name="ipAddress">Ҫ�������ݰ���Ŀ��IP��ַ</param>
    PacketSniffer(const char* ipAddress);
    
    /// <summary>
    /// �����������ر�Socket���ͷ���Դ��
    /// </summary>
    ~PacketSniffer();

    /// <summary>
    /// �������ݰ�����ʹ���
    /// </summary>
    void Start();

private:
    /// <summary>
    /// ������յ������ݰ���
    /// </summary>
    /// <param name="buffer">���յ������ݰ�������</param>
    /// <param name="bytesRecv">���յ����ֽ���</param>
    void ProcessPacket(char* buffer, int bytesRecv);

private:
    SOCKET sock; // Socket�����������ݰ�����
};
