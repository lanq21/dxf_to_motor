/*
	Open(),Read(),Write()�ο���
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
	std::cout << "����Ϊ��(�������֣���ֱ�Ӱ� Enter ��������ı��ĵ�)\tCOM";
	std::wstring Name;
	getline(std::wcin, Name);
	if (Name.empty())
		return false;
	else if (std::stoi(Name) < 10)
		Name = L"COM" + Name;
	else
		Name = L"\\\\.\\COM" + Name;

	//���ھ��
	hCom = CreateFile(Name.c_str(), GENERIC_READ | GENERIC_WRITE/*�������д*/, 0/*��ռ��ʽ*/, NULL, OPEN_EXISTING/*�򿪶����Ǵ���*/, 0/*ͬ����ʽ*/, NULL);

	if (hCom == INVALID_HANDLE_VALUE) return false;
	else
	{
		if (SetupComm(hCom, 64, 64) == false) return false; //���뻺����������������Ĵ�С����1024

		COMMTIMEOUTS TimeOuts{};
		//�趨����ʱ
		TimeOuts.ReadIntervalTimeout = 1000;
		TimeOuts.ReadTotalTimeoutMultiplier = 500;
		TimeOuts.ReadTotalTimeoutConstant = 5000;
		//�趨д��ʱ
		TimeOuts.WriteTotalTimeoutMultiplier = 500;
		TimeOuts.WriteTotalTimeoutConstant = 2000;
		if (SetCommTimeouts(hCom, &TimeOuts) == false) return false; //���ó�ʱ

		DCB dcb{};
		dcb.BaudRate = 9600; //������Ϊ9600
		dcb.ByteSize = 8; //ÿ���ֽ���8λ
		dcb.Parity = NOPARITY; //����żУ��λ
		dcb.StopBits = ONESTOPBIT; //1��ֹͣλ
		if (SetCommState(hCom, &dcb) == false) return false;
		if (PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR) == false) return false;
		COM::Enable = true;
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
	static int i = 1;
//	if (i == 1)
//		file_out << num << ',';
	if (i == 2)
		file_out << num << ',';
	i++;
	if (i == 4) i = 1;
	if (Enable)
	{
		std::string OutString = std::to_string(int(num + 0.5)); // ����ת�ַ���
		DWORD dwBytesWrite = 100;
		COMSTAT ComStat; // �˿�״̬
		DWORD dwErrorFlags;
		ClearCommError(hCom, &dwErrorFlags, &ComStat); // �������״̬

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