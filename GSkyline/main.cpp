#include "gskyline.h"

int main()
{
	GSkyline skyline("data\\test.txt");
	vector<Group> gs = skyline.PointWise(4);
	for each (Group g in gs)
	{
		g.Print();
	}
	return 0;
}