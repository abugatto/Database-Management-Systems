/*



*/

#ifndef RELATION_CPP
#define RELATION_CPP

#include "Relation.h"

////////////////////////////////////////////////////////////////
//                      Relation Functions                    //
////////////////////////////////////////////////////////////////

template<typename Tuple>
RelationT::RelationT(const std::tuple<Args...>& data, const Database::AttributesPtr& vars) {
	relation.push_back(data);
	attributes = vars;
}

int RelationT::getCardinality() {
	return relation.size(); //size of vector
}

int RelationT::getDegree() {
	return attributes.size(); //size of tuple
}

Database::Variables RelationT::getVariables() {
	return variables;
}

////////////////////////////////////////////////////////////////
//                  Save and Print Functions                  //
//////////////////////////////////////////////////////////////// 

void RelationT::printRelation() { //assume that each tuple type is printable with cout
	//Print Table Formatting and Attributes


	//Print Table Data
	if() {
		
	}
}

void RelationT::saveRelation() { //assume that each tuple type is printable with cout
	//CURRENTLY JUST SAVE VARIABLES

}

void RelationT::saveRelationMetrics() {
	return;
}

////////////////////////////////////////////////////////////////
//                   Tuple Print Functions                    //
////////////////////////////////////////////////////////////////

template<class Tuple, std::size_t N>
static void RelationT::TuplePrinter::print(const Tuple& t) {
	TuplePrinter<Tuple, N-1>::print(t);
	std::cout << ", " << std::get<N-1>(t);
}
 
template<class Tuple>
static void RelationT::TuplePrinter::print(const Tuple& t) {
    std::cout << std::get<0>(t);
}

template<class... Args>
void RelationT::printTuple(const std::tuple<Args...>& t) {
    std::cout << "(";
    TuplePrinter<decltype(t), sizeof...(Args)>::print(t); //recursively prints each attribute of the tuple
    std::cout << ")\n";
}

#endif