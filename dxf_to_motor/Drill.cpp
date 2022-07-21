#include "Drill.h"

double Drill::Origin_x = 0;
double Drill::Origin_y = 0;

Drill::Drill(const double& x, const double& y) :Drill_Down(false), Drill_On(false), max_x(x), max_y(y) {}

void Drill::Print_Range()
{
	Move_To(Origin_x, Origin_y);
	Move_To(max_x, Origin_y);
	Move_To(max_x, max_y);
	Move_To(Origin_x, max_y);
	Move_To(Origin_x, Origin_y);
}

void Drill::Raise_Drill()
{
	if (Drill_Down)
	{
		//̧����ͷ
	}
	Drill_Down = false;
}

void Drill::Lower_Drill()
{
	if (!Drill_Down)
	{
		//������ͷ
	}
	Drill_Down = true;
}

void Drill::Run_Drill()
{
	if (!Drill_On)
	{
		//��ͷ����
	}
	Drill_On = true;
}

void Drill::Stop_Drill()
{
	if (Drill_On)
	{
		//��ͷͣ��
	}
	Drill_On = false;
}

void Drill::Move_To(const double& x, const double& y)
{
	if (Drill_Down) Raise_Drill();
	if (Drill_On) Stop_Drill();

	//��ͷ�ƶ���(x,y),��ͷ������

}