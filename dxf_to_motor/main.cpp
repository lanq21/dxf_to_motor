#include "COM.h"
#include "Bridge.h"
#include "Shape.h"
#include <iostream>

int main()
{
	// 获取 dxf 文件名
	std::cout << "输入文件名或按 Enter 使用默认值 (\"test.dxf\"):";
	std::string filename;
	getline(std::cin, filename);
	if (filename.empty())
		filename = "test.dxf"; // 默认文件名

	// 读取 dxf 文件
	if (Bridge::Reading(filename))
	{
		// 钻头抬起的 z 坐标
		Shape::Set_Z_Up(10);

		// 钻头落下的 z 坐标
		Shape::Set_Z_Down(0);

		// 设置弧线上的分割点个数
		Shape::Set_Division(20);

		// 坐标信息将被写入到文本文件 "test.txt"
		file_out.open("test.txt", std::ios_base::trunc);

		// 打开串口
		if (com.Open())
		{
			// 串口发送间隔，单位 ms （串口发送本身有速度限制，不会很快）
			COM::Set_Delay_Time(1500);
		}

		// 在终端 COM 和 文本文件 "test.txt" 输出坐标信息
		// 也可以不打开串口，仅输出到 "test.txt" 和 终端
		Shape::Output_All();
	}

	return 0;
}