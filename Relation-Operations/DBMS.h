/*



*/

#ifndef DBMS_H
#define DBMS_H

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
#include "Database.h"

void processErrors(const int& error);

/*
   Enumerated type for SQL commands
*/

enum COMMAND {READ, USE, CREATE_DB, CREATE_TB, DROP_DB, DROP_TB, SELECT, ALTER, EXIT};

/*
   Database Management System (DBMS) Class
*/

class DBMS {
	public:
		DBMS(std::ifstream& fin, char** argv);
		~DBMS(); //saves databases to file

		bool usingShell();
		void dbmsShell();
		void runDBMS(); //runs after every shell command

		friend void processErrors(const int& error);

	private:
		void sqlQueryParse(const std::string& input); //creates queue of queries in real time. Error if bad syntax (later will parse nested statements and put on stack)
		void sqlFileParse(); //creates queue of queries from file

		void saveDBMS();
		void loadDBMS();
		void saveRunMetrics();
		
		void create(const std::string& dbName);
		void drop(const std::string& dbName);

		//I/O streams
		std::ifstream fin;
		std::ofstream fout;

		//parameters
		int runInShell = 0;
		std::string sqlFilepath;
		std::string saveFilepath;
		std::string metricsFilepath;

		//Query strictures
		typedef std::tuple<COMMAND, std::string, std::string> Query;
		typedef std::shared_ptr<Query> QueryPtr;
		std::queue<QueryPtr> queryQueue; //If nested statements than may need stack for 'recursion'

		//DB structures
		Database::Ptr currentDB; //shared pointer to current database
		std::map<std::string, Database::Ptr> databases;
};

#endif