#include <iostream>
#include "sudoku.h"
int main(){
	Sudoku sudoku;
	//sudoku.printGrid();
	std::cout << std::endl;
	int steps = sudoku.solve();
	sudoku.printGrid();
	if(steps!= -1){
		std::cout << "Solved in " << steps << " steps" << std::endl;
		//std::cout << steps << " ";
	}
	return 0;
}
