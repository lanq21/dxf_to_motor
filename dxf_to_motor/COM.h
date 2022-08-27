#pragma once
#include <Windows.h>
#undef max
#undef min

class COM
{
public:

	bool Open(); // ����˿ںŴ򿪴���

	bool Read(); // ��ȡ�������ݵ�str

	bool Write(double num);	//���������������

	static void Set_Delay_Time(const int time); // ���÷��ͼ��

	static void Disable_COM(); // ͣ�ô��ڷ���

	~COM();

private:

	HANDLE hCom;
	char Input[100]; // ��ȡ��������
	static int Delay_Time; // ���ͼ��
	static bool Enable; // ʹ�ô��ڷ���

};

extern COM com;