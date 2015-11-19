#include "gskyline.h"

#include <fstream>
#include <algorithm>
#include <assert.h>
#include <iostream>
using namespace std;

bool comparePoint(Point* a, Point* b) 
{ 
	return (*a < *b); 
}

bool comparePointByLayer(Point* a, Point* b){
	return a->layer < b->layer;
}

GSkyline::GSkyline(string filename)
{
	this->load(filename);
	this->SortPoints();
	this->BuildDSG();
	this->sortPointsByLayer();
	//this->print_layers();
}

void GSkyline::load(string filename)
{
	this->allPoints.clear();
	ifstream ifile(filename);
	char buf[100];
	int id = 1;
	while (!ifile.eof())
	{
		ifile.getline(buf, 100);
		string str(buf);
		int index;
		Point* p = new Point();
		
		while ((index = str.find_first_of(' ')) >= 0)
		{
			string str1 = str.substr(0, index);
			p->v.push_back(atof(str1.c_str()));
			str = str.substr(index + 1);
		}
		p->v.push_back(atof(str.c_str()));
		p->id = id++;
		if (p->v.size() >= 2){
			this->allPoints.push_back(p);
		}
		
	}
}

void GSkyline::SortPoints()
{
	//对所有点进行排序
	sort(this->allPoints.begin(), this->allPoints.end(), comparePoint);
}

void GSkyline::BuildDSG()
{
	if (allPoints.size()){
		allPoints[0]->layer = 0;
		allPoints[0]->isSkylinePoint = true;
		vector<Point*> layer;
		layer.push_back(allPoints[0]);
		layers.push_back(layer);
		int maxlayer = 0;
		for (int i = 1; i < allPoints.size(); i++)
		{
			Point* p = allPoints[i];
			bool isdomain;
			for (int i = 0; i < layers.size(); i++){
				isdomain = false;
				for (int j = 0; j < layers[i].size(); j++){
					if (layers[i][j]->isDomain(*p)){
						isdomain = true;
						break;
					}
				}
				if (!isdomain){
					//insert to layer i.
					p->layer = i;
					if (i == 0){
						p->isSkylinePoint = true;
					}
					else{
						p->isSkylinePoint = false;
					}
					layers[i].push_back(p);
					break;
				}
			}
			if (isdomain){
				//
				p->layer = ++maxlayer;
				p->isSkylinePoint = false;
				vector<Point*> layer;
				layer.push_back(p);
				layers.push_back(layer);
			}
		


		}
		for (int i = 0; i < layers.size(); i++)
		{
			for (int j = 0; j < layers[i].size(); j++)
			{
				Point * p = layers[i][j];
				for (int k = 0; k < i; k++)
				{
					for (int s = 0; s < layers[k].size(); s++)
					{
						if (layers[k][s]->isDomain(*p))
						{
							layers[k][s]->cSet.push_back(p);
							p->pSet.push_back(layers[k][s]);
						}
					}
				}
			}
		}

	}


}
vector<Group> GSkyline::PointWise(int k)
{
	vector<vector<Group>> groups;
	Group g0;
	g0.tail = 0;
	g0.maxLayer = 0;
	vector<Group> vg0;
	vg0.push_back(g0);
	groups.push_back(vg0);
	for (int i = 1; i <= k; i++)
	{
		vector<Group> vgk;
		vector<Group>::iterator it;
		for (it = groups[i - 1].begin(); it != groups[i - 1].end(); it++)
		{
			it->CalculateCS();
			for (int k = it->tail; k < allPoints.size(); k++)
			{
				Point * p = allPoints[k];
				if (p->isSkylinePoint || it->ChildSet.find(p) != it->ChildSet.end())
				{
					if (p->layer - it->maxLayer < 2)
					{
						//creat new group
						if (it->VerifyPoint(p))
						{
							Group ng;
							ng.pointSet.insert(it->pointSet.begin(), it->pointSet.end());
							ng.pointSet.insert(p);
							ng.tail = k + 1;
							ng.maxLayer = it->maxLayer > p->layer ? it->maxLayer : p->layer;
							vgk.push_back(ng);
						}
					}
				}
			}
		}
		groups.push_back(vgk);
	}
	return groups[k];
}

