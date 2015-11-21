//author KangRong
#include "Unit.h"

#include <algorithm>
#include <assert.h>

Unit::Unit(Point* p){
	index = p->index;
	vector<Point*>::iterator it;
	for (it = p->pSet.begin(); it != p->pSet.end(); it++)
	{
		pointSet.insert(*it);
	}
	pointSet.insert(p);
	point = p;
}

