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
	merge = u.merge;
}

void UGroup::insert(Point* p){
	if(unitSet.find(p) == unitSet.end()){
		unitSet.insert(p);
		allParentSet.insert(p->pSet.begin(),p->pSet.end());
		if(tail > p->index - 1)
			tail = p->index - 1;
		merge.insert(p);
		merge.insert(p->pSet.begin(),p->pSet.end());
		size = merge.size();
	}
}

int UGroup::allPointSize(){
	return size;
}



bool compareById(Point* a, Point*b){
	return a->id < b->id;
}

void UGroup::PrintAsc(){
	//sort(merge.begin(),merge.end(),compareById);
	/*std::cout << "{";
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
	cout << "}" << endl;*/


	/*std::cout << "{";
	set<Point*>::iterator it = merge.begin();
	int sum = 0;
	while (true)
	{
		cout << "p" << (*it)->id;
		sum += (*it)->id;
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
	cout << "," << sum+100000 << endl;*/
}
void UGroup::PrintDetail(){
	cout << "unit:";
	for(set<Point*>::iterator it = unitSet.begin();it != unitSet.end(); it++){
		cout << 'p' << (*it)->id << ":" << (*it)->index << ',';
	}
	cout << "parent:";
	for(set<Point*>::iterator it = allParentSet.begin();it != allParentSet.end(); it++){
		cout << 'p' << (*it)->id << ":" << (*it)->index << ',';
	}
	cout <<"size:" << size << endl;
}
UGroup::~UGroup(){
	set<Point*>().swap(allParentSet);
	set<Point*>().swap(unitSet);
	set<Point*,classcomp>().swap(merge);
}
void UGroup::clearUGroup(){
	allParentSet.clear();
	unitSet.clear();
	merge.clear();
}