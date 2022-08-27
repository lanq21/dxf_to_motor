#pragma once
#include <Windows.h>
#undef max
#undef min

class COM
{
public:

	bool Open(); // 输入端口号打开串口

	bool Read(); // 读取串口内容到str

	bool Write(double num);	//将数字输出到串口

	static void Set_Delay_Time(const int time); // 设置发送间隔

	static void Disable_COM(); // 停用串口发送

	~COM();

private:

	HANDLE hCom;
	char Input[100]; // 读取串口内容
	static int Delay_Time; // 发送间隔
	static bool Enable; // 使用串口发送

};

extern COM com;