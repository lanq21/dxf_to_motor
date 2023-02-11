#include "Bridge.h"
#include "Shape.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include "dl_dxf.h"

double Bridge::max_x = DBL_MIN;
double Bridge::min_x = DBL_MAX;
double Bridge::max_y = DBL_MIN;
double Bridge::min_y = DBL_MAX;
double Bridge::X_Limit_N = -50;
double Bridge::X_Limit_P = 50;
double Bridge::Y_Limit_N = -50;
double Bridge::Y_Limit_P = 50;
bool Bridge::New_PolyLine;
bool Bridge::Is_Closed = false;
int Bridge::Vertex_Num = 0;

double Bridge::Get_Max_X()
{
	return max_x;
}

double Bridge::Get_Min_X()
{
	return min_x;
}

double Bridge::Get_Max_Y()
{
	return max_y;
}

double Bridge::Get_Min_Y()
{
	return min_y;
}

void Bridge::addPoint(const DL_PointData& data)
{
	// �����б�
	new Point(data.x,data.y);
}

void Bridge::addLine(const DL_LineData& data)
{
	max_x = std::max({ max_x, data.x1, data.x2 });
	min_x = std::min({ min_x, data.x1, data.x2 });
	max_y = std::max({ max_y, data.y1, data.y2 });
	min_y = std::min({ min_y, data.y1, data.y2 });

	new Line(data.x1, data.y1, data.x2, data.y2);
}

void Bridge::addArc(const DL_ArcData& data)
{
	// �Ҳ��Ե��
	double Angle_1 = data.angle1;
	double Angle_2 = data.angle2;
	if (Angle_1 > Angle_2) max_x = std::max(max_x, data.cx + data.radius);
	else max_x = std::max({ max_x,data.cx + data.radius * cos(data.angle1 / 180.0 * M_PI),data.cx + data.radius * cos(data.angle2 / 180.0 * M_PI) });

	// ����Ե��
	Angle_1 = (data.angle1 > 180) ? (data.angle1 - 180) : (data.angle1 + 180);
	Angle_2 = (data.angle2 > 180) ? (data.angle2 - 180) : (data.angle2 + 180);
	if (Angle_1 > Angle_2) min_x = std::min({ min_x,data.cx - data.radius });
	else min_x = std::min({ min_x,data.cx + data.radius * cos(data.angle1 / 180.0 * M_PI),data.cx + data.radius * cos(data.angle2 / 180.0 * M_PI) });

	// �ϲ��Ե��
	Angle_1 = (data.angle1 > 90) ? (data.angle1 - 90) : (data.angle1 + 270);
	Angle_2 = (data.angle2 > 90) ? (data.angle2 - 90) : (data.angle2 + 270);
	if (Angle_1 > Angle_2) max_y = std::max({ max_y,data.cy + data.radius });
	else max_y = std::max({ max_y,data.cy + data.radius * sin(data.angle1 / 180.0 * M_PI),data.cy + data.radius * sin(data.angle2 / 180.0 * M_PI) });

	// �²��Ե��
	Angle_1 = (data.angle1 > 270) ? (data.angle1 - 270) : (data.angle1 + 90);
	Angle_2 = (data.angle2 > 270) ? (data.angle2 - 270) : (data.angle2 + 90);
	if (Angle_1 > Angle_2) min_y = std::min({ min_y,data.cy - data.radius });
	else min_y = std::min({ min_y,data.cy + data.radius * sin(data.angle1 / 180.0 * M_PI),data.cy + data.radius * sin(data.angle2 / 180.0 * M_PI) });

	new Arc(data.cx, data.cy, data.radius, data.angle1, data.angle2);
}

void Bridge::addCircle(const DL_CircleData& data)
{
	max_x = std::max({ max_x,data.cx + data.radius });
	min_x = std::min({ min_x,data.cx - data.radius });
	max_y = std::max({ max_y,data.cy + data.radius });
	min_y = std::min({ min_y,data.cy - data.radius });

	new Circle(data);
}

