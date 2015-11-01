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

GSkyline::GSkyline(string filename)
{
	this->load(filename);
	this->SortPoints();
	this->BuildDSG();
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
			if (!(*(layers[0][layers[0].size() - 1])).isDomain(*p))
			{
				p->layer = 0;
				p->isSkylinePoint = true;
				layers[0].push_back(p);
			}
			else if ((*(layers[layers.size() - 1][layers[layers.size() - 1].size() - 1])).isDomain(*p))
			{
				p->layer = ++maxlayer;
				p->isSkylinePoint = false;
				vector<Point*>layer;
				layer.push_back(p);
				layers.push_back(layer);
			}
			else
			{
				//binary search
				int l = 0, r = maxlayer, m;
				while (l < r)
				{
					m = (l + r) >> 1;
					if ((*(layers[m][layers[m].size() - 1])).isDomain(*p))
					{
						l = m + 1;
					}
					else
					{
						r = m;
					}
				}
				p->layer = r;
				p->isSkylinePoint = false;
				layers[l].push_back(p);

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

vector<Group> GSkyline::UnitWise(int k)
{
	throw exception("unimplemented");
	return vector<Group>();
}

vector<Group> GSkyline::UnitWisePlus(int k)
{
	return vector<Group>();
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

void Group::CalculateCS()
{
	ChildSet.clear();
	set<Point*>::iterator it;
	for (it = pointSet.begin(); it != pointSet.end(); it ++)
	{
		for each (Point* q in (*it)->cSet)
		{
			ChildSet.insert(q);
		}
	}
}

bool Group::VerifyPoint(Point* p)
{
	if (p->isSkylinePoint)
		return true;
	for each (Point* q in p->pSet)
	{
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
