#include "sudoku.h"
#include <iostream>
#include "cell.h"
#include <queue>
#include <vector>
#include <map>
Sudoku::Sudoku():gridSize(9),steps(0){
	for(int i=0;i<gridSize;i++){
		int temp;
		for(int j=0; j<gridSize;j++){
			std::cin >> temp;
			grid[i][j]= new Cell(i,j,temp);	
		}
	}
}

//determines position of block
void Sudoku::setBlock(int& boxX,int& boxY,int row,int col){
	if(row<3){
		boxX=0;
	}
	else if(row<6){
		boxX=3;
	}
	else{
		boxX=6;
	}
	if(col<3){
		boxY=0;
	}
	else if(col<6){
		boxY=3;
	}
	else{
		boxY=6;
	}
}

//checks if setting the cell to the val will invalidate its row/col/block cells it affects
//return false if value will lead to no solution, otherwise return true
bool Sudoku::forwardCheck(Cell* cell,int val){
	int boxX;
	int boxY;
	int row=cell->getRow();
	int col = cell->getCol();
	//Determine which of the 9 3X3 subgrids to check
	setBlock(boxX,boxY,row,col);
	for(int i = 0;i<gridSize;i++){

		//check row/col/block and check if their constraint size is 9 and the value does not already exist
		//if so, it will lead to an incorrect solution

		if(grid[i][col]!=cell&&grid[i][col]->numConstraints()==gridSize-1){
			if(grid[i][col]->getConstraints()->find(val)==grid[i][col]->getConstraints()->end()&&grid[i][col]->getVal()==0){
				return false;
			}
	
		}
		if(grid[row][i]!=cell&&grid[row][i]->numConstraints()==gridSize-1){
			if(grid[row][i]->getConstraints()->find(val)==grid[row][i]->getConstraints()->end()&&grid[row][i]->getVal()==0){
				return false;
			}
		}
	}

	for(int i=boxX;i<boxX+3;i++){
		for(int j=boxY;j<boxY+3;j++){
			if(grid[i][j]!=cell&&grid[i][j]->numConstraints()==gridSize-1){
				if(grid[i][j]->getConstraints()->find(val)==grid[i][j]->getConstraints()->end()&&grid[i][j]->getVal()==0){
					return false;
				}
			}

		}
	}
	return true;
}



//called by removeConstraint, recalculates constraints
void Sudoku::resetConstraints(Cell * cell){
	int boxX;
	int boxY;
	int row=cell->getRow();
	int col = cell->getCol();
	//Determine which of the 9 3X3 subgrids to check
	setBlock(boxX,boxY,row,col);

	for(int i = 0;i<gridSize;i++){
		//add constraints to the given cell
		if(grid[i][col]->getVal()!=0){
			cell->addConstraint(grid[i][col]->getVal());
		}
		if(grid[row][i]->getVal()!=0){
			cell->addConstraint(grid[row][i]->getVal());
		}

	}

	for(int i=boxX;i<boxX+3;i++){
		for(int j=boxY;j<boxY+3;j++){
			if(grid[i][j]->getVal()!=0){
				cell->addConstraint(grid[i][j]->getVal());
			}
		}
	}
}

//this function is used to determine the least constraining value given the value
int Sudoku::getConstraintLength(Cell* cell,int val){
	int boxX;
	int boxY;
	int row=cell->getRow();
	int col=cell->getCol();
	int constraintNum=0;
	//Determine which of the 9 3X3 subgrids to check
	setBlock(boxX,boxY,row,col);
	for(int i = 0;i<gridSize;i++){
		if(grid[i][col]->getVal()==0){
			constraintNum+=grid[i][col]->numConstraints(val);
		}
		if(grid[row][i]->getVal()==0){
			constraintNum+=grid[row][i]->numConstraints(val);
		}
	}

	for(int i=boxX;i<boxX+3;i++){
		for(int j=boxY;j<boxY+3;j++){
			if(grid[i][j]->getVal()==0){
				constraintNum+=grid[i][j]->numConstraints(val);
			}

		}
	}
	return constraintNum;
}
//prints the current state of the grid (unsolved or solved)
void Sudoku::printGrid(){
	for(int i=0;i<gridSize;i++){
		for(int j=0; j<gridSize;j++){
			std::cout << grid[i][j]->getVal();
			if(gridSize-1!=j){
				std::cout << " | ";
			}
		}
		std::cout << std::endl;
		for(int k=0;k<gridSize-1;k++){
			std::cout << "-- -";
		}
		std::cout << "--";
		std::cout << std::endl;
	}
}

void Sudoku::addConstraints(Cell * cell,int val){
	int boxX;
	int boxY;
	cell->setVal(val);
	int row=cell->getRow();
	int col = cell->getCol();

	//Determine which of the 9 3X3 subgrids to check
	setBlock(boxX,boxY,row,col);

	for(int i = 0;i<gridSize;i++){
		//value of 0 initializes the cells constraints

		//check row and columns
		if(val==0){
			if(!grid[i][col]->isVariable()){
				cell->addConstraint(grid[i][col]->getVal());
			}
			if(!grid[row][i]->isVariable()){
				cell->addConstraint(grid[row][i]->getVal());
			}
		}
		else{
			if(grid[i][col]->isVariable()){
				grid[i][col]->addConstraint(val);
			}
			if(grid[row][i]->isVariable()){
				grid[row][i]->addConstraint(val);
			}
		}
	}

	//check blocks
	for(int i=boxX;i<boxX+3;i++){
		for(int j=boxY;j<boxY+3;j++){
			if(val==0){
				if(!grid[i][j]->isVariable()){
					cell->addConstraint(grid[i][j]->getVal());
				}
			}
			else{
				if(grid[i][j]->isVariable()){
					grid[i][j]->addConstraint(val);
				}
			}
		}
	}
}


