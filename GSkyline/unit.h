#ifndef UNIT_H_
#define UNIT_H_

#include<set>
#include"Point.h"

class Unit
{
public:
	int index;
	set<Point*> pSet;
	set<Point*> pointSet;
	Point* point;
public:
	Unit(Point* p);
};

#endif