#include "COM.h"
#include "Bridge.h"
#include "Shape.h"
#include <iostream>

int main()
{
	// ��ȡ dxf �ļ���
	std::cout << "�����ļ����� Enter ʹ��Ĭ��ֵ (\"test.dxf\"):";
	std::string filename;
	getline(std::cin,filename);
	if (filename.empty())
		filename = "test.dxf"; // Ĭ���ļ���

	// ��ȡ dxf �ļ�
	Bridge::Reading(filename);
	
/*
	//ͼ�ε���Ӿ�������
	Bridge::Get_Max_X(); // ���ε� X ���ֵ
	Bridge::Get_Min_X(); // ���ε� X ��Сֵ
	Bridge::Get_Max_Y(); // ���ε� Y ���ֵ
	Bridge::Get_Min_Y(); // ���ε� X ��Сֵ
*/

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
		COM::Set_Delay_Time(0);

		// ���ն� COM �� �ı��ļ� "test.txt" ���������Ϣ
		Shape::Output_All();
	}

	// Ҳ���Բ��򿪴��ڣ�ʹ��
	// Shape::Output_All();
	// ������� "test.txt" �� �ն�

	return 0;
}