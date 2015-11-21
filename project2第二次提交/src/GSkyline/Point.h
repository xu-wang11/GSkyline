#ifndef G_POINT_H_
#define G_POINT_H_
#include <vector>
#include <set>
#include <list>
//using namespace std;
#include"Point.h"

using namespace std;

class Point
{
public:
	vector<double> v;
	int layer;
	int id;
	int index;			             //index after sort.
	vector<Point*> pSet;             //parent set
	vector<Point*> cSet;             //children set
	list<Point*> simpleCSet;         //children whose parent is its first parent
	Point* firstParent;              //the first parent of all Parent
	bool isSkylinePoint;
	bool isVisited;
public:
	bool isDomain(const Point &p);
	bool operator < (const Point &m)const;
};

#endif