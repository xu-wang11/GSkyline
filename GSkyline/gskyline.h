#ifndef GSKYLINE_H_
#define GSKYLINE_H_
#include <vector>
using namespace std;
class Point
{
public:
	vector<double> v;
	int layer;
	int id;
	vector<Point*> pSet;
	vector<Point*> cSet;
	
public:
	bool isDomain(const Point &p);
	bool operator < (const Point &m)const;
};

class Group
{
public:
	vector<Point*> p;
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