void Bridge::addPolyline(const DL_PolylineData& data)
{
	New_PolyLine = true;
	Is_Closed = data.flags;
	Vertex_Num = data.number;
}

void Bridge::addVertex(const DL_VertexData& data)
{
	static double First_X = 0;
	static double First_Y = 0;
	static double x;
	static double y;
	static double bulge = 0;

	static int i;

	if (New_PolyLine)
	{
		i = 0;
		New_PolyLine = false;
		First_X = data.x;
		First_Y = data.y;
		new Point(data.x,data.y);
	}
	else if (bulge)
	{
		double mu = (1 - bulge * bulge) / (2 * bulge);
		double L = sqrt((data.x - x) * (data.x - x) + (data.y - y) * (data.y - y));
		double radius = L * (bulge + 1.0 / bulge) / 4.0;
		if (radius < 0) radius = -radius;
		double cx = ((x + data.x) - (data.y - y) * mu) / 2.0;
		double cy = ((y + data.y) + (data.x - x) * mu) / 2.0;

		double angle1 = atan2(y - cy, x - cx);
		angle1 = angle1 / M_PI * 180;
		if (angle1 < 0) angle1 = angle1 + 360;
		double angle2 = atan2(data.y - cy, data.x - cx);
		angle2 = angle2 / M_PI * 180;
		if (angle2 < 0) angle2 = angle2 + 360;
		if (bulge < 0) // ������ת����
		{
			double angle = angle1;
			angle1 = angle2;
			angle2 = angle;
		}

		// �Ҳ��Ե��
		double Angle_1 = angle1;
		double Angle_2 = angle2;
		if (Angle_1 > Angle_2) max_x = std::max(max_x, cx + radius);
		else max_x = std::max({ max_x,cx + radius * cos(angle1 / 180.0 * M_PI),cx + radius * cos(angle2 / 180.0 * M_PI) });

		// ����Ե��
		Angle_1 = (angle1 > 180) ? (angle1 - 180) : (angle1 + 180);
		Angle_2 = (angle2 > 180) ? (angle2 - 180) : (angle2 + 180);
		if (Angle_1 > Angle_2) min_x = std::min({ min_x,cx - radius });
		else min_x = std::min({ min_x,cx + radius * cos(angle1 / 180.0 * M_PI),cx + radius * cos(angle2 / 180.0 * M_PI) });

		// �ϲ��Ե��
		Angle_1 = (angle1 > 90) ? (angle1 - 90) : (angle1 + 270);
		Angle_2 = (angle2 > 90) ? (angle2 - 90) : (angle2 + 270);
		if (Angle_1 > Angle_2) max_y = std::max({ max_y,cy + radius });
		else max_y = std::max({ max_y,cy + radius * sin(angle1 / 180.0 * M_PI),cy + radius * sin(angle2 / 180.0 * M_PI) });

		// �²��Ե��
		Angle_1 = (angle1 > 270) ? (angle1 - 270) : (angle1 + 90);
		Angle_2 = (angle2 > 270) ? (angle2 - 270) : (angle2 + 90);
		if (Angle_1 > Angle_2) min_y = std::min({ min_y,cy - radius });
		else min_y = std::min({ min_y,cy + radius * sin(angle1 / 180.0 * M_PI),cy + radius * sin(angle2 / 180.0 * M_PI) });

		new Arc(cx, cy, radius, angle1, angle2);
	}
	else
	{
		max_x = std::max({ max_x, x, data.x });
		min_x = std::min({ min_x, x, data.x });
		max_y = std::max({ max_y, y, data.y });
		min_y = std::min({ min_y, y, data.y });

		new Line(x, y, data.x, data.y);
	}

	x = data.x;
	y = data.y;
	bulge = data.bulge; // ͹��

	// ͹��!=0����õ�����һ��֮������Բ��
	// Բ���뾶 radius=L*(W+1.0/W)/4.0������LΪ��������
	// Բ��	x=((x_1+x_2)-(y_2-y_1)*mu)/2.0
	//		y=((y_1+y_2)+(x_2-x_1)*mu)/2.0
	//		����mu=(1-W*W)/2*W

	i++;
	if (i == Vertex_Num && Is_Closed)
		new Line(data.x, data.y, First_X, First_Y);
}

