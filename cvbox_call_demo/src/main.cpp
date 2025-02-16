#include <Windows.h>
#include <iostream>


#include "simple_call/simple_call.h"
#include "hid/HidTable.h"


void test_move()
{
	bool status = init_cvbox();
	printf("status %s \t\n", status ? "初始化成功" : "初始化失败");


	if (status)
	{
		Enable_Monitor(true);
		while (true)
		{
			if (GetAsyncKeyState(0x06) || GetAsyncKeyState(0x02))
			{
				bool call_status = cvbox_mouse_move(1, 0);
				printf("调用鼠标移动 %s \n", call_status ? "成功" : "失败");
				//Sleep(1);
			}
			if (GetAsyncKeyState('4'))
			{
				printf("执行左键按下400ms");
				cvbox_mouse_down(1);
				Sleep(400);
				cvbox_mouse_down(0);
			}

#if 0
			if (GetAsyncKeyState('5'))
			{
				printf("重新进入下载模式");
				ReenterDownload();
			}
#endif
			// 以下为从硬件中读取按键状态
			if (MonitorKeyStatus(0x01))
			{
				printf("左键按下 \n");
			}
			if (MonitorKeyStatus(0x02))
			{
				printf("右键按下 \n");
			}
			if (MonitorKeyStatus(0x04))
			{
				printf("中键按下 \n");
			}
			if (MonitorKeyStatus(0x08) || GetAsyncKeyState(0x05))
			{
				printf("后侧键按下 \n");

				// 模拟键盘windows Table 按下
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
				printf("前侧键按下 \n");
				// 模拟键盘windows 按下
				PutOrReleaseKeyboard(KEY_LEFT_GUI,true);
				Sleep(20);
				PutOrReleaseKeyboard(KEY_LEFT_GUI,false);
			}

#if 0
			if (GetAsyncKeyState(VK_F5) && GetAsyncKeyState(VK_LMENU))
			{
				printf("重新进入下载模式 \n");
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
