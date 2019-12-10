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

std::ostream& TupleNode::operator<<(std::ostream& cout) {
	if(data.type == "bool") {
		cout << data.b;
	} if(data.type == "int") {
		cout << data.i;
	} if(data.type == "float") {
		cout << data.f;
	} if(data.type == "char" || data.type == "varchar") {
		cout << data.str;
	} else {
		throw -5;
	}

	return cout;
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
			if(data.str == temp.str) {
				return true;
			} else {
				return false;
			}
		}
	}

	return false;
}

////////////////////////////////////////////////////////////////

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

void Tuple::print() {
	std::cout << "\t[";
	for(std::size_t i = 0; i < data.size(); i++) {
		std::cout << data.at(i);

		if(i != data.size()) {
			std::cout << " ";
		} else {
			std::cout << "]" << std::endl;
		}
	}
}

#endif