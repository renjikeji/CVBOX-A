#include <Windows.h>
#include <iostream>


#include "simple_call/simple_call.h"
#include "hid/HidTable.h"


void test_move()
{
	bool status = init_cvbox();
	printf("status %s \t\n", status ? "��ʼ���ɹ�" : "��ʼ��ʧ��");


	if (status)
	{
		Enable_Monitor(true);
		while (true)
		{
			if (GetAsyncKeyState(0x06) || GetAsyncKeyState(0x02))
			{
				bool call_status = cvbox_mouse_move(1, 0);
				printf("��������ƶ� %s \n", call_status ? "�ɹ�" : "ʧ��");
				//Sleep(1);
			}
			if (GetAsyncKeyState('4'))
			{
				printf("ִ���������400ms");
				cvbox_mouse_down(1);
				Sleep(400);
				cvbox_mouse_down(0);
			}

#if 0
			if (GetAsyncKeyState('5'))
			{
				printf("���½�������ģʽ");
				ReenterDownload();
			}
#endif
			// ����Ϊ��Ӳ���ж�ȡ����״̬
			if (MonitorKeyStatus(0x01))
			{
				printf("������� \n");
			}
			if (MonitorKeyStatus(0x02))
			{
				printf("�Ҽ����� \n");
			}
			if (MonitorKeyStatus(0x04))
			{
				printf("�м����� \n");
			}
			if (MonitorKeyStatus(0x08) || GetAsyncKeyState(0x05))
			{
				printf("�������� \n");

				// ģ�����windows Table ����
				PutOrReleaseKeyboard(KEY_LEFT_GUI, true);
				Sleep(120);
				PutOrReleaseKeyboard(KEY_TAB, true);
				Sleep(20);
				PutOrReleaseKeyboard(KEY_TAB, false);
				Sleep(120);
				PutOrReleaseKeyboard(KEY_LEFT_GUI, false);

			}
			if (MonitorKeyStatus(0x10))
			{
				printf("ǰ������� \n");
				// ģ�����windows ����
				PutOrReleaseKeyboard(KEY_LEFT_GUI,true);
				Sleep(20);
				PutOrReleaseKeyboard(KEY_LEFT_GUI,false);
			}

#if 0
			if (GetAsyncKeyState(VK_F5) && GetAsyncKeyState(VK_LMENU))
			{
				printf("���½�������ģʽ \n");
				init_cvbox();
				ReenterDownload();
			}
#endif


			Sleep(20);
		}

	}
}
int main()
{
	test_move();
	return 0;
}
