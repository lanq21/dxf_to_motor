#pragma once
#include "dl_entities.h"
#include <vector>

enum Type { point, line, arc, circle, ellipse };

class Shape
{
public:
	Shape(const Type type_input);
	virtual void Print() const = 0;
	static void Print_All();
private:
	Type type;
	static std::vector<Shape*> Shape_List;
};

class Point : virtual public Shape
{
public:
	Point(const DL_PointData& obj);
	virtual void Print() const;
private:
	const DL_PointData shape;
};

class Line : virtual public Shape
{
public:
	Line(const DL_LineData& obj);
	virtual void Print() const;
private:
	const DL_LineData shape;
};

class Arc : virtual public Shape
{
public:
	Arc(const DL_ArcData& obj);
	virtual void Print() const;
private:
	const DL_ArcData shape;
};

class Circle : virtual public Shape
{
public:
	Circle(const DL_CircleData& obj);
	virtual void Print() const;
private:
	const DL_CircleData shape;
};

class Ellipse : virtual public Shape
{
public:
	Ellipse(const DL_EllipseData& obj);
	virtual void Print() const;
private:
	const DL_EllipseData shape;
};

class Line_Polyline : virtual public Shape
{
public:
	Line_Polyline(const double& x1, const double& y1, const double& x2, const double& y2);
	virtual void Print() const;
private:
	double x1;
	double y1;
	double x2;
	double y2;
};

class Arc_Polyline : virtual public Shape
{
public:
	Arc_Polyline(const double& cx, const double& cy, const double& radius, const double& angle1,const double& angle2);
	virtual void Print() const;
private:
	double cx;
	double cy;
	double radius;
	double angle1;
	double angle2;
};