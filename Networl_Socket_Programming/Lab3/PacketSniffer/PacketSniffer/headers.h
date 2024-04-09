#pragma once

//IP报文格式
typedef struct IP {
    unsigned char  headLen : 4;   // 头部长度
    unsigned char  version : 4;   // IP 版本
    unsigned char  tos;           // 服务类型
    unsigned short totalLen;      // 总长度
    unsigned short id;            // 标识
    unsigned short fragOff;       // 分段偏移
    unsigned char  ttl;           // 存活时间
    unsigned char  protocol;      // 协议
    unsigned short checksum;      // 校验和
    unsigned long  sourceAddr;    // 源 IP 地址
    unsigned long  destinAddr;    // 目的 IP 地址
}IPHeader;

//TCP报文格式
typedef struct TCP {
    unsigned short sourcePort;    // 源端口号
    unsigned short destinPort;    // 目的端口号
    unsigned long  seqNum;        // 序列号
    unsigned long  ackNum;        // 确认号
    unsigned char  headLen : 4;   // 头部长度
    unsigned char  reserved : 4;  // 保留位
    unsigned short flags;         // 控制位
    unsigned short window;        // 窗口大小
    unsigned short checksum;      // 校验和
    unsigned short urgentPtr;     // 紧急指针
}TCPHeader;