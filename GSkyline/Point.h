#ifndef G_POINT_H_
#define G_POINT_H_
#include <vector>
#include <set>
#include <list>
//using namespace std;

#include"Point.h"

using namespace std;
class Group;

class Point
{
public:
	vector<double> v;
	int layer;
	int id;
	int index;			             //index after sort.
	vector<Point*> pSet;             //parent set
	int* firstLayerIndex;
	int firstLayerLen;
	vector<Point*> cSet;             //children set
	vector<Point*> simpleCSet;         //children whose parent is its first parent
	Point* firstParent;              //the first parent of all Parent
	bool isSkylinePoint;
	bool isVisited;
	int Count; //计数
	Group* group;
	bool isDeleted;

	int indexInSimpleSet; // 在simpleSet中的索引
	
public:
	Point();
	bool isDomain(const Point &p);
	bool operator < (const Point &m)const;
};

#endif