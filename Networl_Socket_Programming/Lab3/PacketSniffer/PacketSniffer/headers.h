#pragma once

//IP���ĸ�ʽ
typedef struct IP {
    unsigned char  headLen : 4;   // ͷ������
    unsigned char  version : 4;   // IP �汾
    unsigned char  tos;           // ��������
    unsigned short totalLen;      // �ܳ���
    unsigned short id;            // ��ʶ
    unsigned short fragOff;       // �ֶ�ƫ��
    unsigned char  ttl;           // ���ʱ��
    unsigned char  protocol;      // Э��
    unsigned short checksum;      // У���
    unsigned long  sourceAddr;    // Դ IP ��ַ
    unsigned long  destinAddr;    // Ŀ�� IP ��ַ
}IPHeader;

//TCP���ĸ�ʽ
typedef struct TCP {
    unsigned short sourcePort;    // Դ�˿ں�
    unsigned short destinPort;    // Ŀ�Ķ˿ں�
    unsigned long  seqNum;        // ���к�
    unsigned long  ackNum;        // ȷ�Ϻ�
    unsigned char  headLen : 4;   // ͷ������
    unsigned char  reserved : 4;  // ����λ
    unsigned short flags;         // ����λ
    unsigned short window;        // ���ڴ�С
    unsigned short checksum;      // У���
    unsigned short urgentPtr;     // ����ָ��
}TCPHeader;