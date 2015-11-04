#include "gskyline.h"
#include<iostream>
#include<vector>
#include"GLast.h"

int main()
{
	GSkyline skyline("data\\test.txt");
	vector<Group> pre = skyline.preprocessing(4);
	cout << "preprocessing result:" << endl;
	for(vector<Group>::iterator it = pre.begin(); it != pre.end(); it++){
		(*it).Print();
	}
	cout << "preprocessing end" << endl;
	cout << "p wise:" << endl;
	vector<Group> gs = skyline.PointWise(4);
	for(vector<Group>::iterator it = gs.begin(); it != gs.end(); it++){
		(*it).Print();
	}
	cout << "u wise plus:" << endl;
	vector<UGroup> gs1 = skyline.UnitWisePlus(4,false);
	for(vector<UGroup>::iterator it = gs1.begin(); it != gs1.end(); it++){
		(*it).PrintAsc();
	}
	return 0;
}