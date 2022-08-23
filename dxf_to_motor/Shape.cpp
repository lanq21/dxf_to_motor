#include "COM.h"
#include "Bridge.h"
#include "Shape.h"
#include <iostream>
#include <math.h>

const double PI = 3.14159265359;
const int UP = 10; // ̧��ʱz��߶�
const int DOWN = 0; // ����ʱz��߶�

std::ofstream file_out;
std::vector<Shape*> Shape::Shape_List;

Shape::Shape(const Type type_input) :type(type_input)
{
	Shape_List.push_back(this);
}

void Shape::Print_All()
{
	for (auto iter = Shape_List.begin(); iter != Shape_List.end(); iter++)
		(*iter)->Print();
}

void Shape::Output_All()
{
	for (auto iter = Shape_List.begin(); iter != Shape_List.end(); iter++)
		(*iter)->Output();
}

Point::Point(const DL_PointData& obj) :Shape(point), shape(obj) {}

void Point::Print() const
{
	std::cout << "��:(" << shape.x << ',' << shape.y << ")\n";
}

void Point::Output() const
{
	// �ƶ�����
	com.Write(shape.x);
	com.Write(shape.y);
	com.Write(UP);

	com.Write(shape.x);
	com.Write(shape.y);
	com.Write(DOWN);

	// ̧��
	com.Write(shape.x);
	com.Write(shape.y);
	com.Write(UP);
}

Circle::Circle(const DL_CircleData& obj) :Shape(circle), shape(obj) {}

void Circle::Print() const
{
	std::cout << "Բ:Բ��(" << shape.cx << ',' << shape.cy << ") �뾶" << shape.radius << std::endl;
}

void Circle::Output() const
{
	const int num = 20; // �ָ������

	double Angle_Path = (2 * PI) / num; // �ǶȲ���

	// �ƶ������
	com.Write(shape.cx + shape.radius);
	com.Write(shape.cy);
	com.Write(UP);

	double x, y;
	for (int i = 0; i <= num; i++)
	{
		x = shape.cx + shape.radius * cos(i * Angle_Path);
		y = shape.cy + shape.radius * sin(i * Angle_Path);
		com.Write(x);
		com.Write(y);
		com.Write(DOWN);
	}

	// ̧��
	com.Write(shape.cx + shape.radius);
	com.Write(shape.cy);
	com.Write(UP);
}

Ellipse::Ellipse(const DL_EllipseData& obj) :Shape(ellipse), shape(obj) {}

void Ellipse::Print() const
{
	std::cout << "��Բ:����(" << shape.cx << ',' << shape.cy << ") ����˵�(" << shape.mx << ',' << shape.my << ") b/aֵ" << shape.ratio << " �Ƕ�" << shape.angle1 << '~' << shape.angle2 << std::endl;
}

void Ellipse::Output() const
{
	const int num = 20; // �ָ������

	double Theta0 = atan2(shape.my, shape.mx); // ��Բ��ת��
	double Cos0 = cos(Theta0);
	double Sin0 = sin(Theta0);
	double a = sqrt(shape.mx * shape.mx + shape.my * shape.my); // ��Բ�볤��
	double b = shape.ratio * a; // ��Բ�����
	double Angle_Path = (shape.angle2 - shape.angle1) / num; // �ǶȲ���

	// �ƶ������
	com.Write(a * cos(shape.angle1) * Cos0 + b * sin(shape.angle1) * Sin0);
	com.Write(b * sin(shape.angle1) * Cos0 - a * cos(shape.angle1) * Sin0);
	com.Write(UP);

	double x, y;
	for (int i = 0; i <= num; i++)
	{
		x = a * cos(shape.angle1 + i * Angle_Path) * Cos0 + b * sin(shape.angle1 + i * Angle_Path) * Sin0;
		y = b * sin(shape.angle1 + i * Angle_Path) * Cos0 - a * cos(shape.angle1 + i * Angle_Path) * Sin0;
		com.Write(x);
		com.Write(y);
		com.Write(DOWN);
	}

	// ̧��
	com.Write(a * cos(shape.angle2) * Cos0 + b * sin(shape.angle2) * Sin0);
	com.Write(b * sin(shape.angle2) * Cos0 - a * cos(shape.angle2) * Sin0);
	com.Write(UP);
}

Line::Line(const double& x1, const double& y1, const double& x2, const double& y2) :Shape(line), x1(x1), y1(y1), x2(x2), y2(y2) {}

void Line::Print() const
{
	std::cout << "�߶�:��(" << x1 << ',' << y1 << ")��(" << x2 << ',' << y2 << ")\n";
}

void Line::Output() const
{
	// �ƶ������
	com.Write(x1);
	com.Write(y1);
	com.Write(UP);

	com.Write(x1);
	com.Write(y1);
	com.Write(DOWN);

	com.Write(x2);
	com.Write(y2);
	com.Write(DOWN);

	// ̧��
	com.Write(x2);
	com.Write(y2);
	com.Write(UP);
}

Arc::Arc(const double& cx, const double& cy, const double& radius, const double& angle1, const double& angle2) :Shape(arc), cx(cx), cy(cy), radius(radius), angle1(angle1), angle2(angle2) {}

void Arc::Print() const
{
	std::cout << "Բ��:Բ��(" << cx << ',' << cy << ") �뾶" << radius << " �Ƕȷ�Χ(deg:0~360)" << angle1 << '~' << angle2 << std::endl;
}

void Arc::Output() const
{
	const int num = 20; // �ָ�����

	double Angle_Path = (angle2 - angle1) / num; // �ǶȲ���

	// �ƶ������
	com.Write(cx + radius * cos(angle1) / 180.0 * PI);
	com.Write(cy + radius * sin(angle1) / 180.0 * PI);
	com.Write(UP);

	double x, y;
	for (int i = 0; i <= num; i++)
	{
		x = cx + radius * cos((angle1 + i * Angle_Path)) / 180.0 * PI;
		y = cy + radius * sin((angle1 + i * Angle_Path)) / 180.0 * PI;
		com.Write(x);
		com.Write(y);
		com.Write(DOWN);
	}

	// ̧��
	com.Write((cx + radius * cos(angle2)) / 180.0 * PI);
	com.Write((cy + radius * sin(angle2)) / 180.0 * PI);
	com.Write(UP);
}
