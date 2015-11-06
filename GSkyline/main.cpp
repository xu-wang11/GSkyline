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
int main(int argc, char** argv)
{
	string filename = "data/inde_2.txt";
	int k = 4;
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
		freopen_s(&output, outfile.data(), "w", stdout);
		cout << "entering custom mode:" << filename.data() << " k:" << k <<"output:" << outfile.data() << endl;
	}
	else{
		cout << "error parameter" << endl;
		return 0;
	}

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
	cout << "preprocessing result:" << endl;
	int preNum = pre.size();
	for(vector<Group>::iterator it = pre.begin(); it != pre.end(); it++){
		(*it).Print();
	}
	cout << "preprocessing end" << endl;
	cout << "p wise:" << endl;
	start = GetTickCount();
	try{
		skyline.PointWisePlus(k);
		/*for(vector<Group>::iterator it = gs.begin(); it != gs.end(); it++){
		(*it).Print();
		}*/
		end = GetTickCount();
		printf("get result group: %d\n", skyline.PointWiseCount + preNum);
	}
	catch (exception &e){
		printf(e.what());
	}
	cout << "pwise cost time: " << (end - start) << " ms" << endl;

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
	
	

	//printf("pwise cost time: %lld ms\n", end - start);
	return 0;
}