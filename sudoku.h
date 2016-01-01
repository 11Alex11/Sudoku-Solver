#ifndef _SUDOKU_H
#define _SUDOKU_H
#include<string>
#include<iostream>
#include <vector>
#include "cell.h"
class Sudoku{
	int gridSize;
	Cell* grid[9][9];
	int steps;
	bool findSolution(std::vector<Cell*>& pq);
	void addConstraints(Cell* cell, int val=0);
	void removeConstraints(Cell* cell);
	int getConstraintLength(Cell* cell,int val);
	void resetConstraints(Cell * cell);
	bool forwardCheck(Cell* cell,int val);
	void setBlock(int& boxX,int& boxY,int row,int col);
	int constrainingVar(Cell* cell);
public:
	Sudoku();
	~Sudoku();
	bool checkValid(Cell* cell);
	void printGrid();
	int solve();
};

struct CellCompare{
	bool operator()(Cell* c1,  Cell* c2) const{
		return (c1->numConstraints()<c2->numConstraints());
	}
};


#endif