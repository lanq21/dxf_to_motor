/*
	Open(),Read(),Write()参考了
	https://wangbaiyuan.cn/c-serial-communication-write-reading.html
*/


#include "COM.h"
#include "Shape.h"
#include <iostream>
#include <string>
#include <fstream>

int COM::Delay_Time;
bool COM::Enable = false;

bool COM::Open()
{
	std::cout << "串口为：(输入数字，或直接按 Enter 仅输出到文本文档)\tCOM";
	std::wstring Name;
	getline(std::wcin, Name);
	if (Name.empty())
		return false;
	else if (std::stoi(Name) < 10)
		Name = L"COM" + Name;
	else
		Name = L"\\\\.\\COM" + Name;

	//串口句柄
	hCom = CreateFile(Name.c_str(), GENERIC_READ | GENERIC_WRITE/*允许读和写*/, 0/*独占方式*/, NULL, OPEN_EXISTING/*打开而不是创建*/, 0/*同步方式*/, NULL);

	if (hCom == INVALID_HANDLE_VALUE) return false;
	else
	{
		if (SetupComm(hCom, 64, 64) == false) return false; //输入缓冲区和输出缓冲区的大小都是1024

		COMMTIMEOUTS TimeOuts{};
		//设定读超时
		TimeOuts.ReadIntervalTimeout = 1000;
		TimeOuts.ReadTotalTimeoutMultiplier = 500;
		TimeOuts.ReadTotalTimeoutConstant = 5000;
		//设定写超时
		TimeOuts.WriteTotalTimeoutMultiplier = 500;
		TimeOuts.WriteTotalTimeoutConstant = 2000;
		if (SetCommTimeouts(hCom, &TimeOuts) == false) return false; //设置超时

		DCB dcb{};
		dcb.BaudRate = 9600; //波特率为9600
		dcb.ByteSize = 8; //每个字节有8位
		dcb.Parity = NOPARITY; //无奇偶校验位
		dcb.StopBits = ONESTOPBIT; //1个停止位
		if (SetCommState(hCom, &dcb) == false) return false;
		if (PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR) == false) return false;
		COM::Enable = true;
		return true;
	}

}

bool COM::Read()
{
	DWORD wCount;//读取的字节数
	if (ReadFile(hCom, Input, 100, &wCount, NULL)) return true;
	else return false;
}

bool COM::Write(double num)
{
	static int i = 1;
//	if (i == 1)
//		file_out << num << ',';
	if (i == 2)
		file_out << num << ',';
	i++;
	if (i == 4) i = 1;
	if (Enable)
	{
		std::string OutString = std::to_string(int(num + 0.5)); // 数字转字符串
		DWORD dwBytesWrite = 100;
		COMSTAT ComStat; // 端口状态
		DWORD dwErrorFlags;
		ClearCommError(hCom, &dwErrorFlags, &ComStat); // 清除错误状态

		bool bWriteStat = WriteFile(hCom, OutString.c_str(), dwBytesWrite, &dwBytesWrite, NULL);

		Sleep(Delay_Time);
		PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		if (bWriteStat)	return true;
		else return false;
	}
	return true;
}

void COM::Set_Delay_Time(const int time)
{
	Delay_Time = time;
}

void COM::Disable_COM()
{
	Enable = false;
}

COM::~COM()
{
	CloseHandle(hCom);
}

COM com;