#pragma once
#include "dl_creationadapter.h"
#include <fstream>
class Bridge : public DL_CreationAdapter
{
public:
	static double Get_Max_X();
	static double Get_Min_X();
	static double Get_Max_Y();
	static double Get_Min_Y();
	virtual void addPoint(const DL_PointData& data);
	virtual void addLine(const DL_LineData& data);
	virtual void addArc(const DL_ArcData& data);
	virtual void addCircle(const DL_CircleData& data);
	virtual void addVertex(const DL_VertexData& data);
	virtual void addEllipse(const DL_EllipseData& data);
	static void Reading(const std::string file);
private:
	static double max_x;
	static double min_x;
	static double max_y;
	static double min_y;
};