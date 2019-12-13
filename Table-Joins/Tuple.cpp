/*



*/

#ifndef TUPLE_CPP
#define TUPLE_CPP

//
#include "Tuple.h"

////////////////////////////////////////////////////////////////

TupleNode::TupleNode(const std::string& type) {
	data.type = type;
}

TupleNode::TupleNode(const Data& input) {
	data = input;
}

void TupleNode::print() {
	if(data.type == "bool") {
		std::cout << data.b;
	} if(data.type == "int") {
		std::cout << std::to_string(data.i);
	} if(data.type == "float") {
		std::cout << std::to_string(data.f);
	} if(data.type == "char" || data.type == "varchar") {
		std::cout << data.s;
	}
}

bool TupleNode::operator==(const TupleNode& node) {
	Data temp = node.getData();
	if(data.type == temp.type) {
		if(data.type == "bool") {
			if(data.b == temp.b) {
				return true;
			} else {
				return false;
			}
		} if(data.type == "int") {
			if(data.i == temp.i) {
				return true;
			} else {
				return false;
			}
		} if(data.type == "float") {
			if(data.f == temp.f) {
				return true;
			} else {
				return false;
			}
		} if(data.type == "char" || data.type == "varchar") {
			if(data.s == temp.s) {
				return true;
			} else {
				return false;
			}
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////

//concat operator
Tuple Tuple::operator+(Tuple& tupl) {
	Tuple newTuple;
	for(std::size_t i = 0; i < this->size(); i++) {
		TupleNode::Ptr temp = std::make_shared<TupleNode>(this->at(i));

		 newTuple.insertNode(temp);
	}

	for(std::size_t i = 0; i < tupl.size(); i++) {
		TupleNode::Ptr temp = std::make_shared<TupleNode>(tupl.at(i));

		 newTuple.insertNode(temp);
	}

	return newTuple;
}

int Tuple::size() {
	return data.size();
}

TupleNode Tuple::at(const int& i) {
	return *data.at(i);
}

void Tuple::insertNode(TupleNode::Ptr& node) {
	data.emplace_back(node);
}

void Tuple::modifyNode(const int& idx, const TupleNode& node) {
	data.at(idx) = std::make_shared<TupleNode>(node);
}

void Tuple::swapNodes(const int& idx1, const int& idx2) {
	TupleNode::Ptr tempNode = data.at(idx1);
	data.at(idx1) = data.at(idx2);
	data.at(idx2) = tempNode;
}

void Tuple::print(const std::vector<int>& indices) {
	std::cout << "[";
	for(std::size_t i = 0; i < data.size(); i++) {
		//if current index not banned
		bool isBanned = false;
		for(std::size_t j = 0; j < indices.size(); j++) {
			if(indices.at(j) == i) {
				isBanned = true;
				break;
			}
		}

		if(!isBanned) {
			data.at(i)->print();

			if(i == data.size()-1) {
				std::cout << "]";;
			} else {
				std::cout << "|";
			}
		}
	}
}

#endif