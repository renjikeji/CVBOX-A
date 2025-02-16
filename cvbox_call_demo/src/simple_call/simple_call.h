#ifndef simple_call_20231006
#define simple_call_20231006

#include "../hid/hidapi.h"
#include <Windows.h>
#include <iostream>
#include <thread>

#define CMD_MOUSE_MOVE 0x08
#define CMD_MOUSE_BUTTON 0x09

#define CMD_ReenterDownload 0xFE // 从新进入下载模式
#define CMD_GETINFO 0x06 // 获取值
#define CMD_SETINFO 0x07 // 设置值
#define CMD_SETINFO_SETMonitor 0x01
#define CMD_SET_CHANGE 0x02



#pragma pack(push, 1)  // 保存当前对齐状态，并设置新的对齐为1

// 定义结构体
static struct InformationStruct
{
    unsigned char reserved = 0x00;//单片机传输中需要多出一个字节
    unsigned char cmd;   //cmd 命令符
    unsigned char recv[63]; // 置空 用其他结构体进行转换
}InformationData = { 0 };

typedef struct _SendMouseMoveInfo {
    unsigned char reserved = 0x00;//单片机传输中需要多出一个字节
    unsigned char cmd;
    short move_x;
    short move_y;
    unsigned char recv[62]; // 置空 用其他结构体进行转换
} SendMouseMoveInfo;

typedef struct _SendMouseButtonInfo {
    unsigned char reserved = 0x00;//单片机传输中需要多出一个字节
    unsigned char cmd;
    unsigned char button;
    unsigned char recv[63]; // 置空 用其他结构体进行转换
} SendMouseButtonInfo;


typedef struct _SendKbdnInfo {
    unsigned char reserved = 0x00;//单片机传输中需要多出一个字节
    unsigned char cmd;
    unsigned char keyData[8] = {0x00};
    unsigned char recv[65-10]; // 置空 用其他结构体进行转换
} SendKbdnInfo;


typedef struct _ReadHardInfo {
    unsigned char reserved = 0x00;//单片机传输中需要多出一个字节
    unsigned char button;
    unsigned char recv[64]; // 置空 用其他结构体进行转换
} ReadHardInfo;



#pragma pack(pop)  // 恢复之前的对齐状态

bool init_cvbox(); // 初始化硬件
bool cvbox_mouse_move( short move_x,short move_y); // 调用鼠标移动
bool cvbox_mouse_down(uint8_t button_stauts); // 鼠标点击
bool ReenterDownload(); // 使单片机重新进入下载模式

int MonitorLeft();
bool MonitorKeyStatus(int vk);
void Enable_Monitor(bool status);

extern int PutOrReleaseKeyboard(int vk_key, bool is_down); // 按下或松开按键
#endif