void Sudoku::removeConstraints(Cell * cell){
	int boxX;
	int boxY;
	int row=cell->getRow();
	int col = cell->getCol();
	//Determine which of the 9 3X3 subgrids to check
	setBlock(boxX,boxY,row,col);

	//remove the constraint of the cell we are reseting to those affected
	for(int i = 0;i<gridSize;i++){
			grid[i][col]->removeConstraint(cell->getVal());
			grid[row][i]->removeConstraint(cell->getVal());
			
	}
	for(int i=boxX;i<boxX+3;i++){
		for(int j=boxY;j<boxY+3;j++){
			grid[i][j]->removeConstraint(cell->getVal());
		}
	}
	cell->removeConstraint(cell->getVal());
	cell->setVal(0);
	//since we may have a cell with more than one constraint, we have to recalculate
	for(int i = 0;i<gridSize;i++){	
			resetConstraints(grid[i][col]);
			resetConstraints(grid[row][i]);	
	}
	for(int i=boxX;i<boxX+3;i++){
		for(int j=boxY;j<boxY+3;j++){
			resetConstraints(grid[i][j]);
		}
	}

}

//calculates most constraining variable
int Sudoku::constrainingVar(Cell* cell){
	int boxX;
	int boxY;
	int row=cell->getRow();
	int col=cell->getCol();
	int constraintNum=0;
	//Determine which of the 9 3X3 subgrids to check
	setBlock(boxX,boxY,row,col);
	for(int i = 0;i<gridSize;i++){
		if(grid[i][col]->getVal()==0){
			constraintNum++;
		}
		if(grid[row][i]->getVal()==0){
			constraintNum++;
		}
	}

	for(int i=boxX;i<boxX+3;i++){
		for(int j=boxY;j<boxY+3;j++){
			if(grid[i][j]->getVal()==0){
				constraintNum++;
			}

		}
	}
	return constraintNum;
}

//main loop of the solver
bool Sudoku::findSolution(std::vector<Cell*>& pq){
		if(steps>10000){
			return false;
		}
		//if no cells left on stack, all are filled so return
		if(pq.empty()){
			return true;
		}
		//pop the most constraining variable off
		Cell* cell =pq.front();
		std::pop_heap(pq.begin(),pq.end(),CellCompare());  
		pq.pop_back();
		std::make_heap(pq.begin(),pq.end(),CellCompare());
		//if there is a tie, use the least constraining variable heuristic
		std::vector<Cell*> tieBreakers;
		int constraints = cell->numConstraints();
		while(!pq.empty()){
			Cell* next = pq.front();
			if(next->numConstraints()!=constraints){
				break;
			}
			if(constrainingVar(next)>constrainingVar(cell)){
				tieBreakers.push_back(cell);
				cell=next;
			}
			else{
				tieBreakers.push_back(next);
			}
			std::pop_heap(pq.begin(),pq.end(),CellCompare());  
			pq.pop_back();
			std::make_heap(pq.begin(),pq.end(),CellCompare());
		}
		for(int i=0;i<tieBreakers.size();i++){
			pq.push_back(tieBreakers[i]);
		}
		//calculate least constraining values
		//key is constrained value, value is sudoku number
		std::multimap<int,int> lCon;
		std::set<int>::iterator it=cell->getConstraints()->begin();
		for(int i=0;i<gridSize;i++){
			if(it==cell->getConstraints()->end()||(i+1)!=*it){
				//forward checking, do not consider if it makes row,col or block cells it affects not solvable
				if(forwardCheck(cell,i+1)){
					lCon.insert(std::make_pair(getConstraintLength(cell,i+1),i+1));
				}
			}
			else if((i+1)== *it){
				it++;
			}
		}
		//iterate over each possible value, recursing on this function
		std::multimap<int,int>::reverse_iterator rit;
		for(rit = lCon.rbegin();rit!=lCon.rend();++rit){
			steps++;

			addConstraints(cell,(*rit).second);
			std::make_heap(pq.begin(),pq.end(),CellCompare());
			//solution found, return true
			if(findSolution(pq)){
				return true;
			}
			removeConstraints(cell);
		}
		//if no values were valid, push the cell back in the priority queue and return to parent node
		pq.push_back(cell);
		std::make_heap(pq.begin(),pq.end(),CellCompare());
		return false;
}

int Sudoku::solve(){
	steps=0;
	//treat vector as priority queue of variables, this is responsible for choosing most constrained variable
	std::vector<Cell*> pq;
	for(int i=0;i<gridSize;i++){
		for(int j=0;j<gridSize;j++){
			//Only add cells that can change to the queue
			if(grid[i][j]->isVariable()){
				pq.push_back(grid[i][j]);
			}
		}
	}
	//initialize constraints for variable cells
	std::vector<int> constraints;
	for(int i=0;i<pq.size();i++){
		addConstraints(pq[i]);
	}
	//turn vector into max heap using CellCompare function for comparator, compares constraint lengths
	std::make_heap(pq.begin(),pq.end(),CellCompare());
	//start looping process
	bool x = findSolution(pq);

	if(x==false){
		std::cout << "Error:Exceed 10,000 steps or no solution"<<std::endl;
		//std::cout << steps <<std::endl;
	}
	return steps;
}




//delete cells that were allocated
Sudoku::~Sudoku(){
	for(int i=0;i<gridSize;i++){
		for(int j=0;j<gridSize;j++){
			delete grid[i][j];
		}
	}
}






