#include "simple_call.h"
#include "tamp.h"
#include <Windows.h>
#include <iostream>
#include <thread>
#include "../hid/HidTable.h"
#include <math.h>

#include <algorithm>
HANDLE g_handle = NULL; // ȫ�־��
bool g_enable_Monitor_status = false;
ReadHardInfo ReadButtonInfo = {0x00};
uint8_t keyInfo[8] = {0x00};

bool init_cvbox()
{
	// ����hid ��ʼ��
	hid_device_info *hid_info = hid_enumerate(0x046d, 0xC539);
	if (g_handle != NULL)
		CloseHandle(g_handle);

	do
	{
		if (hid_info == NULL)
			return false;
		if (hid_info->usage_page == 0xff20)
		{
			break;
		}
		else
			hid_info = hid_info->next;
	} while (1);

	DWORD desired_access = (FALSE) ? 0 : (GENERIC_WRITE | GENERIC_READ);
	DWORD share_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;

	g_handle = CreateFileA(hid_info->path,
						   desired_access,
						   share_mode,
						   NULL,
						   OPEN_EXISTING,
						   FILE_ATTRIBUTE_NORMAL,
						   0);

	if (g_handle != NULL)
	{
		return true;
	}
	return false;
}

bool cvbox_mouse_down(uint8_t button_stauts)
{
	// �ǿ��ж�
	if (g_handle == NULL)
		return false;

	SendMouseButtonInfo MouseButtonInfo = {0x00};
	MouseButtonInfo.cmd = CMD_MOUSE_BUTTON;
	MouseButtonInfo.button = button_stauts;
	BOOL res = WriteFile(g_handle, (unsigned char *)&MouseButtonInfo, 65, NULL, NULL);
	return res;
}
// ��������ƶ�
bool cvbox_mouse_move(short move_x, short move_y)
{
	// �ǿ��ж�
	if (g_handle == NULL)
		return false;

	SendMouseMoveInfo moveInfo = {0x00};
	moveInfo.cmd = CMD_MOUSE_MOVE;
	moveInfo.move_x = move_x;
	moveInfo.move_y = move_y;
	BOOL res = WriteFile(g_handle, (unsigned char *)&moveInfo, 65, NULL, NULL);
	return res;
}

// ���½�������ģʽ
bool ReenterDownload()
{
	// �ǿ��ж�
	if (g_handle == NULL)
	{
		// ���½�������ģʽʧ��
		return false;
	}

	// �������� ִ�ж�Ӧ�Ĳ���
	InformationData.cmd = 0xfe; // ���½�������ģʽ�ķ���
	BOOL res = WriteFile(g_handle, (unsigned char *)&InformationData, 65, NULL, NULL);
	return res;
}

int MonitorLeft()
{
	if (g_handle == 0)
		return -1;
	return ReadButtonInfo.button & 0x01 ? 1 : 0;
}

bool MonitorKeyStatus(int vk)
{
	if (g_handle == 0)
		return false;
	return ReadButtonInfo.button & vk;
}

void Monitor_status()
{
	DWORD bytes_read = 0;

	printf("read \n");
	// ����ȫ��״̬
	while (g_enable_Monitor_status)
	{
		bool res = ReadFile(g_handle, &ReadButtonInfo, 65, &bytes_read, NULL);
		if (res)
		{
			Sleep(1);
			/*printf("mouse status %d mouse x : %d mouse y : %d \n", recvStruct->mouse_status,combine_uint8_to_int(recvStruct->mouse_x_low, recvStruct->mouse_x_high),
				combine_uint8_to_int(recvStruct->mouse_y_low, recvStruct->mouse_y_high));*/
			/*	if (combine_uint8_to_int(recvStruct->mouse_y_high, recvStruct->mouse_y_low) == 65280)
				{
					printf("1\n");
				}*/
		}
	}
}
void Enable_Monitor(bool status)
{
	// ����ֵ
	InformationData.cmd = CMD_SETINFO; // ��ȡ��Ϣ
	InformationData.recv[0] = CMD_SETINFO_SETMonitor;
	InformationData.recv[1] = status;
	BOOL res = WriteFile(g_handle, (unsigned char *)&InformationData, 65, NULL, NULL);
	memset((unsigned char *)&InformationData, 0x00, sizeof((unsigned char *)&InformationData));
	// �����Ҫ����
	if (status)
	{
		g_enable_Monitor_status = status;
		// ����һ���߳̽���ͨѶ
		std::thread(Monitor_status).detach();
	}
	else
	{
		g_enable_Monitor_status = status;
	}
}

// ��������Ϣ�洢�ڱ��ز�����
int PutOrReleaseKeyboard(int vk_key, bool is_down)
{
	if (vk_key >= KEY_LEFTCONTROL && vk_key <= KEY_RIGHT_GUI) // ���Ƽ�
	{
		switch (vk_key)
		{
		case KEY_LEFTCONTROL:
			is_down ? keyInfo[0] |= BIT0 : keyInfo[0] &= ~BIT0;
			break;
		case KEY_LEFTSHIFT:
			is_down ? keyInfo[0] |= BIT1 : keyInfo[0] &= ~BIT1;
			break;
		case KEY_LEFTALT:
			is_down ? keyInfo[0] |= BIT2 : keyInfo[0] &= ~BIT2;
			break;
		case KEY_LEFT_GUI:
			is_down ? keyInfo[0] |= BIT3 : keyInfo[0] &= ~BIT3;
			break;
		case KEY_RIGHTCONTROL:
			is_down ? keyInfo[0] |= BIT4 : keyInfo[0] &= ~BIT4;
			break;
		case KEY_RIGHTSHIFT:
			is_down ? keyInfo[0] |= BIT5 : keyInfo[0] &= ~BIT5;
			break;
		case KEY_RIGHTALT:
			is_down ? keyInfo[0] |= BIT6 : keyInfo[0] &= ~BIT6;
			break;
		case KEY_RIGHT_GUI:
			is_down ? keyInfo[0] |= BIT7 : keyInfo[0] &= ~BIT7;
			break;
		}
	}
	else // �����
	{

		if (is_down) // ����ǰ���
		{
			for (int i = 2; i < 8; i++)
			{
				if (keyInfo[i] == 0) // �ҵ�ֵΪ0���±�
				{
					keyInfo[i] = vk_key; // д��ֵ
					break;
					//return 1;
				}
			}
		}
		else // ������ɿ�
		{
			for (int i = 2; i < 8; i++)
			{
				if (keyInfo[i] == vk_key) // �ҵ���Ӧ�İ���
				{
					keyInfo[i] = 0; // ����Ϊ0
					break;
				}
			}
			//// ֱ�Ӷ�2-8�������� ����Ϊ0������
			//for (int i = 2; i < 8; i++)
			//{																																																																		
			//	if (keyInfo[i] == 0)
			//	{
			//		std::swap(keyInfo[i], keyInfo[8]);
			//	}
			//}
		}
	}

	SendKbdnInfo ski;
	ski.cmd = 0x10;
	memcpy(ski.keyData, keyInfo, sizeof(keyInfo));
	BOOL res = FALSE;
	res = WriteFile(g_handle, (unsigned char *)&ski, 65, NULL, NULL);
	return res;
}
