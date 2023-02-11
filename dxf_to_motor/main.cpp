#include "COM.h"
#include "Bridge.h"
#include "Shape.h"
#include <iostream>

int main()
{
	// ��ȡ dxf �ļ���
	std::cout << "�����ļ����� Enter ʹ��Ĭ��ֵ (\"test.dxf\"):";
	std::string filename;
	getline(std::cin, filename);
	if (filename.empty())
		filename = "test.dxf"; // Ĭ���ļ���

	// ��ȡ dxf �ļ�
	if (Bridge::Reading(filename))
	{
		// ��ͷ̧��� z ����
		Shape::Set_Z_Up(10);

		// ��ͷ���µ� z ����
		Shape::Set_Z_Down(0);

		// ���û����ϵķָ�����
		Shape::Set_Division(20);

		// ������Ϣ����д�뵽�ı��ļ� "test.txt"
		file_out.open("test.txt", std::ios_base::trunc);

		// �򿪴���
		if (com.Open())
		{
			// ���ڷ��ͼ������λ ms �����ڷ��ͱ������ٶ����ƣ�����ܿ죩
			COM::Set_Delay_Time(1500);
		}

		// ���ն� COM �� �ı��ļ� "test.txt" ���������Ϣ
		// Ҳ���Բ��򿪴��ڣ�������� "test.txt" �� �ն�
		Shape::Output_All();
	}

	return 0;
}