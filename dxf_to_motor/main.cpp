#include "COM.h"
#include "dl_dxf.h"
#include "Bridge.h"
#include "Shape.h"

void testReading(char* file)
{
	Bridge* obj = new Bridge();
	DL_Dxf* dxf = new DL_Dxf();
	if (!dxf->in(file, obj))
	{
		std::cerr << file << " could not be opened.\n";
		return;
	}
	delete dxf;
	delete obj;
}

int main()
{
	char filename[50] = "test.dxf";
	testReading(filename);
	file_out.open("test.txt", std::ios_base::trunc);
	Bridge::Print_Range();
	Shape::Print_All();
	Shape::Output_All();
	return 0;
}