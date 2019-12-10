/*



*/

#ifndef TUPLE_H
#define TUPLE_H

//
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>

#include <map>
#include <tuple>
#include <vector>
#include <queue>

/*
	Tuple class
*/

////////////////////////////////////////////////////////////////

struct Data {
	std::string type;
	bool b;
	int i;
	float f;
	std::string str;
};

class TupleNode {
	public:
		typedef std::shared_ptr<TupleNode> Ptr;

		TupleNode(const std::string& type);
		TupleNode(const Data& input);
		~TupleNode() {}

		Data getData() const { return data; };
		std::string getType() { return data.type; }
		std::ostream& operator<<(std::ostream& cout);
		bool operator==(const TupleNode& node);

	protected:
		Data data;
};

////////////////////////////////////////////////////////////////

class Tuple { //Nodes are immutable
	public: 
		int size();
		TupleNode at(const int& i);
		void insertNode(TupleNode::Ptr& node);
		void modifyNode(const int& idx, const TupleNode& node);
		void swapNodes(const int& idx1, const int& idx2);
		void print();

		std::vector<TupleNode::Ptr> data;
};

#endif