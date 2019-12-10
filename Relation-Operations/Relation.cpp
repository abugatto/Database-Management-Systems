/*



*/

#ifndef RELATION_CPP
#define RELATION_CPP

#include "Relation.h"

////////////////////////////////////////////////////////////////
//                      Relation Functions                    //
////////////////////////////////////////////////////////////////

Relation::Relation(const Attributes& vars) {
	variables = vars;
}

void Relation::insertTuple(Tuple& tuple) {
	//check types
	bool validTuple = true;
	if(tuple.size() != variables.size()) {
		validTuple = false;
	}

	int it = 0;
	for(auto variable : variables) {
		if(tuple.at(it).getType() != std::get<1>(variable)) {
			validTuple = false;
		}

		it++;
	}

	// if() { if any variables not initialized init them
	// 	//make temp node to insert into each tuple
	// 	std::shared_ptr<TupleNode> temp;
	// 	if(varType == "bool") {
	// 		temp = std::make_shared<TupleNode>(BoolTupleNode());
	// 	} else if(varType == "int") {
	// 		temp = std::make_shared<TupleNode>(IntTupleNode());	
	// 	} else if(varType == "float") {
	// 		temp = std::make_shared<TupleNode>(FloatTupleNode());
	// 	} else if(varType == "char") {
	// 		temp = std::make_shared<TupleNode>(CharTupleNode(size));
	// 	} else if(varType == "varchar") {
	// 		temp = std::make_shared<TupleNode>(StringTupleNode(size));
	// 	}

	// 	//Insert node into each tuple
	// 	for(std::size_t i = 0; i < relation.size(); i++) {
	// 		relation.at(i).insertNode(temp);
	// 	}
	// }

	//Add or throw error
	if(validTuple) {
		relation.emplace_back(tuple);
	} else {
		std::cout << "\n\nERROR: INVALID TUPLE" << std::endl << std::endl;
	}
}

void Relation::alterADD(const Attribute& attribute) { //add attribute & init zeros
	//Insert into variables
	variables.emplace_back(attribute);
}

int Relation::getCardinality() {
	return relation.size(); //size of vector
}

int Relation::getDegree() {
	return variables.size(); //size of tuple
}

Relation::Attributes Relation::getAttributes() {
	return variables;
}

////////////////////////////////////////////////////////////////
//                  Save and Print Functions                  //
//////////////////////////////////////////////////////////////// 

void Relation::print(const std::string& relName) {
	//Print table data
	std::cout << "\t[" << relName << std::endl;
			//Print Table Formatting and Attributes
	std::cout << "\t\t[";
	for(std::size_t i = 0; i < variables.size(); i++) {
		std::string var, type;
		int size = 0;
		std::tie(var, type, size) = variables.at(i);

		std::cout << var << "|" << type;
		if(type == "char" || type == "varchar") {
			std::cout << "(" << std::to_string(size) << ")";
		}

		if(i == getDegree()-1) {
			std::cout << "]" << std::endl; 
		} else {
			std::cout << ", ";
		}
	}

	for(std::size_t i = 0; i < getCardinality(); i++) {
		relation.at(i).print();
	}

	std::cout << "\t]" << std::endl;
}

void Relation::saveRelation(std::ifstream& fout) { //assume that each tuple type is printable with cout
	return;
}

void Relation::saveRelationMetrics() {
	return;
}

#endif