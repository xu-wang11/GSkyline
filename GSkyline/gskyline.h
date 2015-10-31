#ifndef GSKYLINE_H_
#define GSKYLINE_H_
#include <vector>
using namespace std;
class Point
{
public:
	int* v;
	int len;
	int level;
	int id;
	vector<Point> pSet;
	vector<Point> cSet;
	
public:
	bool isDomain(const Point &p);
};

class Group
{
public:
	vector<Point> p;
};

class GSkyline
{
public:
	vector<Point> allPoints;
public:
	GSkyline(string filename);
	void load();
	void SortPoints();
	void BuildDSG();
	vector<Group> PointWise();
	vector<Group> UnitWise();

};
#endif