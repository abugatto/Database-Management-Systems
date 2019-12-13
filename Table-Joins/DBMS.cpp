/*



*/

#ifndef DBMS_CPP
#define DBMS_CPP

//
#include "DBMS.h"

////////////////////////////////////////////////////////////////
//       Database Management System Friend Functions          //
////////////////////////////////////////////////////////////////

void processErrors(const int& error) {
	if(error == -1) {
		std::cout << "ERROR CODE -1: SQL FILE NOT FOUND" << std::endl;
	}

   	if(error == -2) {
    	std::cout << "ERROR CODE -2: INVALID SQL FILE EXTENSION" << std::endl;
   	}

  	if(error == -3) {
    	std::cout << "ERROR CODE -3: INVALID SQL COMMAND" << std::endl;
  	}

	if(error == -4) {
    	std::cout << "ERROR CODE -4: EMPTY QUEUE" << std::endl;
  	}

  	if(error == -5) {
    	std::cout << "ERROR CODE -6: INVALID JOIN" << std::endl;
  	}
}

////////////////////////////////////////////////////////////////
//       Database Management System (DBMS) Functions          //
////////////////////////////////////////////////////////////////

DBMS::DBMS(std::ifstream& fin, char** argv) {
	//Parse args
	sqlFilepath = argv[2];
	saveFilepath = argv[3];
	metricsFilepath = argv[4];

	std::string fileExists = argv[1];
	if(fileExists == "false") {
		sqlFileParse();
		runDBMS();
	}
}

DBMS::~DBMS() { //saves databases to file
	saveRunMetrics();
	saveDBMS();
}

bool DBMS::usingShell() {
	return runInShell >= 0;
}

void DBMS::dbmsShell() { //Run within each shell loop
	// //Print Intro statement
	// if(runInShell == 0) {
	// 	std::cout << "SQL V0.2:\tData Manipulation" << std::endl;
	// 	std::cout << "\t\tType SQL queries only!" << std::endl;
	// }

	// //Command Prompt
	// std::cout << "sql>\t";

	// //parse input into a list of queries
	// std::vector<std::string> lineQueue;

	// //Input Command (quit parse at newline)
	// std::string input;
	// std::getline(std::cin, input, '\n');

	// //check for exit command
	// if(input.find(".EXIT") != std::string::npos || input.find(".exit") != std::string::npos) {
	// 	std::cout << std::endl << "PROGRAM EXITING..." << std::endl;
	// 	runInShell = -1;
	// 	return;
	// }

	// //break into queries
	// char* queryToken = std::strtok(const_cast<char*>(input.data()), ";");
	// while(queryToken != nullptr && queryToken[0] != 13) {
	// 	//append query to queue
	// 	lineQueue.emplace_back((std::string) queryToken + ";"); 

	// 	//Get next query if available
	// 	queryToken = strtok(nullptr, ";");
	// }

	// //parse queries into queue
	// while(!lineQueue.empty()) {
	// 	sqlQueryParse(lineQueue.front());
	// 	lineQueue.erase(lineQueue.begin());
	// }

	// //Increment Shell
	// runInShell++;

	sqlQueryParse("read");
}

