#include"Ugroup.h"
#include<iostream>
#include<algorithm>

UGroup::UGroup(vector<Point*> vp){
	tail = INT_MAX;
	size = 0;
	for(vector<Point*>::iterator it = vp.begin();it != vp.end(); it++){
		//Unit u(*it);
		//Point* p = *it; 
		insert(*it);
	}
}

UGroup::UGroup(){
	tail = INT_MAX;
	size = 0;
}
UGroup::UGroup(const UGroup& u){
	tail = u.tail;
	size = u.size;
	allParentSet = u.allParentSet;
	unitSet = u.unitSet;
}

void UGroup::insert(Point* p){
	if(unitSet.find(p) == unitSet.end()){
		unitSet.insert(p);
		allParentSet.insert(p->pSet.begin(),p->pSet.end());
		if(tail > p->index - 1)
			tail = p->index - 1;
		size = unitSet.size() + allParentSet.size();
	}
}

int UGroup::allPointSize(){
	return size;
}

struct classcomp 
{
  bool operator()(Point* a, Point*b){
	return a->id < b->id;
  }
};

bool compareById(Point* a, Point*b){
	return a->id < b->id;
}

void UGroup::PrintAsc(){
	set<Point*,classcomp> merge;
	merge.insert(unitSet.begin(),unitSet.end());
	merge.insert(allParentSet.begin(),allParentSet.end());
	//sort(merge.begin(),merge.end(),compareById);
	std::cout << "{";
	set<Point*>::iterator it = merge.begin();
	while (true)
	{
		cout << "p" << (*it)->id;
		it++;
		if (it != merge.end())
		{
			std::cout << ",";
		}
		else
		{
			break;
		}
	}
	cout << "}" << endl;
}
