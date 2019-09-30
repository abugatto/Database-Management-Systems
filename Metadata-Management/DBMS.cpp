/*



*/

#ifndef DBMS_CPP
#define DBMS_CPP

#include "DBMS.h"

////////////////////////////////////////////////////////////////
//       Database Management System (DBMS) Functions          //
////////////////////////////////////////////////////////////////

DBMS::DBMS(ifstream& fin, const char *argv[]) {
	//initialize variables


	//parse command line arguments (filename)


	//if file input then parse and run --USE FOR TESTING

}

DBMS::~DBMS() { //saves databases to file
	//save data to disk

}

bool DBMS::usingShell() {
	return runInShell;
}

void DBMS::dbmsShell() { //Run within each shell loop

}

/*
	-Processes and executes parsed query queue
	-runs DBMS or DB depending on state
*/
void DBMS::runDBMS() { //runs after every shell command
	//if the queue is empty throw exception
	if(queryQueue.empty()) {
		throw																				; print error
	}

	//process and execute queue
	for(int i = 0; i < queryQueue.size(); i++) {
		//retrieve and pop query from queue
		QueryPtr query = queryQueue.front();
		queryQueue.pop();

		//for each state execute command and parse accordingly
		COMMAND command = get<0>(*query);
		if(command == USE) {
			//parse DB name from data
			string dbName = 															get<1>(*query)

			//set current DB
			currentDB = make_shared<Database>(databases.at(dbName));
		} else if(command == CREATE_DB) {
			create(get<1>(*query));
		} else if(command == CREATE_TB) {
			//break to shell if database is not loaded 
			if(!currentDB) {
																							; print error
				break;
			}

			//parse table name and data
																							get<1>(*query)
			string tbName = 
			string data = 

			//create table in database
			currentDB->create(tbName, data);
		} else if(command == DROP_DB) {
			drop(get<1>(*query));
		} else if(command == DROP_TB) {
			//break to shell if database is not loaded 
			if(!currentDB) {
																							; print error
				break;
			}

			//parse table name and data
																							get<1>(*query)
			string tbName = 
			string data = 

			//drop table in database
			currentDB->drop(tbName, data);
		} else if(command == SELECT) {
			//parse data (* in this case)
			string data = 

			//parse table name
			string tbName = 

			//load table and print 
			currentDB->select(tbName, data);
		} else if(command == ALTER) {
			//parse table name
			string tbName = 
			string data = 

			//alter table (create new table with alteration)
			currentDB->alter(tbName, data);
		} else if(command == EXIT) {
			runInShell = false; //will cause shell loop to end
			break;
		} else {
			throw 																			; print error
		}
	}
}

void DBMS::sqlQueryParse() { //creates queue of queries

}

void DBMS::sqlFileParse() { //creates queue of queries

}
			
void DBMS::saveDBMS() {
   ofstream fout; //open file
   fout.clear();
   fout.open(filepath);


}

void DBMS::saveRunMetrics() {
   ofstream fout; //open file
   fout.clear();
   fout.open(filepath);


}
		
bool DBMS::create(const string& dbName) {
	if(databases.find(dbName) == databases.end()) {
		throw																				REPEATED name
	}

	databases.emplace(dbName, make_shared<Database>(new Database()));
}

bool DBMS::drop(const string& dbName) {
	databases.erase(dbName);
}

#endif