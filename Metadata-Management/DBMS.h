/*



*/

#ifndef DBMS_H
#define DBMS_H

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
		void sqlQueryParse(); //creates queue of queries. Error if bad syntax
		void sqlFileParse(); //creates queue of queries
			
		void saveDBMS();
		void saveRunMetrics();
		
		bool create(const string& dbName);
		bool drop(const string& dbName);

		bool runInShell = true;
		ifstream fin;
		string filepath;
		shared_ptr<string> shellCommand;
		queue<shared_ptr<tuple<COMMAND, string>>> queryQueue;

		uint numDB = 0;
		shared_ptr<Database> currentDB; //shared pointer to current database
		map<string, shared_ptr<Database>> databases;
};

#endif