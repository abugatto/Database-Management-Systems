/*



*/

#ifndef DBMS_H
#define DBMS_H

//
#include 

using namespace std;

/*
   Enumerated type for SQL commands
*/

enum COMMAND {USE, CREATE_DB, CREATE_TB, DROP_DB, DROP_TB, SELECT, ALTER, EXIT};

/*
   Database Management System (DBMS) Class
*/

class DBMS {
	public:
		DBMS(ifstream& fin, const char *argv[]);
		~DBMS(); //saves databases to file

		bool usingShell();
		void dbmsShell();
		void runDBMS(); //runs after every shell command

	private:
		void sqlQueryParse(); //creates queue of queries. Error if bad syntax (later will parse nested statements and put on stack)
		void sqlFileParse(); //creates queue of queries
			
		void saveDBMS();
		void saveRunMetrics();
		
		bool create(const string& dbName);
		bool drop(const string& dbName);

		bool runInShell = true;
		ifstream fin;
		std::string filepath;

		typedef std::tuple<COMMAND, std::string> Query;
		typedef std::shared_ptr<Query> QueryPtr;
		std::queue<QueryPtr> queryQueue; //If nested statements than may need stack for 'recursion'

		uint numDB = 0;
		Database::Ptr currentDB; //shared pointer to current database
		std::map<std::string, Database> databases;
};

#endif