//add by wx
//breadth first algorithm according to the paper will fail when data bombs because of too heavy memory cosumption
//instead we use depth first search
void GSkyline::PointWisePlus(int k)
{
	this->PointWiseCount = 0;
	//calculate first parent and simplecSet
	int len = this->allPoints.size();
	Point* root = new Point();
	for (int i = len - 1; i >= 0; i--){
		Point *p = allPoints[i];
		int player = p->layer - 1;
		if (player == -1){
			p->firstParent = root;
			root->simpleCSet.push_front(p);
		}
		else{
			Point *parent = NULL;
			for (vector<Point*>::iterator it = p->pSet.begin(); it != p->pSet.end(); it++){
				if ((*it)->layer == player){
					if (parent == NULL){
						parent = *it;
					}
					else if ((*it)->index < parent->index){
						parent = *it;
					}
				}
			}
			p->firstParent = parent;
			if (parent){
				parent->simpleCSet.push_front(p);
			}
		}
	}

	//breadth first
	
	Group g;
	g.pointSet.insert(root);
	g.pointStack.push_back(root);
	Solve(g, k);
}

void  GSkyline::Solve(Group &g, int k){
	set<Point*> parentPoints;
	for (vector<Point*>::iterator it = g.pointStack.begin() + 1; it != g.pointStack.end(); it++){
		parentPoints.insert(*it);
		for (vector<Point*>::iterator ij = (*it)->pSet.begin(); ij != (*it)->pSet.end(); ij++){
			parentPoints.insert(*ij);
		}
		
	}
	
	int psize = parentPoints.size();
	Point* lastPoint = g.pointStack[g.pointStack.size() - 1];
	for (list<Point*>::iterator it = (lastPoint->simpleCSet).begin(); it != lastPoint->simpleCSet.end(); it++){
		int cpsize = psize;
		if (parentPoints.find(*it) == parentPoints.end()){
			cpsize++;
		}
		for (vector<Point*>::iterator ik = (*it)->pSet.begin(); ik != (*it)->pSet.end(); ik++){
			if (parentPoints.find(*ik) == parentPoints.end()){
				cpsize++;
				
			}
		}
		if (cpsize > k){
			continue;
		}
		else if (cpsize == k && g.pointStack.size() == k){
			
				Group ng;
				ng.pointSet.insert(parentPoints.begin(), parentPoints.end());
				ng.pointSet.insert(*it);
				for (vector<Point*>::iterator ik = (*it)->pSet.begin(); ik != (*it)->pSet.end(); ik++){
					ng.pointSet.insert(*ik);
				}
				//ng.Print();
				PointWiseCount++;
				continue;
			
		}
		
		else{
			g.pointStack.push_back(*it);
			Solve(g, k);
			g.pointStack.pop_back();
		}
	}
	// iterator all siblings.
	Point * parent = lastPoint->firstParent;
	while (parent){
		if (parent->simpleCSet.size() > 0){
			list<Point*>::reverse_iterator it = parent->simpleCSet.rbegin();

			while ((*it) != lastPoint){
				//
				int cpsize = psize;
				if (parentPoints.find(*it) == parentPoints.end()){
					cpsize++;
				}
				for (vector<Point*>::iterator ik = (*it)->pSet.begin(); ik != (*it)->pSet.end(); ik++){
					if (parentPoints.find(*ik) == parentPoints.end()){
						cpsize++;
					}
				}
				if (cpsize > k){
					it++;
					continue;
				}
				else if (cpsize == k && g.pointStack.size() == k){
					
						Group ng;
						ng.pointSet.insert(parentPoints.begin(), parentPoints.end());
						ng.pointSet.insert(*it);
						for (vector<Point*>::iterator ik = (*it)->pSet.begin(); ik != (*it)->pSet.end(); ik++){
							ng.pointSet.insert(*ik);
						}
						PointWiseCount++;
						//ng.Print();
						it++;
						continue;
					
				}
				
				else{
					g.pointStack.push_back(*it);
					Solve(g, k);
					g.pointStack.pop_back();
				}
				it++;
			}
		}
		lastPoint = parent;
		parent = parent->firstParent;
	}


}



vector<Group> GSkyline::UnitWise(int k)
{
	throw exception("unimplemented");
	return vector<Group>();
}
UGroup GSkyline::getGLast(Point* p){
	UGroup ret;
	for(int i = p->index; i >= 0; i--){
		//cout << "add p" << allPoints[i]->id << endl;
		ret.insert(allPoints[i]);
//		if(i % 1000 == 0)
//			cout << "add " << i << endl;
	}
	return ret;
}

