#include "gskyline.h"
#include<iostream>
#include<vector>
#include"GLast.h"

int main()
{
	GSkyline skyline("data\\test.txt");
	vector<Group> gs = skyline.PointWise(4);
	for(vector<Group>::iterator it = gs.begin(); it != gs.end(); it++){
		(*it).Print();
	}
	vector<UGroup> gs1 = skyline.UnitWisePlus(4,false);
	for(vector<UGroup>::iterator it = gs1.begin(); it != gs1.end(); it++){
		(*it).PrintAsc();
	}
	return 0;
}