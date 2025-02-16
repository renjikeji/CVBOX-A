#ifndef simple_call_20231006
#define simple_call_20231006

#include "../hid/hidapi.h"
#include <Windows.h>
#include <iostream>
#include <thread>

#define CMD_MOUSE_MOVE 0x08
#define CMD_MOUSE_BUTTON 0x09

#define CMD_ReenterDownload 0xFE // ���½�������ģʽ
#define CMD_GETINFO 0x06 // ��ȡֵ
#define CMD_SETINFO 0x07 // ����ֵ
#define CMD_SETINFO_SETMonitor 0x01
#define CMD_SET_CHANGE 0x02



#pragma pack(push, 1)  // ���浱ǰ����״̬���������µĶ���Ϊ1

// ����ṹ��
static struct InformationStruct
{
    unsigned char reserved = 0x00;//��Ƭ����������Ҫ���һ���ֽ�
    unsigned char cmd;   //cmd �����
    unsigned char recv[63]; // �ÿ� �������ṹ�����ת��
}InformationData = { 0 };

typedef struct _SendMouseMoveInfo {
    unsigned char reserved = 0x00;//��Ƭ����������Ҫ���һ���ֽ�
    unsigned char cmd;
    short move_x;
    short move_y;
    unsigned char recv[62]; // �ÿ� �������ṹ�����ת��
} SendMouseMoveInfo;

typedef struct _SendMouseButtonInfo {
    unsigned char reserved = 0x00;//��Ƭ����������Ҫ���һ���ֽ�
    unsigned char cmd;
    unsigned char button;
    unsigned char recv[63]; // �ÿ� �������ṹ�����ת��
} SendMouseButtonInfo;


typedef struct _SendKbdnInfo {
    unsigned char reserved = 0x00;//��Ƭ����������Ҫ���һ���ֽ�
    unsigned char cmd;
    unsigned char keyData[8] = {0x00};
    unsigned char recv[65-10]; // �ÿ� �������ṹ�����ת��
} SendKbdnInfo;


typedef struct _ReadHardInfo {
    unsigned char reserved = 0x00;//��Ƭ����������Ҫ���һ���ֽ�
    unsigned char button;
    unsigned char recv[64]; // �ÿ� �������ṹ�����ת��
} ReadHardInfo;



#pragma pack(pop)  // �ָ�֮ǰ�Ķ���״̬

bool init_cvbox(); // ��ʼ��Ӳ��
bool cvbox_mouse_move( short move_x,short move_y); // ��������ƶ�
bool cvbox_mouse_down(uint8_t button_stauts); // �����
bool ReenterDownload(); // ʹ��Ƭ�����½�������ģʽ

int MonitorLeft();
bool MonitorKeyStatus(int vk);
void Enable_Monitor(bool status);

extern int PutOrReleaseKeyboard(int vk_key, bool is_down); // ���»��ɿ�����
#endif
