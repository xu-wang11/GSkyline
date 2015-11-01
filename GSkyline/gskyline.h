#ifndef GSKYLINE_H_
#define GSKYLINE_H_
#include <vector>
#include <set>
using namespace std;
class Point
{
public:
	vector<double> v;
	int layer;
	int id;
	int index;//index after sort.
	vector<Point*> pSet; //parent set
	vector<Point*> cSet; // children set
	bool isSkylinePoint;
public:
	bool isDomain(const Point &p);
	bool operator < (const Point &m)const;
};

class Group
{
public:
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
	vector<Group> UnitWise(int k);
	vector<Group> UnitWisePlus(int k);
	void print_layers();

};
#endif