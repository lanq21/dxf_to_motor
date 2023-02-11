#include "COM.h"
#include "Bridge.h"
#include "Shape.h"
#include <iostream>
#include <math.h>

template<typename T>
constexpr auto Eq(T x, T y)
{
	return x - y<0.01 && x - y>-0.01;
}

const double PI = 3.14159265359;
int Shape::UP = 10; // ̧��ʱz��߶�
int Shape::DOWN = 0; // ����ʱz��߶�
int Shape::Division = 20; // �����Ϸָ���
double Shape::Pan_X = 0; // X λ��
double Shape::Pan_Y = 0; // Y λ��
double Shape::Zoom = 1; // ����
double Shape::Last_X = 0;
double Shape::Last_Y = 0;

std::ofstream file_out;
std::vector<Shape*> Shape::Shape_List;

Shape::Shape(const Type type_input) :type(type_input)
{
	Shape_List.push_back(this);
}

void Shape::Set_Z_Up(const int z)
{
	UP = z;
}

void Shape::Set_Z_Down(const int z)
{
	DOWN = z;
}

void Shape::Set_Division(const int num)
{
	Division = num;
}

void Shape::Set_Pan_X(const double num)
{
	Pan_X = num;
}

void Shape::Set_Pan_Y(const double num)
{
	Pan_Y = num;
}

void Shape::Set_Zoom(const double num)
{
	Zoom = num;
}

void Shape::Output_All()
{
	// ͼ����Ӿ�������
	com.Write(((Bridge::Get_Min_X() + Pan_X) * Zoom));
	com.Write(((Bridge::Get_Min_Y() + Pan_Y) * Zoom));
	com.Write(UP);

	com.Write(((Bridge::Get_Max_X() + Pan_X) * Zoom));
	com.Write(((Bridge::Get_Min_Y() + Pan_Y) * Zoom));
	com.Write(UP);

	com.Write(((Bridge::Get_Max_X() + Pan_X) * Zoom));
	com.Write(((Bridge::Get_Max_Y() + Pan_Y) * Zoom));
	com.Write(UP);

	com.Write(((Bridge::Get_Min_X() + Pan_X) * Zoom));
	com.Write(((Bridge::Get_Max_Y() + Pan_Y) * Zoom));
	com.Write(UP);

	com.Write(((Bridge::Get_Min_X() + Pan_X) * Zoom));
	com.Write(((Bridge::Get_Min_Y() + Pan_Y) * Zoom));
	com.Write(UP);

	for (auto iter = Shape_List.begin(); iter != Shape_List.end(); iter++)
	{
		(*iter)->Output();
		(*iter)->Print();
	}

	com.Write(Last_X);
	com.Write(Last_Y);
	com.Write(UP);
}

Point::Point(const double& x,const double& y) :Shape(point), x(x),y(y) {}

void Point::Print() const
{
	std::cout << "��:(" << x << ',' << y << ")\n";
}

void Point::Output()
{
	x = (x + Pan_X) * Zoom;
	y = (y + Pan_Y) * Zoom;
	if (!Eq(Last_X, x) || !Eq(Last_Y, y))
	{
		com.Write(Last_X);
		com.Write(Last_Y);
		com.Write(UP);

		com.Write(x);
		com.Write(y);
		com.Write(UP);

		com.Write(x);
		com.Write(y);
		com.Write(DOWN);

		Last_X = x;
		Last_Y = y;
	}
}

Circle::Circle(const DL_CircleData& obj) :Shape(circle), shape(obj) {}

void Circle::Print() const
{
	std::cout << "Բ:Բ��(" << shape.cx << ',' << shape.cy << ") �뾶" << shape.radius << std::endl;
}

void Circle::Output()
{
	double Angle_Path = (2 * PI) / Division; // �ǶȲ���

	shape.cx = (shape.cx + Pan_X) * Zoom;
	shape.cy = (shape.cy + Pan_Y) * Zoom;
	shape.radius *= Zoom;

	// �ƶ������
	com.Write(Last_X);
	com.Write(Last_Y);
	com.Write(UP);

	com.Write(shape.cx + shape.radius);
	com.Write(shape.cy);
	com.Write(UP);

	double x, y;
	for (int i = 0; i <= Division; i++)
	{
		x = shape.cx + shape.radius * cos(i * Angle_Path);
		y = shape.cy + shape.radius * sin(i * Angle_Path);
		com.Write(x);
		com.Write(y);
		com.Write(DOWN);
	}
	Last_X = shape.cx + shape.radius;
	Last_Y = shape.cy;
}

Ellipse::Ellipse(const DL_EllipseData& obj) :Shape(ellipse), shape(obj) {}

void Ellipse::Print() const
{
	std::cout << "��Բ:����(" << shape.cx << ',' << shape.cy << ") ����˵�(" << shape.mx << ',' << shape.my << ") b/aֵ" << shape.ratio << " �Ƕ�" << shape.angle1 << '~' << shape.angle2 << std::endl;
}

