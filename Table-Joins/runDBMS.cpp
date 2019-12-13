/*



*/

#ifndef RUNDBMS_CPP
#define RUNDBMS_CPP

//
#include <iostream>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <fstream>
#include <string>

#include <map>
#include <tuple>
#include <vector>
#include <queue>

//
#include "DBMS.h"

/*
   Main Function 

   (P4: Client/Server is file i/o. Server reads commands from file until exit. If transaction skip other users.)
   (server generates username file. client grabs one and posts db creation. server posts output in file.)
*/
int main(int argc, char** argv) {
	try {
		//Instantiates DBMS object, parses, and stores SQL queries if using file input
		std::ifstream fin;
		DBMS dbms(fin, argv);

		//make thread callback
		//Run shell if requested otherwise run sql fileExists
		std::string fileExists = argv[1];
		if(fileExists == "true") {
			while(dbms.usingShell()) {
				dbms.dbmsShell();
				dbms.runDBMS();
			}
		}
	} catch(int error) {
		processErrors(error);
	}

	return EXIT_SUCCESS;
}

#endif