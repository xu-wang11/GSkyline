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
	void print_layers();
	void sortPointsByLayer();
	vector<Group> preprocessing(int k);
	UGroup getGLast(Point* p);
	UGroup getGLast(UGroup p);
};
#endif