void Ellipse::Output()
{
	shape.cx = (shape.cx + Pan_X) * Zoom;
	shape.cy = (shape.cy + Pan_Y) * Zoom;
	shape.mx *= Zoom;
	shape.my *= Zoom;

	double Theta0 = atan2(shape.my, shape.mx); // ��Բ��ת��
	double Cos0 = cos(Theta0);
	double Sin0 = sin(Theta0);
	double a = sqrt(shape.mx * shape.mx + shape.my * shape.my); // ��Բ�볤��
	double b = shape.ratio * a; // ��Բ�����
	double Angle_Path = (shape.angle2 - shape.angle1) / Division; // �ǶȲ���

	com.Write(Last_X);
	com.Write(Last_Y);
	com.Write(UP);

	// �ƶ������
	com.Write(a * cos(shape.angle1) * Cos0 + b * sin(shape.angle1) * Sin0 + shape.cx);
	com.Write(b * sin(shape.angle1) * Cos0 - a * cos(shape.angle1) * Sin0 + shape.cy);
	com.Write(UP);

	double x, y;
	for (int i = 0; i <= Division; i++)
	{
		x = a * cos(shape.angle1 + i * Angle_Path) * Cos0 + b * sin(shape.angle1 + i * Angle_Path) * Sin0 + shape.cx;
		y = b * sin(shape.angle1 + i * Angle_Path) * Cos0 - a * cos(shape.angle1 + i * Angle_Path) * Sin0 + shape.cy;
		com.Write(x);
		com.Write(y);
		com.Write(DOWN);
	}

	Last_X = a * cos(shape.angle2) * Cos0 + b * sin(shape.angle2) * Sin0 + shape.cx;
	Last_Y = b * sin(shape.angle2) * Cos0 - a * cos(shape.angle2) * Sin0 + shape.cy;
}

Line::Line(const double& _x1, const double& _y1, const double& _x2, const double& _y2) :Shape(line), x1(_x1), x2(_x2), y1(_y1), y2(_y2) {}

void Line::Print() const
{
	std::cout << "�߶�:��(" << x1 << ',' << y1 << ")��(" << x2 << ',' << y2 << ")\n";
}

void Line::Output()
{
	x1 = (x1 + Pan_X) * Zoom;
	y1 = (y1 + Pan_Y) * Zoom;
	x2 = (x2 + Pan_X) * Zoom;
	y2 = (y2 + Pan_Y) * Zoom;

	if (Eq(Last_X, x1) && Eq(Last_Y, y1))
	{
		com.Write(x2);
		com.Write(y2);
		com.Write(DOWN);

		Last_X = x2;
		Last_Y = y2;
	}
	else if (Eq(Last_X, x2) && Eq(Last_Y, y2))
	{
		com.Write(x1);
		com.Write(y1);
		com.Write(DOWN);

		Last_X = x1;
		Last_Y = y1;
	}
	else
	{
		com.Write(Last_X);
		com.Write(Last_Y);
		com.Write(UP);

		com.Write(x1);
		com.Write(y1);
		com.Write(UP);

		com.Write(x1);
		com.Write(y1);
		com.Write(DOWN);

		com.Write(x2);
		com.Write(y2);
		com.Write(DOWN);

		Last_X = x2;
		Last_Y = y2;
	}
}

Arc::Arc(const double& _cx, const double& _cy, const double& radius, const double& angle1, const double& angle2) :Shape(arc), cx(_cx), cy(_cy), radius(radius), angle1(angle1), angle2(angle2) {}

void Arc::Print() const
{
	std::cout << "Բ��:Բ��(" << cx << ',' << cy << ") �뾶" << radius << " �Ƕȷ�Χ(deg:0~360)" << angle1 << '~' << angle2 << std::endl;
}

void Arc::Output()
{
	if (angle1 > angle2) angle2 += 360;
	double Angle_Path = (angle2 - angle1) / Division; // �ǶȲ���

	cx = (cx + Pan_X) * Zoom;
	cy = (cy + Pan_Y) * Zoom;
	radius *= Zoom;

	// �ƶ������

	if (Eq(Last_X, cx + radius * cos(angle1 / 180.0 * PI)) && Eq(Last_Y, cy + radius * sin(angle1 / 180.0 * PI)))
	{
		double x, y;
		for (int i = 1; i <= Division; i++)
		{
			x = cx + radius * cos((angle1 + i * Angle_Path) / 180.0 * PI);
			y = cy + radius * sin((angle1 + i * Angle_Path) / 180.0 * PI);
			com.Write(x);
			com.Write(y);
			com.Write(DOWN);
		}
		Last_X = cx + radius * cos(angle2 / 180.0 * PI);
		Last_Y = cy + radius * sin(angle2 / 180.0 * PI);
	}
	else if (Eq(Last_X, cx + radius * cos(angle2 / 180.0 * PI)) && Eq(Last_Y, cy + radius * sin(angle2 / 180.0 * PI)))
	{
		double x, y;
		for (int i = 1; i <= Division; i++)
		{
			x = cx + radius * cos((angle2 - i * Angle_Path) / 180.0 * PI);
			y = cy + radius * sin((angle2 - i * Angle_Path) / 180.0 * PI);
			com.Write(x);
			com.Write(y);
			com.Write(DOWN);
		}
		Last_X = cx + radius * cos(angle1 / 180.0 * PI);
		Last_Y = cy + radius * sin(angle1 / 180.0 * PI);
	}
	else
	{
		com.Write(Last_X);
		com.Write(Last_Y);
		com.Write(UP);

		com.Write(cx + radius * cos(angle1 / 180.0 * PI));
		com.Write(cy + radius * sin(angle1 / 180.0 * PI));
		com.Write(UP);

		double x, y;
		for (int i = 0; i <= Division; i++)
		{
			x = cx + radius * cos((angle1 + i * Angle_Path) / 180.0 * PI);
			y = cy + radius * sin((angle1 + i * Angle_Path) / 180.0 * PI);
			com.Write(x);
			com.Write(y);
			com.Write(DOWN);
		}
		Last_X = cx + radius * cos(angle2 / 180.0 * PI);
		Last_Y = cy + radius * sin(angle2 / 180.0 * PI);
	}
}