/*
	-Processes and executes parsed query queue
	-runs DBMS or DB depending on state
*/
void DBMS::runDBMS() { //runs after every shell command
	//if the queue is empty throw exception
	if(queryQueue.empty()) {
		throw -4;
	}

	//process and execute queue
	while(!queryQueue.empty()) {
		//retrieve and pop query from queue
		QueryPtr query = queryQueue.front();
		queryQueue.pop();

		//If not in shell
		if(runInShell == -1) {
			std::cout << "sql>\t" << std::get<2>(*query) << std::endl;
		}

		//for each state execute command and parse accordingly
		COMMAND command = std::get<0>(*query);
		if(command == READ) {
			//parse data for filename and set sqlFilepath
			if(std::get<1>(*query).length() != 0) {
				sqlFilepath = std::get<1>(*query);
			}

			std::cout << "\tReading [" << sqlFilepath << "]..." << std::endl;

			//read file
			runInShell = -1;
			sqlFileParse();
		} else if(command == USE) {
			std::string dbName = std::get<1>(*query);
			if(databases.find(dbName) == databases.end()) {
				std::cout << "\tERROR: Database name [" << dbName << "] does not exist!" << std::endl;
				break;
			} else {
				//set current DB as shared ptr
				currentDB = databases.at(dbName);

				std::cout << "\tUsing [" << dbName << "]." << std::endl;
			}
		} else if(command == CREATE_DB) {
			create(std::get<1>(*query));
		} else if(command == CREATE_TB) {
			//break to shell if database is not loaded 
			if(!currentDB) {
				std::cout << "\tERROR: Database not loaded;" << std::endl;
				break;
			}

			//parse table name and data tokens
			std::istringstream ss(std::get<1>(*query) + ";");

			std::string tbName, data;
			std::getline(ss, tbName, ' ');
			std::getline(ss, data, ';');

			//create table in database
			currentDB->create(tbName, data);
		} else if(command == DROP_DB) {
			drop(std::get<1>(*query));
		} else if(command == DROP_TB) {
			//break to shell if database is not loaded 
			if(!currentDB) {
				std::cout << "\tERROR: Database not loaded;" << std::endl;
				break;
			}

			//drop table in database
			currentDB->drop(std::get<1>(*query));
		} else if(command == INSERT) { 
			//break to shell if database is not loaded 
			if(!currentDB) {
				std::cout << "\tERROR: Database not loaded;" << std::endl;
				break;
			}

			//parse relName
			std::istringstream ss(std::get<1>(*query) + ";");

			std::string tbName, data;
			std::getline(ss, tbName, '|');
			std::getline(ss, data, ';');

			//insert tuple
			currentDB->insert(tbName, data);
		} else if(command == SELECT) {
			//break to shell if database is not loaded 
			if(!currentDB) {
				std::cout << "\tERROR: Database not loaded;" << std::endl;
				break;
			}

			//format before ss
			std::string q = std::get<1>(*query) + "@";
			if(q[0] == ' ') {
				q = q.substr(1,q.length());
			}

			//parse tbName and data
			std::istringstream ss(q);

			std::string tbNames, data, junk;
			std::getline(ss, tbNames, '|');
			std::getline(ss, data, '@');

			//load table and print 
			currentDB->select(tbNames, data);
		} else if(command == UPDATE) {
			//break to shell if database is not loaded 
			if(!currentDB) {
				std::cout << "\tERROR: Database not loaded;" << std::endl;
				break;
			}

			//parse relName
			std::istringstream ss(std::get<1>(*query) + ";");

			std::string tbName, data;
			std::getline(ss, tbName, '|');
			std::getline(ss, data, ';');

			//update relation tuple values
			currentDB->update(tbName, data);
		} else if(command == DELETE) { 
			//break to shell if database is not loaded 
			if(!currentDB) {
				std::cout << "\tERROR: Database not loaded;" << std::endl;
				break;
			}

			//parse relName
			std::istringstream ss(std::get<1>(*query) + ";");

			std::string tbName, data;
			std::getline(ss, tbName, '|');
			std::getline(ss, data, ';');

			//remove tuple from relation
			currentDB->remove(tbName, data);
		} else if(command == ALTER) {
			//break to shell if database is not loaded 
			if(!currentDB) {
				std::cout << "\tERROR: Database not loaded;" << std::endl;
				break;
			}

			std::string q = std::get<1>(*query) + ";";
			if(q[0] == ' ') {
				q = q.substr(1,q.length());
			}

			//parse table name
			std::istringstream ss(q);

			std::string tbName, operation, data;
			std::getline(ss, tbName, ' ');
			std::getline(ss, operation, ' ');
			std::getline(ss, data, ';');

			//alter table (create new table with alteration)
			currentDB->alter(tbName, data);
		} else if(command == EXIT) {
			std::cout << "\tExiting shell..." << std::endl;
			runInShell = -1; //will cause shell loop to end
			break;
		} else {
			throw -3;
		}

		std::cout << std::endl;
	}
}