void Bridge::addEllipse(const DL_EllipseData& data)
{
	new Ellipse(data);

	// ���Գ���Ϊ�뾶��Բ������Բ
	double radius = sqrt(data.mx * data.mx + data.my * data.my);
	double angle1 = data.angle1 / M_PI * 180;
	double angle2 = data.angle2 / M_PI * 180;
	double Angle_1 = angle1;
	double Angle_2 = angle2;
	if (Angle_1 > Angle_2) max_x = std::max(max_x, data.cx + radius);
	else max_x = std::max({ max_x,data.cx + radius * cos(angle1 / 180.0 * M_PI),data.cx + radius * cos(angle2 / 180.0 * M_PI) });

	Angle_1 = (angle1 > 180) ? (angle1 - 180) : (angle1 + 180);
	Angle_2 = (angle2 > 180) ? (angle2 - 180) : (angle2 + 180);
	if (Angle_1 > Angle_2) min_x = std::min({ min_x,data.cx - radius });
	else min_x = std::min({ min_x,data.cx + radius * cos(angle1 / 180.0 * M_PI),data.cx + radius * cos(angle2 / 180.0 * M_PI) });

	Angle_1 = (angle1 > 90) ? (angle1 - 90) : (angle1 + 270);
	Angle_2 = (angle2 > 90) ? (angle2 - 90) : (angle2 + 270);
	if (Angle_1 > Angle_2) max_y = std::max({ max_y,data.cy + radius });
	else max_y = std::max({ max_y,data.cy + radius * sin(angle1 / 180.0 * M_PI),data.cy + radius * sin(angle2 / 180.0 * M_PI) });

	Angle_1 = (angle1 > 270) ? (angle1 - 270) : (angle1 + 90);
	Angle_2 = (angle2 > 270) ? (angle2 - 270) : (angle2 + 90);
	if (Angle_1 > Angle_2) min_y = std::min({ min_y,data.cy - radius });
	else min_y = std::min({ min_y,data.cy + radius * sin(angle1 / 180.0 * M_PI),data.cy + radius * sin(angle2 / 180.0 * M_PI) });
}

bool Bridge::Reading(const std::string file)
{
	Bridge* obj = new Bridge();
	DL_Dxf* dxf = new DL_Dxf();
	if (!dxf->in(file.c_str(), obj))
	{
		std::cerr << file << " ��ʧ��\n";
		return false;
	}

	Shape::Set_Pan_X((X_Limit_P + X_Limit_N) / 2.0 - (max_x + min_x) / 2.0);
	Shape::Set_Pan_Y((Y_Limit_P + Y_Limit_N) / 2.0 - (max_y + min_y) / 2.0);

	if ((max_x - min_x) <= (X_Limit_P - X_Limit_N) && (max_y - min_y) <= (Y_Limit_P - Y_Limit_N)) return true;
	else
	{
		std::cout << "ͼ�������������num ( 0 ~ 100 ) ��ͼ�����������ߴ�� num % ����/��̣��� Enter ȡ�����˳���";
		std::string input;
		getline(std::cin, input);

		if (!input.empty())
		{
			double zoom = std::stod(input) / 100.0;
			Shape::Set_Zoom(std::min((X_Limit_P - X_Limit_N), (Y_Limit_P - Y_Limit_N)) * zoom / std::max((max_y - min_y), (max_x - min_x)));
			return true;
		}
		else
			return false;
	}

	delete dxf;
	delete obj;
}

void Bridge::Set_X_Limit_N(const double num)
{
	X_Limit_N = num;
}

void Bridge::Set_X_Limit_P(const double num)
{
	X_Limit_P = num;
}

void Bridge::Set_Y_Limit_N(const double num)
{
	Y_Limit_N = num;
}

void Bridge::Set_Y_Limit_P(const double num)
{
	Y_Limit_P = num;
}
