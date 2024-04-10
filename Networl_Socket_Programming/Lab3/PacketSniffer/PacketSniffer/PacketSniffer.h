/**
 * @file
 * @brief ������ PacketSniffer �࣬���ڲ���ָ��IP��ַ�����ݰ�������
 * @author ChangeeShaw
 * @version 1.0
 * @date 2024.04.01
 */

#pragma once

#include <WinSock2.h>

 /**
  * @class PacketSniffer
  * @brief PacketSniffer �࣬���ڲ���ָ��IP��ַ�����ݰ�������
  */
class PacketSniffer {
public:
	/**
	 * @brief ���캯������ʼ�� PacketSniffer ����ָ��Ҫ�������ݰ���IP��ַ��
	 * @param ipAddress Ҫ�������ݰ���Ŀ��IP��ַ��
	 */
	PacketSniffer(const char* ipAddress);
	/**
	 * @brief �����������ر�Socket���ͷ���Դ��
	 */
	~PacketSniffer();

	/**
	 * @brief �������ݰ�����ʹ���
	 */
	void Start();

private:
	/**
	 * @brief ������յ������ݰ���
	 * @param buffer ���յ������ݰ���������
	 * @param bytesRecv ���յ����ֽ�����
	 */
	void ProcessPacket(char* buffer, int bytesRecv);

private:
	SOCKET sock; ///< Socket�����������ݰ�����
};
