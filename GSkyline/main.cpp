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
	string filename = "data/corr_4.txt";
	//string filename = "data/test.txt";
	int k = 11;
	//string flags = "yyyyy";
	string flags = "nnnny";
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
	else if (argc == 5){
		filename = string(argv[1]);
		k = atoi(argv[2]);
		string outfile = string(argv[3]);
		//flags use "y/n" to control which algorithms will be executed
		//0:uwiseplus;	1:uwiseplus+1;	2:uwiseplus+2;	3:pwiseplus
		flags = string(argv[4]);
		printf(argv[4]);
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
	//cout << "layers:" << skyline.layers.size() << endl;
	end = GetTickCount();
	//printf("building cost time: %lld ms\n", end - start);
	cout << filename << "\t";
	cout << k << "\t";
	printf("%lld\t", end - start);

	start = GetTickCount();
	vector<Group> pre = skyline.preprocessing(k);
    end = GetTickCount();
	//printf("processing cost time: %lld ms\n", end - start);
	cout << "layers:" << skyline.layers.size() << "\t" << "size:" << skyline.allPoints.size() << "\t";
	printf("%lld\t", end - start);
	//cout << "preprocessing result:" << endl;
	long preNum = pre.size();
	/*for(vector<Group>::iterator it = pre.begin(); it != pre.end(); it++){
		(*it).Print();
	}*/
	//cout << "preprocessing end" << endl;


	Point** all = new Point*[skyline.allPoints.size()];
	for (int i = 0; i < skyline.allPoints.size(); i++){
		all[i] = skyline.allPoints[i];
	}
	
	if(flags[0] == 'y'){
		//cout <<flags[0] << "u wise plus:" << endl;
		start = GetTickCount();
		int uwiseNum = skyline.UnitWisePlus(k,false);
		end = GetTickCount();
		cout <<(uwiseNum + preNum) << "\t";
		cout <<(end - start) << "\t";
		//for(vector<UGroup>::iterator it = gs1.begin(); it != gs1.end(); it++){
		//	(*it).PrintAsc();
		//}
	}
	else{
		cout << -1 << "\t" << -1 << "\t";
	}

	if(flags[1] == 'y'){
		//cout << flags[1] << "u wise plusplus:" << endl;
		start = GetTickCount();
		int uwiseppNum = 0;
		uwiseppNum = skyline.UnitWisePlusPlus(k);
		end = GetTickCount();
		cout <<(uwiseppNum + preNum) << "\t";
		cout <<(end - start) << "\t";
	}
	else{
		cout << -1 << "\t" << -1 << "\t";
	}

	if(flags[2] == 'y'){
		//cout <<"u++" << endl;
		start = GetTickCount();
		//long long uwisepp1Num = skyline.UnitWisePlusPlus1(k,all,skyline.allPoints.size());
		end = GetTickCount();
		//cout <<(uwisepp1Num + preNum) << "\t";
		cout <<(end - start) << "\t";
	}
	else{
		cout << -1 << "\t" << -1 << "\t";
	}

	if(flags[3] == 'y'){
		//cout <<"u++2" << endl;
		start = GetTickCount();
		long long uwisepp2Num = skyline.UnitWisePlusPlus2(k,all,skyline.allPoints.size());
		end = GetTickCount();
		cout <<(uwisepp2Num + preNum) << "\t";
		cout <<(end - start) << "\t";
	}
	else{
		cout << -1 << "\t" << -1 << "\t";
	}

	if(flags[4] == 'y'){
		//cout << "p wise:";	
		try{
			start = GetTickCount();
			skyline.PointWisePlus(k);
			/**for(vector<Group>::iterator it = vg.begin(); it != vg.end(); it++){
			(*it).Print();
			}*/
			end = GetTickCount();
			printf("%ld\t", skyline.PointWiseCount + preNum);
			cout << (end - start) << "\t";
		}
		catch (exception &e){
			printf(e.what());
		}
		//return 0;
	}
	else{
		cout << -1 << "\t" << -1 << "\t";
	}

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
	*/

	cout <<"\n";

	
	return 0;
}