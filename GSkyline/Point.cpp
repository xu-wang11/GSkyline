#include "Point.h"
#include "gskyline.h"

bool Point::isDomain(const Point &p)
{
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i] > p.v[i])
		{
			return false;
		}
	}
	return true;
}

bool Point::operator < (const Point &m)const
{
	
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i] < m.v[i])
		{
			return true;
		}
		else if (v[i] > m.v[i])
		{
			return false;
		}
	}
	return false;
}

Point::Point()
{
	Count = 0;
	group = NULL;
	isDeleted = false;
}
