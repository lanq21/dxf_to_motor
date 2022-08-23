#pragma once
#include "dl_entities.h"
#include <vector>

enum Type { point, line, arc, circle, ellipse };

class Shape
{
public:
	Shape(const Type type_input);
	virtual void Print() const = 0;
	virtual void Output() const = 0;
	static void Print_All();
	static void Output_All();
protected:
	static bool between(double x, double y, double x1, double y1, double x2, double y2);
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
	double x1;
	double y1;
	double x2;
	double y2;
};

class Arc : virtual public Shape
{
public:
	Arc(const double& cx, const double& cy, const double& radius, const double& angle1,const double& angle2);
	virtual void Print() const;
	virtual void Output() const;
private:
	double cx;
	double cy;
	double radius;
	// 使用角度制
	double angle1;
	double angle2;
};