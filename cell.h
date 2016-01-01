#ifndef _CELL_H
#define _CELL_H
#include<string>
#include<iostream>
#include <set>
class Cell{
	int row;
	int col;
	int val;
	bool variable;
	std::set<int> constraints;
public:
	Cell(int x,int y,int val);
	int numConstraints(int v = 0);
	void setVal(int v);
	int getVal();
	bool isVariable();
	int getRow();
	int getCol();
	std::set<int>* getConstraints();
	void addConstraint(int con);
	void removeConstraint(int con);
};

#endif