/*



*/

#ifndef RUNDBMS_CPP
#define RUNDBMS_CPP

//
#include <iostream>
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
*/
int main(int argc, char** argv) {
	try {
		//Instantiates DBMS object, parses, and stores SQL queries if using file input
		std::ifstream fin;
		DBMS dbms(fin, argv);

		//Run shell if requested otherwise run sql file
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