/*



*/

#ifndef RELATION_H
#define RELATION_H

//
#include <iostream>
#include <cstdlib>
#include <typeinfo>
#include <fstream>
#include <string>

#include <map>
#include <tuple>
#include <vector>
#include <queue>

//
#include "Tuple.h"

/*
	Relation class 
	-Uses Polumorphism to store variadic instances of RelationT in the same container 
	-Stored in memory as ->[attribute map | vector of address tuples]
*/

//Use polymorphism to make enclosures of variable Relations
class Relation {
	public:
		//Definition for Variables (name, type)
		typedef std::shared_ptr<Relation> Ptr;

		typedef std::tuple<std::string, std::string, int> Attribute;
		typedef std::vector<Attribute> Attributes;

		//Definition of Table
		typedef std::vector<Tuple> Table;

		Relation(const Attributes& vars);

		//Relational Algebra
			//Set Operations (overload)
				//Union (|)
				//Intersection (&)
				//Difference (-)
				//Cartesian Product (*)
			//Relational Operations 
				//selection(Vars)
				//projection(conditions)
				//division (/)
			//Set Functions
				//sum
				//average
				//count
				//any
				//max
				//min

		void insertTuple(Tuple& tuple);
		void alterADD(const Attribute& attribute); //add attribute & init zeros
		// void alterMODIFY();

		int getCardinality();
		int getDegree();
		Attributes getAttributes();

		//May need memory management for B-tree (mapreduce for distributed?)
		void print(const std::string& relName);
		// void printRelation(const std::vector<int> indices);
		void saveRelation(std::ifstream& fout);
		void saveRelationMetrics();

		friend void processErrors(const int& error);

	private:
		Attributes variables;
		Table relation;//use array of tuple smart pointers for table
		//use B-tree of TupleNode pointers for search
};

#endif

