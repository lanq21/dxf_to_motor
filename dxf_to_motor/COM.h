#pragma once
#include <Windows.h>
#undef max
#undef min

class COM
{
public:

	COM();

	bool Open(const int num); // ����˿ںŴ򿪴���

	bool Read(); // ��ȡ�������ݵ�str

	bool Write(double num);	//���������������

	~COM();

private:

	HANDLE hCom;
	char Input[100]; // ��ȡ��������

};

extern COM com;