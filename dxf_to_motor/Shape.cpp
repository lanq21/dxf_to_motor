#include "Shape.h"
#include <iostream>

std::vector<Shape*> Shape::Shape_List;

Shape::Shape(const Type type_input) :type(type_input)
{
	Shape_List.push_back(this);
}

void Shape::Print_All()
{
	for (int i = 0; i < Shape_List.size(); i++)
		Shape_List[i]->Print();
}

Point::Point(const DL_PointData& obj) :Shape(point), shape(obj) {}

void Point::Print() const
{
	std::cout << "��:(" << shape.x << ',' << shape.y << ')\n';
}

Line::Line(const DL_LineData& obj) :Shape(line), shape(obj) {}

void Line::Print() const
{
	std::cout << "�߶�:��(" << shape.x1 << ',' << shape.y1 << ")��(" << shape.x2 << ',' << shape.y2 << ")\n";
}

Arc::Arc(const DL_ArcData& obj) :Shape(arc), shape(obj) {}

void Arc::Print() const
{
	std::cout << "Բ��:Բ��(" << shape.cx << ',' << shape.cy << ") �뾶" << shape.radius << " �Ƕȷ�Χ(deg:0~360)" << shape.angle1 << '~' << shape.angle2 << std::endl;
}

Circle::Circle(const DL_CircleData& obj) :Shape(circle), shape(obj) {}

void Circle::Print() const
{
	std::cout << "Բ:Բ��(" << shape.cx << ',' << shape.cy << ") �뾶" << shape.radius << std::endl;
}

Ellipse::Ellipse(const DL_EllipseData& obj) :Shape(ellipse), shape(obj) {}

void Ellipse::Print() const
{
	std::cout << "��Բ:����(" << shape.cx << ',' << shape.cy << ") ����˵�(" << shape.mx << ',' << shape.my << ") b/aֵ" << shape.ratio << " �Ƕ�" << shape.angle1 << '~' << shape.angle2 << std::endl;
}

Line_Polyline::Line_Polyline(const double& x1, const double& y1, const double& x2, const double& y2) :Shape(line), x1(x1), y1(y1), x2(x2), y2(y2) {}

void Line_Polyline::Print() const
{
	std::cout << "�߶�:��(" << x1 << ',' << y1 << ")��(" << x2 << ',' << y2 << ")\n";
}

Arc_Polyline::Arc_Polyline(const double& cx, const double& cy, const double& radius, const double& angle1, const double& angle2) :Shape(arc), cx(cx), cy(cy), radius(radius), angle1(angle1), angle2(angle2) {}

void Arc_Polyline::Print() const
{
	std::cout << "Բ��:Բ��(" << cx << ',' << cy << ") �뾶" << radius << " �Ƕȷ�Χ(deg:0~360)" << angle1 << '~' << angle2 << std::endl;
}