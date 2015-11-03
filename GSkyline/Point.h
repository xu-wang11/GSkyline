#ifndef G_POINT_H_
#define G_POINT_H_
#include <vector>
#include <set>
//using namespace std;
#include"Point.h"

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

#endif