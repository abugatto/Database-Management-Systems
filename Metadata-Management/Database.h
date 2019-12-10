/*



*/

#ifndef DB_H
#define DB_H

//
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#include <map>
#include <tuple>
#include <vector>
#include <queue>

//
#include "Relation.h"

/*
   Database Class

   Stored in memory as ->[map of key=string and val=[Relation address]]
*/

class Database {
	public:
		//Shared Pointer
		typedef std::shared_ptr<Database> Ptr;

		//Database declaration
		Database() {}
		~Database();

		void create(const std::string& relName, const std::string& data);
		void drop(const std::string& relName);
		void select(const std::string& relName, const std::string& data);
		void alter(const std::string& relName, const std::string& data); //create new table and copy addresses (use tuple concat and tie)

		//Relational Algebra Operations (using relation names)
			//joinInner
			//joinOuter
			//joinTheta(condiitons)

		void saveDB();
		void saveDBMetrics();

		friend void processErrors(const int& error);

	private:
		//I/O streams
		std::ifstream fin;
		std::ofstream fout;

		//Definition for Relation Class (pointer because large database can take up all the memory)
		std::map<std::string, Relation::Ptr> relations; //Pointer for polymorphism of different relations
};

#endif