UGroup GSkyline::getGLast(UGroup ug){
	set<Point*>::iterator it = ug.unitSet.begin();
	Point* lastUnit = *it;
	UGroup ret;
	for(;it != ug.unitSet.end();it++){
		ret.insert(*it);
		if((*it)->index < lastUnit->index)
			lastUnit = *it;
	}
	for(int i = lastUnit->index-1; i >= 0; i--)
		ret.insert(allPoints[i]);
	return ret;
}

/*vector<UGroup>*/int GSkyline::UnitWisePlus(int k,bool optimize)
{
	int resultNum = 0;
	//int statCandidateNum = 0;
	vector<UGroup> result;
	for(int u1 = allPoints.size()-1; u1 >= 0; u1--){
		//cout << "try: p"<< allPoints[u1]->id << endl;
		//construct 1-unit groups with each point
		vector<Point*> points;
		points.push_back(allPoints[u1]);
		UGroup ug1(points);
		//statCandidateNum++;
		//preprocessing
		//if(ug1.size == k){
		//	//ug1.Print();
		//	//result.push_back(ug1);
		//	resultNum++;
		//	continue;
		//}
		//statCandidateNum++;
		//UGroup last = getGLast(allPoints[u1]);
		if(u1 + 1 == k){
			//result.push_back(last);
			resultNum++;
			break;
		}
		else if(u1 + 1 < k){
			break;
		}

		vector<UGroup> now_layer_i_ugs;
		vector<UGroup> last_layer_i_ugs;
		now_layer_i_ugs.push_back(ug1);

		//test to debug
		//now_layer_i_ugs.clear();
		//points.clear();
		//points.push_back(allPoints[9440]);
		//UGroup ug2(points);
		//now_layer_i_ugs.push_back(ug2);

		int i = 2;
		while(true){
			//vector<UGroup> layer_i_ugs;
			vector<UGroup>().swap( last_layer_i_ugs);
			last_layer_i_ugs = now_layer_i_ugs;
			vector<UGroup>().swap( now_layer_i_ugs);
			now_layer_i_ugs.clear();
			for(vector<UGroup>::iterator it = last_layer_i_ugs.begin();it != last_layer_i_ugs.end(); it++){
				UGroup ug = *it;
				//ug.PrintDetail();
				if(optimize){
					//a tentative optimization
					//statCandidateNum++;
					UGroup last = getGLast(ug);
					if(last.size == k){
						//result.push_back(last);
						resultNum++;
						//cout << "candidate groups:" << statCandidateNum << endl;
						return resultNum;
					}
					else if(last.size < k){
						//cout << "candidate groups:" << statCandidateNum << endl;
						return resultNum;
					}
				}
				set<Point*> ps = it->allParentSet;
				for(int j = ug.tail; j >= 0; j--){
					if(ps.find(allPoints[j]) == ps.end()){
						//statCandidateNum++;
						UGroup new_ug(ug);
						//ug.PrintAsc();
						//new_ug.unitSet=  ug.unitSet;
						//new_ug.allParentSet = ug.allParentSet;
						new_ug.insert(allPoints[j]);
						//new_ug.PrintDetail();
						new_ug.tail = j - 1;
						if(new_ug.size == k){
							//new_ug.Print();
							//result.push_back(new_ug);
							resultNum++;
						}
						else if(new_ug.allPointSize() < k){
							now_layer_i_ugs.push_back(new_ug);
						}
					}
				}
			}
			if(now_layer_i_ugs.empty())
				break;
			i++;
		}
	}
	//cout << "candidate groups:" << statCandidateNum << endl;
	//cout <<"uwise result count:" << resultNum << endl;
	return resultNum;
}


