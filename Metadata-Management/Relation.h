/*



*/

#ifndef RELATION_H
#define RELATION_H

//
#include 

/*
   Table class
*/
class Relation { //dynamic table of different types (abstracts away types
	public:
		template<typename ... Args>
		Relation(const string& tbName, Args...);

		int getCardinality();
		int getDegree();

		void printRelation();
		void saveRelation();
		void saveRelationMetrics();

	private: //use array of tuple smart pointers for table
		//represented as vector of tuples
		template<typename ... Args>
		vector<shared_ptr<tuple<Args...>>> relation;
};

#endif