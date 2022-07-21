#pragma once
class Drill
{
public:
	static double Origin_x;
	static double Origin_y;
	Drill(const double& x, const double& y);
	void Print_Range();
	void Move_To(const double& x, const double& y);
	void Raise_Drill();
	void Lower_Drill();
	void Run_Drill();
	void Stop_Drill();
private:
	bool Drill_Down;
	bool Drill_On;
	double max_x;
	double max_y;
};