/*vector<UGroup>*/int GSkyline::UnitWisePlusPlus(int k)
{
	if(allPoints.size() == 0)
		return 0;
	int resultNum = 0;
	vector<UGroup> result;
	UGroup* uGroups = new UGroup[k+1];
	size_t startI = 0;
	int allLen = allPoints.size();
	int layer = 1;
	int* tailList = new int[k+1];
	int* parentNumList = new int[k+1];
	memset(parentNumList, 0, sizeof(int)*(k+1));
	int lastSum = 0;
	tailList[0] = allLen - 1;
	int tail = tailList[0];
	int i = tail;
	int leftAll;
	while(1){
		if(layer <= 0)
			break;
		//cout << "try: p"<< allPoints[u1]->id << endl;
		//new_point
		Point* np = allPoints[i];
		leftAll = layer + lastSum + parentNumList[layer] + np->index;
		if(leftAll <= k){
			if(leftAll == k){
				uGroups[layer] = uGroups[layer - 1];
				//如果出现这种情况，把np后面所有的点都加进去
				resultNum++;
			}
			//退回上一层
			i = tailList[layer-1];
			uGroups[layer].clearUGroup();
			lastSum -= parentNumList[layer-1];
			parentNumList[layer] = 0;
			layer--;
			continue;
		}
		set<Point*>* ps = &(uGroups[layer - 1].allParentSet);
		if(ps->find(np) == ps->end()){
			//UGroup* new_ug = &uGroups[layer];
			//这里会不会有危险，直接覆盖的话。
			//TODO：vector直接覆盖会不会产生没有回收的内存
			uGroups[layer] = uGroups[layer - 1];
			uGroups[layer].insert(np);
			uGroups[layer].tail = i - 1;
			tailList[layer] = i - 1;
			
			if(uGroups[layer].allPointSize() < k){
				//do it next loop
				i = tailList[layer];
				lastSum += parentNumList[layer];
				layer++;
			}
			else{
				if(uGroups[layer].size == k){
					//uGroups[layer].PrintAsc();
					//result.push_back(new_ug);
					resultNum++;
				}
				//sibling point
				i--;
				uGroups[layer].clearUGroup();
			}
		}
		else{
			i--;
			parentNumList[layer]++;
		}
		if(i < 0){
			//向上退一层，应该删除layer-1这一层，否则下次要写在layer-1这一层，没有清空直接覆盖。
			i = tailList[layer - 1];
			uGroups[layer-1].clearUGroup();
			lastSum -= parentNumList[layer-1];
			parentNumList[layer] = 0;
			layer--;
		}
	}
	//cout << "candidate groups:" << statCandidateNum << endl;
	//cout <<"uwise result count:" << resultNum << endl;
	return resultNum;
	return 0;
}

long long GSkyline::UnitWisePlusPlus1(int k,Point** all,int len)
{
	//cout << "hehe" << printAllLayer1(10,NULL,0,4) << endl;
	//exit(0);
	if(len == 0)
		return 0;
	long long resultNum = 0;
	vector<UGroup> result;
	UGroup* uGroups = new UGroup[k+1];
	size_t startI = 0;
	int allLen = len;
	int layer = 1;
	int* tailList = new int[k+1];
	//int* parentNumList = new int[k+1];
	//memset(parentNumList, 0, sizeof(int)*(k+1));
	//int lastSum = 0;
	tailList[0] = allLen - 1;
	int tail = tailList[0];
	int i = tail;
	int leftAll;
	while(1){
		if(layer <= 0)
			break;
		if(layer == 1 && all[i]->layer == 0){
			//int s = 0;
			return resultNum + printAllLayer1(i, all, len, k);
		}
		//cout << "try: p"<< allPoints[u1]->id << endl;
		//new_point
		Point* np = all[i];
		//leftAll = layer + lastSum + parentNumList[layer] + np->index;
		/*if(leftAll <= k){
			if(leftAll == k){
				uGroups[layer] = uGroups[layer - 1];
				//如果出现这种情况，把np后面所有的点都加进去
				resultNum++;
			}
			//退回上一层
			i = tailList[layer-1];
			uGroups[layer].clearUGroup();
			//lastSum -= parentNumList[layer-1];
			//parentNumList[layer] = 0;
			layer--;
			continue;
		}*/
		set<Point*>* ps = &(uGroups[layer - 1].allParentSet);
		if(ps->find(np) == ps->end()){
			//UGroup* new_ug = &uGroups[layer];
			//这里会不会有危险，直接覆盖的话。
			//TODO：vector直接覆盖会不会产生没有回收的内存
			uGroups[layer] = uGroups[layer - 1];
			uGroups[layer].insert(np);
			uGroups[layer].tail = i - 1;
			tailList[layer] = i - 1;
			
			if(uGroups[layer].allPointSize() < k){
				//do it next loop
				i = tailList[layer];
				//lastSum += parentNumList[layer];
				layer++;
			}
			else{
				if(uGroups[layer].size == k){
					//uGroups[layer].PrintAsc();
					//result.push_back(new_ug);
					resultNum++;
				}
				//sibling point
				i--;
				//uGroups[layer].clearUGroup();
			}
		}
		else{
			i--;
			//parentNumList[layer]++;
		}
		while(i < 0){
			//向上退一层，应该删除layer-1这一层，否则下次要写在layer-1这一层，没有清空直接覆盖。
			i = tailList[layer - 1];
			//uGroups[layer-1].clearUGroup();
			//lastSum -= parentNumList[layer-1];
			//parentNumList[layer] = 0;
			layer--;
		}
	}
	//cout << "candidate groups:" << statCandidateNum << endl;
	//cout <<"uwise result count:" << resultNum << endl;
	return resultNum;
	return 0;
}

