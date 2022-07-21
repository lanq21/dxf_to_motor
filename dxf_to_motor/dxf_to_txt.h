#pragma once
#include "dl_creationadapter.h"
#include "Shape.h"

class dxf_to_txt : public DL_CreationAdapter
{
public:
	dxf_to_txt();
	static void Print_Range();
	virtual void addPoint(const DL_PointData& data);
	virtual void addLine(const DL_LineData& data);
	virtual void addArc(const DL_ArcData& data);
	virtual void addCircle(const DL_CircleData& data);
	virtual void addVertex(const DL_VertexData& data);
	virtual void addEllipse(const DL_EllipseData& data);
private:
	static double max_x;
	static double min_x;
	static double max_y;
	static double min_y;
	int polyline_vertex_count;
};