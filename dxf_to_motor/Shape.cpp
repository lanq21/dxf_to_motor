#include "COM.h"
#include "dxf_to_txt.h"
#include "Shape.h"
#include <iostream>

const double PI = 3.14159265359;
const int UP = 10;
const int DOWN = 0;

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

bool Shape::between(double x, double y, double x1, double y1, double x2, double y2)
{
	return (x > x1 && x < x2&& y > y1 && y < y2) || (x > x1 && x < x2&& y < y1&& y > y2) || (x < x1&& x > x2 && y > y1 && y < y2) || (x < x1&& x > x2 && y < y1&& y > y2);
}

Point::Point(const DL_PointData& obj) :Shape(point), shape(obj) {}

void Point::Print() const
{
	std::cout << "点:(" << shape.x << ',' << shape.y << ')\n';
}

void Point::Output() const
{
	com.Write(shape.x);
	com.Write(shape.y);
	com.Write(DOWN);

	com.Write(shape.x);
	com.Write(shape.y);
	com.Write(UP);
}

Circle::Circle(const DL_CircleData& obj) :Shape(circle), shape(obj) {}

void Circle::Print() const
{
	std::cout << "圆:圆心(" << shape.cx << ',' << shape.cy << ") 半径" << shape.radius << std::endl;
}

void Circle::Output() const
{
	const int num = 20;

	//float** result = new float* [num];
	//for (int i = 0; i < num; i++) {
	//	result[i] = new float[2];
	//}
	//for (int i = 0; i < num / 2; i++) //将x分成一半
	//{
	//	result[i][0] = shape.cx - shape.radius + 2 * shape.radius * (1.0 / ((num / 2.0) - 1)) * i;
	//	result[i + num / 2][0] = shape.cx - shape.radius + 2 * shape.radius * (1.0 / ((num / 2.0) - 1)) * i;
	//	result[i][1] = sqrt(shape.radius * shape.radius - (result[i][0] - shape.cx) * (result[i][0] - shape.cx)) + shape.cy;
	//	result[i + num / 2][1] = -sqrt(shape.radius * shape.radius - (result[i + num / 2][0] - shape.cx) * (result[i + num / 2][0] - shape.cx)) + shape.cy;
	//}

	double Angle_Path = (2 * PI) / num; // 角度步长
	com.Write(shape.cx + shape.radius);
	com.Write(shape.cy);
	com.Write(UP);
	for (int i = 0; i < num; i++)
	{
		com.Write(shape.cx + shape.radius * cos(i * Angle_Path));
		com.Write(shape.cy + shape.radius * sin(i * Angle_Path));
		com.Write(DOWN);
	}
	com.Write(shape.cx + shape.radius);
	com.Write(shape.cy);
	com.Write(DOWN);

	com.Write(shape.cx + shape.radius);
	com.Write(shape.cy);
	com.Write(UP);
}

Ellipse::Ellipse(const DL_EllipseData& obj) :Shape(ellipse), shape(obj) {}

void Ellipse::Print() const
{
	std::cout << "椭圆:中心(" << shape.cx << ',' << shape.cy << ") 主轴端点(" << shape.mx << ',' << shape.my << ") b/a值" << shape.ratio << " 角度" << shape.angle1 << '~' << shape.angle2 << std::endl;
}

void Ellipse::Output() const
{

}

Line::Line(const double& x1, const double& y1, const double& x2, const double& y2) :Shape(line), x1(x1), y1(y1), x2(x2), y2(y2) {}

void Line::Print() const
{
	std::cout << "线段:从(" << x1 << ',' << y1 << ")到(" << x2 << ',' << y2 << ")\n";
}

void Line::Output() const
{
	com.Write(x1);
	com.Write(y1);
	com.Write(UP);

	com.Write(x1);
	com.Write(y1);
	com.Write(DOWN);

	com.Write(x2);
	com.Write(y2);
	com.Write(DOWN);

	com.Write(x2);
	com.Write(y2);
	com.Write(UP);
}

Arc::Arc(const double& cx, const double& cy, const double& radius, const double& angle1, const double& angle2) :Shape(arc), cx(cx), cy(cy), radius(radius), angle1(angle1), angle2(angle2) {}

void Arc::Print() const
{
	std::cout << "圆弧:圆心(" << cx << ',' << cy << ") 半径" << radius << " 角度范围(deg:0~360)" << angle1 << '~' << angle2 << std::endl;
}

void Arc::Output() const
{
	const int num = 20;

	//float** result = new float* [num];
	//for (int i = 0; i < num; i++) {
	//	result[i] = new float[2];
	//}
	//for (int i = 0; i < num / 2; i++) //将x分成一半
	//{
	//	result[i][0] = cx - radius + 2 * radius * (1.0 / ((num / 2.0) - 1)) * i;
	//	result[i + num / 2][0] = cx - radius + 2 * radius * (1.0 / ((num / 2.0) - 1)) * i;
	//	result[i][1] = sqrt(radius * radius - (result[i][0] - cx) * (result[i][0] - cx)) + cy;
	//	result[i + num / 2][1] = -sqrt(radius * radius - (result[i + num / 2][0] - cx) * (result[i + num / 2][0] - cx)) + cy;
	//}
	//double x1 = cx + radius * cos(angle1);
	//double x2 = cx + radius * cos(angle2);
	//double y1 = cy + radius * sin(angle1);
	//double y2 = cy + radius * sin(angle2);
	//for (int i = 0; i < num; i++) {
	//	if (between(result[i][0], result[i][1], x1, y1, x2, y2) == 0) {
	//		result[i][0] = -1;
	//		result[i][1] = -1;
	//	}
	//}

	com.Write(cx + radius * cos(angle1) / 180.0 * PI);
	com.Write(cy + radius * sin(angle1) / 180.0 * PI);
	com.Write(UP);
	double Angle_Path = (angle2 - angle1) / num; // 角度步长
	for (int i = 0; i < num; i++)
	{
		com.Write(cx + radius * cos((angle1 + i * Angle_Path)) / 180.0 * PI);
		com.Write(cy + radius * sin((angle1 + i * Angle_Path)) / 180.0 * PI);
		com.Write(DOWN);
	}
	com.Write((cx + radius * cos(angle2)) / 180.0 * PI);
	com.Write((cy + radius * sin(angle2)) / 180.0 * PI);
	com.Write(DOWN);

	com.Write((cx + radius * cos(angle2)) / 180.0 * PI);
	com.Write((cy + radius * sin(angle2)) / 180.0 * PI);
	com.Write(UP);
}
