#pragma once
#include <Windows.h>
#undef max
#undef min

class COM
{
public:

	COM();

	bool Open(const int num); // 输入端口号打开串口

	bool Read(); // 读取串口内容到str

	bool Write(double num);	//将数字输出到串口

	~COM();

private:

	HANDLE hCom;
	char Input[100]; // 读取串口内容

};

extern COM com;