void DBMS::sqlQueryParse(const std::string& input) { //creates queue of queries
	//Parse into queries and throw away comments
	COMMAND command;
	std::string queryCommand, data;
	std::tuple<std::string, std::string> parsedData;

	//get queryCommand from input
	std::istringstream ss(input);
	std::getline(ss, queryCommand, ' ');
	if(queryCommand.find("READ") != std::string::npos || input.find("read") != std::string::npos) {
		command = READ;
	} else if(queryCommand.find("USE") != std::string::npos || queryCommand.find("use") != std::string::npos) {
		command = USE;
	} else if(queryCommand.find("CREATE") != std::string::npos || queryCommand.find("create") != std::string::npos) {
		std::string createCommand;
		std::getline(ss, createCommand, ' ');											

		//Check if creating database or table
		if(createCommand.find("DATABASE") != std::string::npos || createCommand.find("database") != std::string::npos) {
			command = CREATE_DB;
		} else if(createCommand.find("TABLE") != std::string::npos || createCommand.find("table") != std::string::npos) {
			command = CREATE_TB;
		} else { //if neither
			throw -3;
		}
	} else if(queryCommand.find("DROP") != std::string::npos || queryCommand.find("drop") != std::string::npos) {
		std::string dropCommand;
		std::getline(ss, dropCommand, ' ');

		//Check if dropping database or table
		if(dropCommand.find("DATABASE") != std::string::npos || dropCommand.find("database") != std::string::npos) {
			command = DROP_DB;
		} else if(dropCommand.find("TABLE") != std::string::npos || dropCommand.find("table") != std::string::npos) {
			command = DROP_TB;
		} else { //if neither
			throw -3;
		}
	} else if(queryCommand.find("INSERT") != std::string::npos || queryCommand.find("insert") != std::string::npos) {
		command = INSERT;
	} else if(queryCommand.find("SELECT") != std::string::npos || queryCommand.find("select") != std::string::npos) {
		command = SELECT;
	} else if(queryCommand.find("UPDATE") != std::string::npos || queryCommand.find("update") != std::string::npos) {
		command = UPDATE;
	} else if(queryCommand.find("DELETE") != std::string::npos || queryCommand.find("delete") != std::string::npos) {
		command = DELETE;
	} else if(queryCommand.find("ALTER") != std::string::npos || queryCommand.find("alter") != std::string::npos) {
		std::getline(ss, data, ' ');
		std::getline(ss, data, ';');

		command = ALTER;
	} else if(queryCommand == ".EXIT") {
		command = EXIT;
	} else {
		throw -3;
	}

	//get rest of query
	if(command != EXIT) {
		if(command == CREATE_TB) {
			//parse table name and data tokens
			std::string tbName;
			std::getline(ss, tbName, '(');
			std::getline(ss, data, ';');

			data = tbName + " " + data;
		} else if(command == INSERT) {
			//parse table name and data tokens
			std::string into, tbName, values;
			std::getline(ss, into, ' ');
			std::getline(ss, tbName, ' ');
			std::getline(ss, values, '(');
			std::getline(ss, data, ')');

			data = tbName + "|" + data;
		} else if(command == SELECT) {
			//find desired attributes and tbName
			std::size_t it = 0;
			if(input.find("F") != std::string::npos) {
				it = input.find("F");
			} else if(input.find("f") != std::string::npos) {
				it = input.find("f");
			}

			data = input.substr(0, it-1);
			if(data.find("select") != std::string::npos) {
				std::size_t itS = data.find("select");
				data = data.substr(itS+7, data.length());
			}

			std::string tbName = input.substr(it+4, input.length());
			std::string cond = "";
			if(tbName.find("where") != std::string::npos) {
				std::size_t itW = tbName.find("where");
				cond = tbName.substr(itW+6, tbName.length()) + "|";
				tbName = tbName.substr(0, itW-2);
			}else if(tbName.find("on") != std::string::npos) {
				std::size_t itO = tbName.find("on");
				cond = tbName.substr(itO+3, tbName.length()) + "|";
				tbName = tbName.substr(0, itO-2);
			}

			data = tbName + "|" + cond + data;
		} else if(command == UPDATE) {
			//parse table name and data tokens
			std::string tbName, var, cond;
			std::getline(ss, data, ';');
			
			std::size_t itS = 0;
			if(data.find("set") != std::string::npos) {
				itS = data.find("set");
			}

			tbName = data.substr(0, itS-1);

			std::size_t itW = 0;
			if(data.find("where") != std::string::npos) {
				itW = data.find("where");
			}

			var = data.substr(itS+4, itW-14);

			data = tbName + "|" + var + "|" + data.substr(itW+6, data.length());
		} else if(command == DELETE) {
			//parse table name and data tokens
			std::string tbName, var, cond;
			std::getline(ss, data, ' '); //from
			std::getline(ss, data, ';');

			std::size_t itW = 0;
			if(data.find("where") != std::string::npos) {
				itW = data.find("where");
			}

			tbName = data.substr(0, itW-1);

			data = tbName + "|" + data.substr(itW+6, data.length());
		} else if(command == ALTER) {
			//parse table name
			std::string tbName;
			std::getline(ss, tbName, ' '); //get table name
			std::getline(ss, data, ' ');
			std::getline(ss, data, ';');

			data = tbName + " " + data;
		} else {
			std::getline(ss, data, ';');

			//delete carriage return char if present
			for(std::size_t i = 0; i < data.length(); i++) {
				if(data[i] == 13) {
					data[i] = ' ';
				}
			}
		}
	} else {	
		if(command != READ) {
			data = "N/A";
		}
	}

	//Insert query into query queue
	queryQueue.emplace(std::make_shared<Query>(std::make_tuple(command, data, input)));
}

