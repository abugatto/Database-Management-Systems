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
   Enumeration for join operations
*/

enum JOIN {NAJ, INNER, OUTER, LEFT_OUTER, RIGHT_OUTER};

/*
	Enumeration for query comparisons
*/

enum CONDITION {NAN, EQ, NEQ, GR, GREQ, LSEQ, LS};

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

		//definition of attributes
		typedef std::tuple<std::string, std::string, int> Attribute;
		typedef std::vector<Attribute> Attributes;

		//definition of single conditions (run in print or save statement)
		typedef std::tuple<std::string, CONDITION, Data> Condition;
		typedef std::shared_ptr<Condition> ConditionPtr;
		typedef std::vector<ConditionPtr> Conditions;

		//definitions of join conditions (requires creation of new table)
		typedef std::tuple<std::string, std::string, CONDITION, std::string, std::string> JoinCondition;
		typedef std::shared_ptr<JoinCondition> JoinConditionPtr;
		typedef std::vector<JoinCondition> JoinConditions;

		//Definition of Table
		typedef std::vector<Tuple::Ptr> Table;

		Relation(const Attributes& vars);

		int getCardinality();
		int getDegree();
		Attributes getAttributes();
		Table getRelation();

		void alterADD(const Attribute& attribute); //add attribute & init zeros
		// void alterREMOVE(const std::vector<std::string>& varNames, const Condition& condition);

		void insertTuple(Tuple& tuple);
		void update(const std::string relName, const Condition& set, const Condition& condition);
		void remove(const std::string relName, const Condition& condition);

		Relation::Ptr join(const JOIN& join, const JoinCondition& condition, const Relation::Ptr& rel);

		//May need memory management for B-tree (mapreduce for distributed?)
		void print(const std::string& relName, const std::vector<std::string>& varNames, const Condition& condition);
		// void printRelation(const std::vector<int> indices);
		void saveRelation(std::ifstream& fout, const std::string& relName, const std::vector<std::string>& varNames, const Condition& condition);
		void saveRelationMetrics();

		friend void processErrors(const int& error);

	private:
		std::shared_ptr<Attribute> primaryKey;
		Attributes variables;
		Table relation;//use array of tuple smart pointers for table
		//use B-tree of TupleNode pointers for search
};

#endif

