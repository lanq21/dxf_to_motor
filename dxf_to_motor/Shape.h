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
	virtual void Output() const = 0;

	static void Set_Z_Up(const int z = 10);
	static void Set_Z_Down(const int z = 0);
	static void Output_All();
protected:
	static int UP;
	static int DOWN;
private:
	Type type;
	static std::vector<Shape*> Shape_List;
};

class Point : virtual public Shape
{
public:
	Point(const DL_PointData& obj);
	virtual void Print() const;
	virtual void Output() const;
private:
	const DL_PointData shape;
};

class Circle : virtual public Shape
{
public:
	Circle(const DL_CircleData& obj);
	virtual void Print() const;
	virtual void Output() const;
private:
	const DL_CircleData shape;
};

class Ellipse : virtual public Shape
{
public:
	Ellipse(const DL_EllipseData& obj);
	virtual void Print() const;
	virtual void Output() const;
private:
	const DL_EllipseData shape;
};

class Line : virtual public Shape
{
public:
	Line(const double& x1, const double& y1, const double& x2, const double& y2);
	virtual void Print() const;
	virtual void Output() const;
private:
	double x1; // 起点x坐标
	double y1; // 起点y坐标
	double x2; // 终点x坐标
	double y2; // 终点x坐标
};

class Arc : virtual public Shape
{
public:
	Arc(const double& cx, const double& cy, const double& radius, const double& angle1, const double& angle2);
	virtual void Print() const;
	virtual void Output() const;
private:
	double cx; // 圆心x坐标
	double cy; // 圆心y坐标
	double radius; // 半径
	double angle1; // 使用角度degree
	double angle2; // 使用角度degree
};

extern std::ofstream file_out; // 输出txt文件