void DBMS::sqlFileParse() { //creates queue of queries
	fin.clear();
	fin.open(sqlFilepath);
	if(sqlFilepath.find(".sql") == -1) { //checks file extension
		throw -1;
	}

	if(fin.is_open() == false) { //checks for open file
   		throw -2;
	}

	//parse into a list of queries
	std::queue<std::string> lineQueue;

	for(;;) {
		//read in line until end line
		std::string input;
		std::getline(fin, input);
		if(input[0] == '\n' || input[0] == 13 || input.substr(0,2) == "--") { //ASCII 13 is carriage return
			continue; 
		}

		//check for exit command
		if(input.find(".EXIT") != std::string::npos || input.find(".exit") != std::string::npos) {
			lineQueue.emplace(".EXIT"); 
			break;
		}

		//if multiline query (comments filtered out by now)
		if(input.find(";") == std::string::npos) {
			std::string temp;
			std::getline(fin, temp, ';');

			//concat input w/o newline and rest of line
			input = input.substr(0, input.length()-1) + " " + temp;

			//remove spaces and newlines
			for(std::size_t i = 0; i < input.length(); i++) {
				if(input[i] == 13 || input[i] == '\n' || input[i] == '\t') {
					input[i] = ' ';
				}

				if(i+1 != input.length()-1 && input.substr(i,i+1) == "  ") {
					input = input.substr(0,i) + input.substr(i+2,input.length());
				}
			}
		}

		//remove spaces and newlines
		for(std::size_t i = 0; i < input.length(); i++) {
			if(input[i] == '\n') {
				input[i] = ' ';
			}

			if(i+1 != input.length()-1 && input.substr(i,i+1) == "  ") {
				input = input.substr(0,i) + input.substr(i+2,input.length());
			}

			if(input[i] == '\t') {
				input[i] = ' ';

				if(i+1 != input.length()-1 && input[i+1] == '\t') {
					input = input.substr(0,i) + input.substr(i+2,input.length());
				}
			}
		}

		//push query onto queue
		lineQueue.emplace(input);
	}

	//parse queries into queue
	while(!lineQueue.empty()) {
		sqlQueryParse(lineQueue.front());
		lineQueue.pop();
	}

	fin.close();
	fin.clear();
}
			
