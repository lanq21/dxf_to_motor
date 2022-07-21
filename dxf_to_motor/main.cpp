#include "dl_dxf.h"
#include "dxf_to_txt.h"
#include "Shape.h"

void testReading(char* file)
{
	dxf_to_txt* obj = new dxf_to_txt();
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
	char filename[50] = "C:\\Users\\10376\\Desktop\\test.dxf";
	testReading(filename);
	dxf_to_txt::Print_Range();
	Shape::Print_All();
	return 0;
}