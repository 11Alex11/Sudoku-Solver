#include "cell.h"

Cell::Cell(int x,int y,int val):row(x),col(y),val(val){
	variable=true;
	//If value is 0, then this is a variable
	if(val!=0){
		variable=false;
	}
}

int Cell::numConstraints(int val){
	int ret = constraints.size();
	//The heap comparator will not have a val to check, so return just the length
	if(val==0){
		return ret;
	}

	if(constraints.find(val)==constraints.end()){
		ret++;
	}
	return ret;
}

void Cell::setVal(int value){
	val=value;
}

int Cell::getVal(){
	return val;
}

int Cell::getRow(){
	return row;
}

int Cell::getCol(){
	return col;
}

void Cell::addConstraint(int con){
	constraints.insert(con);
}

void Cell::removeConstraint(int con){
	constraints.erase(con);
}

bool Cell::isVariable(){
	return variable;
}


std::set<int>* Cell::getConstraints(){

	return &constraints;
}