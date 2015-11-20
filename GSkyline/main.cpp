#include "gskyline.h"
#include<iostream>
#include<vector>
#include"GLast.h"
#include<time.h>
#include <windows.h>

#include "MyPrintf.h"
FILE* output;
using namespace std;
int MyPrintf::cur_index = 0;
char MyPrintf::print_str[buf_size];

void testUwisePlusPlus2(){
	
}
int main(int argc, char** argv)
{
	string filename = "data/anti_4.txt";
	//string filename = "data/test.txt";
	int k = 3;
	//handle input by wx
	printf("Usage: \n default mode:GSkyline \n Custom mode: GSkyline <data file> <gskyline size> <output: if empty use stdout\n");
	if (argc == 1) {
		cout << "entering default mode: file:" << filename.data() << " k:" << k << endl;
	}
	else if (argc == 3){
		filename = string(argv[1]);
		k = atoi(argv[2]);
		cout << "entering custom mode:" << filename.data() << " k:" << k << endl;
	}
	else if (argc == 4){
		filename = string(argv[1]);
		k = atoi(argv[2]);
		string outfile = string(argv[3]);
		freopen_s(&output, outfile.data(), "a", stdout);
		//cout << "entering custom mode:" << filename.data() << " k:" << k <<"output:" << outfile.data() << endl;
	}
	else{
		cout << "error parameter" << endl;
		return 0;
	}

	time_t start, end;
	//cout << "building graph" << endl;
	start = GetTickCount();
	GSkyline skyline(filename);
	//cout << "size:" << skyline.allPoints.size() << endl;
	cout << "layers:" << skyline.layers.size() << endl;
	end = GetTickCount();
	//printf("building cost time: %lld ms\n", end - start);
	cout << filename << "\t";
	cout << k << "\t";
	printf("%lld\t", end - start);

	start = GetTickCount();
	vector<Group> pre = skyline.preprocessing(k);
    end = GetTickCount();
	//printf("processing cost time: %lld ms\n", end - start);
	printf("%lld\t", end - start);
	//cout << "preprocessing result:" << endl;
	long preNum = pre.size();
	/*for(vector<Group>::iterator it = pre.begin(); it != pre.end(); it++){
		(*it).Print();
	}*/
	cout << "preprocessing end" << endl;


	Point** all = new Point*[skyline.allPoints.size()];
	for (int i = 0; i < skyline.allPoints.size(); i++){
		all[i] = skyline.allPoints[i];
	}

	cout << "p wise:" << endl;
	start = GetTickCount();
	try{
		skyline.PointWisePlus(k);
		/**for(vector<Group>::iterator it = vg.begin(); it != vg.end(); it++){
		(*it).Print();
		}*/
		end = GetTickCount();
		printf("count: %d\n", skyline.PointWiseCount + preNum);
		//printf("get result group: %d\n", vg.size() + preNum);

	}
	catch (exception &e){
		printf(e.what());
	}
	//return 0;

	/*
	cout << "p wise:" << endl;
	start = GetTickCount();
	skyline.PointWisePlus(k);
	end = GetTickCount();
	printf("%d\t", skyline.PointWiseCount + preNum);
	cout <<(end - start) << "\t";
	//for(vector<Group>::iterator it = gs.begin(); it != gs.end(); it++){
	//(*it).Print();
	//}
	
	//cout << "u wise plus:" << endl;
	start = GetTickCount();
	int uwiseNum = skyline.UnitWisePlus(k,false);
	end = GetTickCount();
	cout <<(uwiseNum + preNum) << "\t";
	cout <<(end - start) << "\t";
	//for(vector<UGroup>::iterator it = gs1.begin(); it != gs1.end(); it++){
	//	(*it).PrintAsc();
	//}

	start = GetTickCount();
	int uwiseppNum = 0;
	uwiseppNum = skyline.UnitWisePlusPlus(k);
	end = GetTickCount();
	cout <<(uwiseppNum + preNum) << "\t";
	cout <<(end - start) << "\t";
	*/


	cout <<"u++" << endl;
	start = GetTickCount();
	long long uwisepp1Num = skyline.UnitWisePlusPlus1(k,all,skyline.allPoints.size());
	end = GetTickCount();
	cout <<(uwisepp1Num + preNum) << "\t";
	cout <<(end - start) << "\t";
	
	cout <<"u++2" << endl;
	start = GetTickCount();
	long long uwisepp2Num = skyline.UnitWisePlusPlus2(k,all,skyline.allPoints.size());
	end = GetTickCount();
	cout <<(uwisepp2Num + preNum) << "\t";
	cout <<(end - start) << "\t";
	
	cout <<"\n";

	
	return 0;
}