long long GSkyline::printAllLayer1(int stIndex, Point** all,int len,int k){
	if(stIndex < k - 1)
		return 0;
	long long count = 1;
	int* flags = new int[k];
	for(int i = 0; i < k; i++)
		flags[i] = stIndex-i;
	int sk = k-1;
	int k1 = k-1;
	while (true)
	{
		int sk = k-1;
		while(flags[sk] == k-1-sk){
			if(sk==0)
				return count;
			sk--;
		}
		flags[sk]--;
		if(sk != k1){
			for(int s = sk+1; s < k; s++){
				flags[s] = flags[s-1]-1;
			}
		}
		count++;
		//print all
		/*for(int s = 0; s < k; s++)
			cout << flags[s] << ",";
		cout << endl;*/
	}
}

long long GSkyline::printAllCombineLayer1(int stIndex, Point** all,int len,int k){
	if(stIndex < k - 1)
		return 0;
	long long count = 1;
	int* flags = new int[k];
	for(int i = 0; i < k; i++)
		flags[i] = stIndex-i;
	int sk = k-1;
	int k1 = k-1;
	while (true)
	{
		int sk = k-1;
		while(flags[sk] == k-1-sk){
			if(sk==0)
				return count;
			sk--;
		}
		flags[sk]--;
		if(sk != k1){
			for(int s = sk+1; s < k; s++){
				flags[s] = flags[s-1]-1;
			}
		}
		count++;
		//print all
		/*for(int s = 0; s < k; s++)
			cout << flags[s] << ",";
		cout << endl;*/
	}
}

long long GSkyline::UnitWisePlusPlus2(int k,Point** all,int len)
{
	//cout << "hehe" << printAllLayer1(10,NULL,0,4) << endl;
	//exit(0);
	if(len == 0)
		return 0;
	long long resultNum = 0;
	vector<UGroup> result;
	UGroup* uGroups = new UGroup[k+1];
	size_t startI = 0;
	int allLen = len;
	int layer = 1;
	int* tailList = new int[k+1];
	//int* parentNumList = new int[k+1];
	//memset(parentNumList, 0, sizeof(int)*(k+1));
	//int lastSum = 0;
	tailList[0] = allLen - 1;
	int tail = tailList[0];
	int i = tail;
	int leftAll;
	while(1){
		if(layer <= 0)
			break;
		if(all[i]->layer == 0){
			//int s = 0;
			if(layer == 1)
				return resultNum + printAllLayer1(i, all, len, k);
			//add feature
			UGroup* ug = &uGroups[layer-1];
			return resultNum + printAllCombineLayer1(i, all, len, k);

			//go back to up layer
			i = tailList[layer - 1];
			layer--;
		}

		//new_point
		Point* np = all[i];
		set<Point*>* ps = &(uGroups[layer - 1].allParentSet);
		if(ps->find(np) == ps->end()){
			//UGroup* new_ug = &uGroups[layer];
			//这里会不会有危险，直接覆盖的话。
			//TODO：vector直接覆盖会不会产生没有回收的内存
			uGroups[layer] = uGroups[layer - 1];
			uGroups[layer].insert(np);
			uGroups[layer].tail = i - 1;
			tailList[layer] = i - 1;
			
			if(uGroups[layer].allPointSize() < k){
				//do it next loop
				i = tailList[layer];
				//lastSum += parentNumList[layer];
				layer++;
			}
			else{
				if(uGroups[layer].size == k){
					//uGroups[layer].PrintAsc();
					//result.push_back(new_ug);
					resultNum++;
				}
				//sibling point
				i--;
				//uGroups[layer].clearUGroup();
			}
		}
		else{
			i--;
			//parentNumList[layer]++;
		}
		while(i < 0){
			//向上退一层，应该删除layer-1这一层，否则下次要写在layer-1这一层，没有清空直接覆盖。
			i = tailList[layer - 1];
			//uGroups[layer-1].clearUGroup();
			//lastSum -= parentNumList[layer-1];
			//parentNumList[layer] = 0;
			layer--;
		}
	}
	//cout << "candidate groups:" << statCandidateNum << endl;
	//cout <<"uwise result count:" << resultNum << endl;
	return resultNum;
	return 0;
}

