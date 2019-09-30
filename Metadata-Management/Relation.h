/*



*/

#ifndef RELATION_H
#define RELATION_H

//
#include <iostream>
#include

/*
   Relation class 

   Stored in memory as ->[attribute map | vector of address tuples]
*/

//Use polymorphism to make enclosures of variable Relations
class Relation {
	public:
		typedef std::shared_ptr<Table> Ptr;
};

template<class Args...> //shared pointers of types
class RelationT : public Relation { //dynamic table of different types (abstracts away types
	public:
		RelationT(const std::tuple<Args...>& data, const Database::AttributesPtr& vars);

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

		int getCardinality();
		int getDegree();
		Database::AttributesPtr getVariables();

		void printRelation();
		void saveRelation();
		void saveRelationMetrics();

	private:
		//Declare relation data
		Database::Attributes attributes; //(varname, type)
		std::vector<std::tuple<Args...>> relation; //use array of tuple smart pointers for table

		//Tuple print helper functions for recursive print function
		template<class Tuple, std::size_t N>
		struct TuplePrinter {
			static void print(const Tuple& t); //static allows access without instance
		};

		template<class Tuple>
		struct TuplePrinter<Tuple, 1> {
    		static void print(const Tuple& t);
		};

		//uses recursive helper struct to print tuple
		void printTuple(const std::tuple<Args...>& t);
};

#endif