void DBMS::saveDBMS() {
	// fout.clear();
	// fout.open(saveFilepath);

	// //Save db for loading later


	// fout.clear();
	// fout.close();
	return;
}

void DBMS::loadDBMS() {
	// fin.clear();
	// fin.open(saveFilepath);

	// //Load DB
	

	// fin.clear();
	// fin.close();
	return;
}

void DBMS::saveRunMetrics() {
	fout.clear();
	fout.open(metricsFilepath);

	fout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;
	fout << "||||||||||||||||Pending Query Queue||||||||||||||||||" << std::endl;
	fout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl << std::endl;

	//Save Metadata
	//////SAVE TIME

	fout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;
	fout << "||||||||||||||||||||Database Log|||||||||||||||||||||" << std::endl;
	fout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl << std::endl;

	//Save Databases (saves in destructor)
	while(!databases.empty()) {
		databases.erase(databases.begin());
	}

	//Print Pending Queue Queries
	fout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;
	fout << "||||||||||||||||Pending Query Queue||||||||||||||||||" << std::endl;
	fout << "|||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl << std::endl;
   	fout << "Query queue:" << std::endl;
	int i = 0;
	while(!queryQueue.empty()) {
		QueryPtr query = queryQueue.front();
		queryQueue.pop();

		std::string queryStr, dataStr;
		if(std::get<0>(*query) == READ) {
			queryStr = "READ";
			dataStr = std::get<1>(*query);
		} else if(std::get<0>(*query) == USE) {
			queryStr = "USE";
			dataStr = std::get<1>(*query);
		} else if(std::get<0>(*query) == CREATE_DB) {
			queryStr = "CREATE_DB";
			dataStr = std::get<1>(*query);
		} else if(std::get<0>(*query) == CREATE_TB) {
			queryStr = "CREATE_TB";
			dataStr = std::get<1>(*query);
		} else if(std::get<0>(*query) == DROP_DB) {
			queryStr = "DROP_DB";
			dataStr = std::get<1>(*query);
		} else if(std::get<0>(*query) == DROP_TB) {
			queryStr = "DROP_TB";
			dataStr = std::get<1>(*query);
		} else if(std::get<0>(*query) == SELECT) {
			queryStr = "SELECT";
			dataStr = std::get<1>(*query);
		} else if(std::get<0>(*query) == ALTER) {
			queryStr = "ALTER";
			dataStr = std::get<1>(*query);
		} else if(std::get<0>(*query) == EXIT) {
			queryStr = "EXIT";
			dataStr = "N/A";
		}

		fout << "\tQuery #" << i++ << ":\n\t\tcommand = " << queryStr << "\n\t\tdata = " << dataStr << std::endl;
	}

	fout.clear();
	fout.close();
}
		
void DBMS::create(const std::string& dbName) {
	if(databases.find(dbName) != databases.end()) {
		std::cout << "\tERROR: Database name [" << dbName << "] taken!" << std::endl;
		return;
	}

	Database::Ptr db(new Database);
	databases.emplace(dbName, db);

	std::cout << "\tDatabase [" << dbName << "] created." << std::endl;
}

void DBMS::drop(const std::string& dbName) {
	if(databases.find(dbName) == databases.end()) {
		std::cout << "\tERROR: Database [" << dbName << "] does not exist!" << std::endl;
		return;
	}

	databases.erase(dbName);

	std::cout << "\tDatabase [" << dbName << "] removed." << std::endl;
}

#endif