#ifndef _UPROUP_H_
#define _UPROUP_H_
#include"unit.h"

class UGroup{
public:
	set<Point*> unitSet;
	set<Point*> allParentSet;
	int tail;
	int size;
	//tail set,note it's reverse to tail value in class "Group".If the smallest index in this Ugroup is k,the tail is k-1
	//set<Unit*> ChildSet;

public:
	//void CalculateCS();
	//bool VerifyPoint(Point* p);
	UGroup();
	UGroup(const UGroup& u);
	UGroup(vector<Point*>);
	void insert(Point* p);
	void Print();
	void PrintAsc();
	int allPointSize();
};

#endif