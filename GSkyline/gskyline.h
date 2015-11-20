#ifndef GSKYLINE_H_
#define GSKYLINE_H_
#include <vector>
#include <set>
#include <stack>
#include"Point.h"
#include"Ugroup.h"

using namespace std;


class Group
{
public:
	vector<Point*> pointStack;
	set<Point*> pointSet;
	int tail;//tail set
	set<Point*> ChildSet;
	int maxLayer;
	int PointCount;

	int MaxPointCount;

public:
	void CalculateCS();
	bool VerifyPoint(Point* p);
	
	void Print();
};

class GSkyline
{

public:
	vector<Point*> allPoints;
	vector<vector<Point*>> layers;
	vector<Group> allGroups;
public:
	GSkyline(string filename);
	void load(string filename);
	void SortPoints();
	void BuildDSG();
	vector<Group> PointWise(int k);
	long long int PointWiseCount;
	void PointWisePlus(int k);
	void Solve(Group & g, int k);
	vector<Group> UnitWise(int k);
	//optimize should be false.it's a unsuccessful attempt
	/*vector<UGroup>*/int UnitWisePlus(int k,bool optimize);
	int UnitWisePlusPlus(int k);
	long long UnitWisePlusPlus1(int k,Point** all,int len);
	long long UnitWisePlusPlus2(int k,Point** all,int len);
	void print_layers();
	void sortPointsByLayer();
	void addFirstLayer(int k);
	vector<Group> preprocessing(int k);
	long long printAllLayer1(int stIndex, Point** all,int len,int k);
	void printAllCombineLayer1(int stIndex, Point** all,int len,int k,int* layer1,int layer1_len,int* flags,int sum,int block,int* blockSizes,string preString,long long& count);
	UGroup getGLast(Point* p);
	UGroup getGLast(UGroup p);

	void printByFlags(Point** all,int * flags,int k);
	void printByFlagsWithPre(string preString,Point** all,int * flags,int k);
	string getPreString(UGroup* ug);
};
#endif