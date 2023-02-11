#pragma once
#include "dl_entities.h"
#include <vector>

enum Type { point, line, arc, circle, ellipse };

class Shape
{
public:
	Shape(const Type type_input);
	virtual void Print() const = 0;

	// 通过串口输出给Arduino，同时输出到txt，在终端打印
	virtual void Output() = 0;

	static void Set_Z_Up(const int z = 10);
	static void Set_Z_Down(const int z = 0);
	static void Set_Division(const int num = 20);
	static void Set_Pan_X(const double num);
	static void Set_Pan_Y(const double num);
	static void Set_Zoom(const double num);
	static void Output_All();
protected:
	static int UP;
	static int DOWN;
	static int Division;
	static double Pan_X;
	static double Pan_Y;
	static double Zoom;
	static double Last_X;
	static double Last_Y;
private:
	Type type;
	static std::vector<Shape*> Shape_List;
};

class Point : virtual public Shape
{
public:
	Point(const double& x,const double& y);
	virtual void Print() const;
	virtual void Output();
private:
	double x;
	double y;
};

class Circle : virtual public Shape
{
public:
	Circle(const DL_CircleData& obj);
	virtual void Print() const;
	virtual void Output();
private:
	DL_CircleData shape;
};

class Ellipse : virtual public Shape
{
public:
	Ellipse(const DL_EllipseData& obj);
	virtual void Print() const;
	virtual void Output();
private:
	DL_EllipseData shape;
};

class Line : virtual public Shape
{
public:
	Line(const double& _x1, const double& _y1, const double& _x2, const double& _y2);
	virtual void Print() const;
	virtual void Output();
private:
	double x1; // 起点x坐标
	double y1; // 起点y坐标
	double x2; // 终点x坐标
	double y2; // 终点x坐标
};

class Arc : virtual public Shape
{
public:
	Arc(const double& _cx, const double& _cy, const double& radius, const double& angle1, const double& angle2);
	virtual void Print() const;
	virtual void Output();
private:
	double cx; // 圆心x坐标
	double cy; // 圆心y坐标
	double radius; // 半径
	double angle1; // 使用角度degree
	double angle2; // 使用角度degree
};

extern std::ofstream file_out; // 输出txt文件