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
#include 


//set namespace
using namespace std;

/*
   Main Function
*/
int main(int argc, char** argc) {
	//Instantiates DBMS object, parses, and stores SQL queries if using file input
	ifstream fin;
	DBMS dbms(fin, argv);

	//Run shell if requested otherwise run sql file
	while(dbms.usingShell()) {
		dbms.dbmsShell();
		dbms.run();
	}

	return 0;
}

#endif