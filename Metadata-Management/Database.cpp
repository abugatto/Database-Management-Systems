/*



*/

#ifndef DB_CPP
#define DB_CPP

#include "Database.h"

////////////////////////////////////////////////////////////////
//                      Database Functions                    //
////////////////////////////////////////////////////////////////

Database::~Database() {
	saveDB();
	saveDBMetrics();
}

void Database::create(const std::string& relName, const std::string& data) {
	//checks for existance
	if(relations.find(relName) != relations.end()) {
		std::cout << "\tERROR: Relation name [" << relName << "] is taken!" << std::endl;
		return;
	}

	//parse data into vector of string varname, string typename, and int size
	Relation::Attributes parsedData;

	//parse data into attribute tokens (var type(size), ..., var type(size))
	char* attr = std::strtok(const_cast<char*>(data.data()), ",;");
	bool first = true;
	while(attr) {
		std::string var, type;
		int size = 0;

		//if beginning of list
		std::string attribute = attr;
		if(first) {
			if(attribute[0] == '(') { //remove "("
				attribute = attribute.substr(1, attribute.length());
			} else if(attribute[1] == '(') { //remove "("
				attribute = attribute.substr(2, attribute.length());
			}

			first = false;
		}

		//if space first
		if(attribute[0] == ' ') {
			attribute = attribute.substr(1, attribute.length());
		}

		int len = attribute.length();
		if(attribute.substr(len-1, len) == ")") { //if end of list
			if(attribute.substr(len-2, len-1) == "))") { //if varchar
				attribute = attribute.substr(0, len-2);

				std::size_t it = attribute.find("("); //find iterator to "("
				size = std::stoi(attribute.substr(it+1, attribute.length()));
				attribute = attribute.substr(0, it); //removes size and "("
			} else { //if varchar
				attribute = attribute.substr(0, len-1); //remove ")"
			}
		}

		//parse attribute into (var,type,size)
		std::istringstream ss(attribute + ";");
		std::getline(ss, var, ' ');
		std::getline(ss, type, ';');

		parsedData.emplace_back(std::make_tuple(var, type, size));

		//get next attribute
		attr = std::strtok(nullptr, ",;");
	}

	//Instantiate the relation implicitly from temp tuple 
	Relation::Ptr rel(new Relation(parsedData));
	relations.emplace(relName, rel);

	std::cout << "\tRelation [" << relName << "] created." << std::endl;
}

void Database::drop(const std::string& relName) {
	//checks for existance
	if(relations.find(relName) == relations.end()) {
		std::cout << "\tERROR: Relation name [" << relName << "] does not exist!" << std::endl;
		return;
	}

	relations.erase(relName); //shared pointer should deconstruct on removal

	std::cout << "\tRelation [" << relName << "] removed." << std::endl;
}

void Database::select(const std::string& relName, const std::string& data) { //works for single
	//checks for existance
	if(relations.find(relName) == relations.end()) {
		std::cout << "\tERROR: Relation name [" << relName << "] does not exist!" << std::endl;
		return;
	}

	//parse data into attribute tokens (var type(size), ..., var type(size))
	Relation::Attributes parsedData;
	if(data != "*") {
		std::istringstream ss(data + ',');
		std::string var;
		while(std::getline(ss, var, ',')) {
			//if space first
			if(var[0] == ' ') {
				var = var.substr(1, var.length());
			}

			parsedData.emplace_back(std::make_tuple(var, "", 0));
		}

		// std::string var, type;
		// int size;
		// std::tie(var, type, size) = parsedData;
		// relations.at(relname)->printRelation();
	} else {
		relations.at(relName)->print(relName);
	}
}

void Database::alter(const std::string& relName, const std::string& data) { //Concatinate each tuple with new attribute
	//checks for existance
	if(relations.find(relName) != relations.end()) {
		std::cout << "\tERROR: Relation name [" << relName << "] does not exist!" << std::endl;
		return;
	}

	//parse data into attribute tokens (var type(size), ..., var type(size))
	std::istringstream ss(data + ";");

	//parse attribute into (var,type,size)
	std::string varName, varType;
	std::getline(ss, varName, ' ');
	std::getline(ss, varType, ';');

	//Add new Attribute
	relations.at(relName)->alterADD(std::make_tuple(varName, varType, 0));
}

////////////////////////////////////////////////////////////////
//                      	Save Functions                    //
//////////////////////////////////////////////////////////////// 

void Database::saveDB() {
	//OPEN FILESTREAM AND ADD EACH RELATION
	return;
}

void Database::saveDBMetrics() {
	return;
}

#endif