void GSkyline::print_layers()
{
	for (int i = 0; i < layers.size(); i++)
	{
		cout << i<<" layer:" << endl;
		for (int j = 0; j < layers[i].size(); j++)
		{
			cout << "p" << layers[i][j]->id << " ";
		}
		cout << endl;
	}
	for (int i = 0; i < allPoints.size(); i++)
	{
		//
		cout << "p" << allPoints[i]->id << "'s children: ";
		for (int j = 0; j < allPoints[i]->cSet.size(); j++)
		{
			cout << "p" << allPoints[i]->cSet[j]->id << " ";
		}
		cout << endl;
	}
}


//add by kangrong
void GSkyline::sortPointsByLayer(){
	sort(this->allPoints.begin(), this->allPoints.end(), comparePointByLayer);
	for(int i = 0; i < allPoints.size(); i++)
		allPoints[i]->index = i;
}
vector<Group> GSkyline::preprocessing(int k){
	vector<Point*> temp;
	vector<Group> ret;
	int index = 0;
	for(vector<Point*>::iterator it = allPoints.begin(); it != allPoints.end(); it++){
		Point* p = *it;
		if (p->layer > k){
			//break;
			//remove p from its parent
			
			continue;
		}
		if(p->pSet.size() + 1 == k){
			Group ng;
			ng.pointSet.insert(p->pSet.begin(),p->pSet.end());
			ng.pointSet.insert(p);
			ret.push_back(ng);
		}
		else if(p->pSet.size() + 1 < k){
			p->index = index++;
			temp.push_back(p);
		}
		//if the size of unit (include point and its parent) is greater than k,delete
	}
	this->allPoints = temp;
	addFirstLayer(k);
	return ret;
}

void GSkyline::addFirstLayer(int k){
	int len = allPoints.size();
	int x,l,t,temp;
	for(int i = 0; i < len; i++){
		Point* p = allPoints[i];
		p->firstLayerIndex = new int[k];
		memset(p->firstLayerIndex,0,sizeof(int));
		x = 0;
		l = p->pSet.size();
		for(int s = 0; s < l; s++){
			if(p->pSet[s]->layer == 0){
				p->firstLayerIndex[x] = p->pSet[s]->index;
				//sort
				t = x;
				while(t > 0){
					if(p->firstLayerIndex[t] > p->firstLayerIndex[t-1]){
						temp = p->firstLayerIndex[t];
						p->firstLayerIndex[t] = p->firstLayerIndex[t-1];
						p->firstLayerIndex[t-1] = temp;
					}
				}
				x++;
			}
		}
	}
}
void Group::CalculateCS()
{
	ChildSet.clear();
	set<Point*>::iterator it;
	for (it = pointSet.begin(); it != pointSet.end(); it ++)
	{
		/*for each (Point* q in (*it)->cSet)
		{
			ChildSet.insert(q);
		}*/
		vector<Point*>::iterator cSetIt;
		for(cSetIt = (*it)->cSet.begin(); cSetIt != (*it)->cSet.end(); cSetIt++)
		{
			ChildSet.insert(*cSetIt);
		}
	}
}

bool Group::VerifyPoint(Point* p)
{
	if (p->isSkylinePoint)
		return true;
	//for each (Point* q in p->pSet)
	for(vector<Point*>::iterator it = p->pSet.begin(); it != p->pSet.end(); it++)
	{
		Point* q = *it;
		if (pointSet.find(q) == pointSet.end())
		{
			return false;
		}
	}
	return true;
}

void Group::Print()
{
	
	set<Point*>::iterator it = pointSet.begin();
	cout << "{";
	while (true)
	{
		cout << "p" << (*it)->id;
		it++;
		if (it != pointSet.end())
		{
			cout << ",";
		}
		else
		{
			break;
		}
	}
	cout << "}" << endl;
	
}
