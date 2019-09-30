/*



*/

#ifndef DB_CPP
#define DB_CPP

#include "Database.h"

////////////////////////////////////////////////////////////////
//                      Database Functions                    //
//////////////////////////////////////////////////////////////// 

Database::Database() {
	//initialize data to dummy so it doesn't error
	relations.emplace("uninitialized", RelationT<string>("uninitialized"));
}

Database::create(const string& relName, const string& data) {
	//If first relation delete dummy 
	if(relations.find("uninitialized") != relations.end()) {

	}

	//parse data into vector of string varname, type string, and int size
	vector<string, string, int> data;

	while() {

	}

	//create a concatinated tuple of the variables and add type to vector
	for(int i = 0; i < numVars; i++) { //create dummy tuple of input form and make shared_ptr from that
		if(type == "int") {

		} else if(type == "float") {

		} else if(type == "char") {

		} else if(type == "varchar") { //string type

		} else {
			throw																					;
		}
	}

	//use concatinated tuple and decltype(tuple) to instantiate relation


	//Instantiate the relation implicitly from temp tuple 
	relations.emplace(relName, Relation<decltype(data)>);
}

Database::drop(const string& relName) {
	//checks for existance
	if(relations.find(relName) == relations.end()) {
		throw																					UNINIT;
		return;
	}

	relations.erase(relName); //shared pointer should deconstruct on removal
}

Database::select(const string& relName, const string& data) { //works for single
	//checks for existance
	if(relations.find(relName) == relations.end()) {
		throw																					UNINIT;
		return;
	}

	//Check for what data is requested
	if(data == "*") {
		//print all data from relation
		relations.at(relName)->printRelation();
	} else {
		throw																					INVALID;
	}/*else {
		//parse query


		//Process Query


		// //print all requesteddata from relation
		relations.at(relName)->printRelation();
	}*/
}

Database::alter(const string& relName, const string& data) { //create new table and copy addresses
	//checks for existance
	if(relations.find(relName) == relations.end()) {
		throw																					UNINIT;
		return;
	}

	//parse new data
	string varName = 
	string varType = 

	//Get types of relation and add new type
	auto types = relations.at(relName)->getTypes();

	types->emplace(varName, varType);

	//Concat null tuple of type varType into each tuple of relation

}

////////////////////////////////////////////////////////////////
//                      	Save Functions                    //
//////////////////////////////////////////////////////////////// 

void Database::saveDB() {
	//OPEN FILESTREAM AND ADD EACH RELATION

}

void Database::saveDBMetrics() {
	return;
}

#endif
