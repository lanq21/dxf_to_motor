/*
	Open(),Read(),Write()参考了
	https://wangbaiyuan.cn/c-serial-communication-write-reading.html
*/


#include "COM.h"
#include "Shape.h"
#include <iostream>
#include <string>
#include <fstream>

COM::COM()
{
	while(true)
	{
		std::cout << "串口为：(输入数字)\tCOM";
		int num;
		std::cin >> num;
		if(Open(num)) break;
	}
}

bool COM::Open(const int num)
{
	std::wstring Name=std::to_wstring(num);
	Name = L"COM" + Name;

	//串口句柄
	hCom = CreateFile(Name.c_str(), GENERIC_READ | GENERIC_WRITE/*允许读和写*/, 0/*独占方式*/, NULL, OPEN_EXISTING/*打开而不是创建*/, 0/*同步方式*/, NULL);

	if (hCom == INVALID_HANDLE_VALUE)
	{
		std::cerr << "串口未打开\n";
		return false;
	}
	else
	{
		if(SetupComm(hCom, 1024, 1024)==false) return false; //输入缓冲区和输出缓冲区的大小都是1024

		COMMTIMEOUTS TimeOuts{};
		//设定读超时
		TimeOuts.ReadIntervalTimeout = 1000;
		TimeOuts.ReadTotalTimeoutMultiplier = 500;
		TimeOuts.ReadTotalTimeoutConstant = 5000;
		//设定写超时
		TimeOuts.WriteTotalTimeoutMultiplier = 500;
		TimeOuts.WriteTotalTimeoutConstant = 2000;
		if(SetCommTimeouts(hCom, &TimeOuts)==false) return false; //设置超时

		DCB dcb{};
		dcb.BaudRate = 9600; //波特率为9600
		dcb.ByteSize = 8; //每个字节有8位
		dcb.Parity = NOPARITY; //无奇偶校验位
		dcb.StopBits = TWOSTOPBITS; //两个停止位
		if(SetCommState(hCom, &dcb)==false) return false;
		if(PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR)==false) return false;
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
	file_out << num << std::endl;
	std::string OutString = std::to_string(num); // 数字转字符串
	DWORD dwBytesWrite = 100;
	COMSTAT ComStat; // 端口状态
	DWORD dwErrorFlags;
	ClearCommError(hCom, &dwErrorFlags, &ComStat); // 清除错误状态
//	bool bWriteStat = WriteFile(hCom, OutString.c_str(), dwBytesWrite, &dwBytesWrite, NULL);
	bool bWriteStat = true;
	PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
//	Sleep(100);
	if (bWriteStat)	return true;
	else return false;
}

COM::~COM()
{
	CloseHandle(hCom);
}

COM com;