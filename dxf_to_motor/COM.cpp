/*
	Open(),Read(),Write()�ο���
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
		std::cout << "����Ϊ��(��������)\tCOM";
		int num;
		std::cin >> num;
		if(Open(num)) break;
	}
}

bool COM::Open(const int num)
{
	std::wstring Name=std::to_wstring(num);
	Name = L"COM" + Name;

	//���ھ��
	hCom = CreateFile(Name.c_str(), GENERIC_READ | GENERIC_WRITE/*�������д*/, 0/*��ռ��ʽ*/, NULL, OPEN_EXISTING/*�򿪶����Ǵ���*/, 0/*ͬ����ʽ*/, NULL);

	if (hCom == INVALID_HANDLE_VALUE)
	{
		std::cerr << "����δ��\n";
		return false;
	}
	else
	{
		if(SetupComm(hCom, 1024, 1024)==false) return false; //���뻺����������������Ĵ�С����1024

		COMMTIMEOUTS TimeOuts{};
		//�趨����ʱ
		TimeOuts.ReadIntervalTimeout = 1000;
		TimeOuts.ReadTotalTimeoutMultiplier = 500;
		TimeOuts.ReadTotalTimeoutConstant = 5000;
		//�趨д��ʱ
		TimeOuts.WriteTotalTimeoutMultiplier = 500;
		TimeOuts.WriteTotalTimeoutConstant = 2000;
		if(SetCommTimeouts(hCom, &TimeOuts)==false) return false; //���ó�ʱ

		DCB dcb{};
		dcb.BaudRate = 9600; //������Ϊ9600
		dcb.ByteSize = 8; //ÿ���ֽ���8λ
		dcb.Parity = NOPARITY; //����żУ��λ
		dcb.StopBits = TWOSTOPBITS; //����ֹͣλ
		if(SetCommState(hCom, &dcb)==false) return false;
		if(PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR)==false) return false;
		return true;
	}
}

bool COM::Read()
{
	DWORD wCount;//��ȡ���ֽ���
	if (ReadFile(hCom, Input, 100, &wCount, NULL)) return true;
	else return false;
}

bool COM::Write(double num)
{
	file_out << num << std::endl;
	std::string OutString = std::to_string(num); // ����ת�ַ���
	DWORD dwBytesWrite = 100;
	COMSTAT ComStat; // �˿�״̬
	DWORD dwErrorFlags;
	ClearCommError(hCom, &dwErrorFlags, &ComStat); // �������״̬
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