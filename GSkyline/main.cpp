#include "gskyline.h"
#include<iostream>
#include<vector>
#include"GLast.h"
#include<time.h>
#include <windows.h>

using namespace std;
int main()
{
	int k = 2;
	//string filename = "data\\anti_8.txt";
	//string filename = "data\\corr_8.txt";
	string filename = "data\\inde_8.txt";
	//string filename = "data\\test.txt";
	time_t start, end;
	cout << "building graph" << endl;
	start = GetTickCount();
	GSkyline skyline(filename);
	cout << "size:" << skyline.allPoints.size() << endl;
	cout << "layers:" << skyline.layers.size() << endl;
	end = GetTickCount();
	printf("building cost time: %lld ms\n", end - start);

	start = GetTickCount();
	vector<Group> pre = skyline.preprocessing(k);
    end = GetTickCount();
	printf("processing cost time: %lld ms\n", end - start);
	//cout << "preprocessing result:" << endl;
	int preNum = pre.size();
	//for(vector<Group>::iterator it = pre.begin(); it != pre.end(); it++){
	//	(*it).Print();
	//}
	//cout << "preprocessing end" << endl;

	cout << "u wise plus:" << endl;
	start = GetTickCount();
	/*vector<UGroup> gs1*/
	int uwiseNum = skyline.UnitWisePlus(k,false);
	//for(vector<UGroup>::iterator it = gs1.begin(); it != gs1.end(); it++){
	//	(*it).PrintAsc();
	//}
	//printf("get result group: %d\n",gs1.size());
	end = GetTickCount();
	cout << "u wise get result group num:" <<(uwiseNum + preNum) << endl;
	cout << "uwise cost time: "<<(end - start) <<" ms" << endl;
	
	cout << "p wise:" << endl;
	start = GetTickCount();
	try{
		vector<Group> gs = skyline.PointWise(k);
		/*for(vector<Group>::iterator it = gs.begin(); it != gs.end(); it++){
			(*it).Print();
		}*/
		end = GetTickCount();
		printf("get result group: %d\n",gs.size());
	}
	catch(exception &e){
		printf(e.what());
	}
	cout << "pwise cost time: "<<(end - start) <<" ms" << endl;

	//printf("pwise cost time: %lld ms\n", end - start);
	return 0;
}