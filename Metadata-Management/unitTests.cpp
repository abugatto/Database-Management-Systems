#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

#include <map>
#include <tuple>
#include <vector>
#include <queue>

/////////////////////////////////////////////////////////////////////////////////

//typedefs
enum COMMAND {READ, USE, CREATE_DB, CREATE_TB, DROP_DB, DROP_TB, SELECT, ALTER, EXIT};

typedef std::tuple<COMMAND, std::string> Query;
typedef std::shared_ptr<Query> QueryPtr;

typedef std::tuple<std::string, std::string, int> Attribute;
typedef std::shared_ptr<Attribute> AttributePtr;
typedef std::vector<AttributePtr> AttributeVector;

typedef std::shared_ptr<std::string> StringPtr;
typedef std::vector<StringPtr> StringList;

//Parse test func decls
AttributeVector sqlSelectParse(std::string& data);
AttributeVector sqlAlterParse(std::string& data);
AttributeVector sqlTableParse(const std::string& data);

void sqlQueryParse(const std::string&);
void sqlFileParse(std::ifstream& fin, const std::string& filename);

void printQueue();
int processErrors(const int& error);

//globalvars
std::string sqlFilename = "PA1_test.sql";
std::queue<QueryPtr> queryQueue; //If nested statements than may need stack for 'recursion'

/////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
	//parse file
	try {
		std::ifstream fin;
		sqlFileParse(fin, sqlFilename);

		//parse and edit data arrays for printing
		AttributeVector attributes;
		for(std::size_t i = 0; i < queryQueue.size(); i++) {
			QueryPtr query = queryQueue.front();
			if(std::get<0>(*query) == CREATE_TB) {
				attributes = sqlTableParse(std::get<1>(*query));
			} else if(std::get<0>(*query) == SELECT) {
				attributes = sqlSelectParse(std::get<1>(*query));
			} else if(std::get<0>(*query) == ALTER) {
				attributes = sqlAlterParse(std::get<1>(*query));
			}

			queryQueue.pop(); //pop to edit for printing

			if(std::get<0>(*query) == CREATE_TB) {
				std::string tbName = std::strtok(const_cast<char*>(std::get<1>(*query).data()), " "); //must be parsed to the beginning

				//std::string attributeStr = "";
				std::get<1>(*query) = "\n\t\t\ttable:\t" + tbName + "\n\t\t\tattributes:\t";

				for(std::size_t i = 0; i < attributes.size(); i++) {
					AttributePtr attribute = attributes.at(i);
					std::get<1>(*query) = std::get<1>(*query) + "\n\t\t\t\t|" + std::get<0>(*attribute) + "|" + std::get<1>(*attribute) + "|" + std::to_string(std::get<2>(*attribute)) + "|";
				}
			} else if(std::get<0>(*query) == SELECT) {
				std::string tbName = std::strtok(const_cast<char*>(std::get<1>(*query).data()), " "); //must be parsed to the beginning

				//std::string attributeStr = "";
				std::get<1>(*query) = "\n\t\t\ttable:\t" + tbName + "\n\t\t\tattributes:\t";

				for(std::size_t i = 0; i < attributes.size(); i++) {
					AttributePtr attribute = attributes.at(i);
					std::get<1>(*query) = std::get<1>(*query) + "\n\t\t\t\t|" + std::get<0>(*attribute) + "|" + std::get<1>(*attribute) + "|" + std::to_string(std::get<2>(*attribute)) + "|";
				}
			} else if(std::get<0>(*query) == ALTER) {
				std::string tbName = std::strtok(const_cast<char*>(std::get<1>(*query).data()), " "); //must be parsed to the beginning

				//std::string attributeStr = "";
				std::get<1>(*query) = "\n\t\t\ttable:\t" + tbName + "\n\t\t\tattributes:\t";

				for(std::size_t i = 0; i < attributes.size(); i++) {
					AttributePtr attribute = attributes.at(i);
					std::get<1>(*query) = std::get<1>(*query) + "\n\t\t\t\t|" + std::get<0>(*attribute) + "|" + std::get<1>(*attribute) + "|" + std::to_string(std::get<2>(*attribute)) + "|";
				}
			}

			queryQueue.emplace(query); //push print friendly query back into queue
		}
	} catch(int error) {
		return processErrors(error);
	}

	//create structures
	printQueue();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////

AttributeVector sqlSelectParse(std::string& data) {
	//parse data into vector of string varname, string typename, and int size
	AttributeVector parsedData;

	//parse data into attribute tokens (var type(size), ..., var type(size))
	std::istringstream SS(data + ";");

	std::string attr;
	std::getline(SS, attr, 'T'); //GET RID OF Tbname/SELECT
	std::getline(SS, attr, ';');
	attr = attr + ",";
	if(attr.find("*") == std::string::npos) {
		std::istringstream ss(attr);
		std::string var;
		while(std::getline(ss, var, ',')) {
			//if space first
			if(var[0] == ' ') {
				var = var.substr(1, var.length());
			}

			parsedData.emplace_back(std::make_shared<Attribute>(std::tuple<std::string, std::string, int>(var, "", 0)));
		}

		return parsedData;
	} else {
		std::string var = "*";
		parsedData.emplace_back(std::make_shared<Attribute>(std::tuple<std::string, std::string, int>(var, "", 0)));

		return parsedData;
	}
}

AttributeVector sqlAlterParse(std::string& data) {
	//parse data into attribute tokens (var type(size), ..., var type(size))
	std::istringstream ss(data + ";");

	std::getline(ss, data, ' '); //NOT IN OOP
	std::getline(ss, data, ' '); //NOT IN OOP
	std::getline(ss, data, ' '); //NOT IN OOP

	//parse attribute into (var,type,size)
	std::string var, type;
	std::getline(ss, var, ' ');
	std::getline(ss, type, ';');

	std::cout << "|" << var << "|" << type << "|" << std::endl;

	//parse data into vector of string varname, string typename, and int size
	AttributeVector parsedData;
	parsedData.emplace_back(std::make_shared<Attribute>(std::tuple<std::string, std::string, int>(var, type, 0)));

	return parsedData;
}

AttributeVector sqlTableParse(const std::string& data) {
	//parse data into vector of string varname, string typename, and int size
	AttributeVector parsedData;

	//parse data into attribute tokens (var type(size), ..., var type(size))
	char* attr = std::strtok(const_cast<char*>(data.data()), " ,;");
	attr = std::strtok(nullptr, ",;"); //get rid of tb name (NOT FOR OOP)
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
		std::istringstream ss(attribute);
		std::getline(ss, var, ' ');
		std::getline(ss, type, ';');

		//std::cout << "|" << var << "|" << type << "|" << size << "|" << std::endl << std::endl;
		parsedData.emplace_back(std::make_shared<Attribute>(std::tuple<std::string, std::string, int>(var, type, size)));

		//get next attribute
		attr = std::strtok(nullptr, ",;");
	}

	return parsedData;
}

//inputs one line at a time
void sqlQueryParse(const std::string& input) { //creates queue of queries
	//Parse into queries and throw away comments
	//parse query command and data and into tuple
			//take care of end comments
			//take care of lack of ';'
			//take care of "EXIT."
	COMMAND command;
	std::string queryCommand, data;
	std::tuple<std::string, std::string> parsedData;

	//get queryCommand from input
	std::istringstream ss(input);
	std::getline(ss, queryCommand, ' ');
	if(queryCommand == "READ") {
		command = READ;
	} else if(queryCommand == "USE") {
		command = USE;
	} else if(queryCommand == "CREATE") {
		std::string createCommand;
		std::getline(ss, createCommand, ' ');

		//Check if creating database or table
		if(createCommand == "DATABASE") {
			command = CREATE_DB;
		} else if(createCommand == "TABLE") {
			command = CREATE_TB;
		} else { //if neither
			throw -3;
		}
	} else if(queryCommand == "DROP") {
		std::string dropCommand;
		std::getline(ss, dropCommand, ' ');

		//Check if dropping database or table
		if(dropCommand == "DATABASE") {
			command = DROP_DB;
		} else if(dropCommand == "TABLE") {
			command = DROP_TB;
		} else { //if neither
			throw -3;
		}
	} else if(queryCommand == "SELECT") {
		command = SELECT;
	} else if(queryCommand == "ALTER") {
		std::getline(ss, data, ' ');
		std::getline(ss, data, ';');											std::cout << data << std::endl;

		command = ALTER;
	} else if(queryCommand == "EXIT") {
		command = EXIT;
	} else {
		throw -3;
	}

	//get rest of query
	if(command != EXIT) {
		if(command == CREATE_TB) {
			//parse table name and data tokens
			std::string tbName;
			std::getline(ss, tbName, ' ');	
			std::getline(ss, data, ';');

			data = tbName + " " + data;
		} else if(command == SELECT) {
			//find desired attributes and tbName
			std::size_t it = input.find("F");
			data = input.substr(0, it-1);
			std::string tbName = input.substr(it+4, input.length());

			data = tbName + " " + data;
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
	queryQueue.emplace(std::make_shared<Query>(std::make_tuple(command, data)));
}

void sqlFileParse(std::ifstream& fin, const std::string& filename) { //creates queue of queries
	fin.clear();
	fin.open(sqlFilename);
	if(sqlFilename.find(".sql") == -1) { //checks file extension
		throw -1;
	}

	if(fin.is_open() == false) { //checks for open file
   		throw -2;
	}

	//parse into a list of queries
	std::queue<std::string> lineQueue;

	bool exitLoop = false;
	while(!exitLoop) {
		//read in line until end line
		std::string input;
		std::getline(fin, input);
		if(input[0] == 13) { //ASCII 13 is carriage return
			continue; 
		}

		//check for exit command
		if(input.find(".EXIT") != std::string::npos) {
			lineQueue.emplace("EXIT ;"); 
			break;
		}

		//Check for comment or empty line
		std::string comment = input.substr(0,2);
		while(comment == "--" || (input[0] == 13)) {
			std::getline(fin, input);
			comment = input.substr(0,1);

			//check for exit command
			if(input.find(".EXIT") != std::string::npos) {
				lineQueue.emplace("EXIT ;"); 
				exitLoop = true;
			}
		}

		//if multiline query (comments filtered out by now)
		while(input.find(";") == std::string::npos) {
			std::string temp = input;
			std::getline(fin, input);

			//Check for comment or empty line
			std::string comment = input.substr(0,2);
			while(comment == "--" || (input[0] == 13)) {
				std::getline(fin, input);
				comment = input.substr(0,1);

				//check for exit command
				if(input.find(".EXIT") != std::string::npos) {
					lineQueue.emplace("EXIT ;");
					exitLoop = true;
				}
			}

			input = temp + input; 
		}

		//if line isnt exit break into queries
		if(!exitLoop) {
			//break into queries
			char* queryToken = std::strtok(const_cast<char*>(input.data()), ";");
			while(queryToken != nullptr && queryToken[0] != 13) {
				//append query to queue
				lineQueue.emplace((std::string) queryToken + ";"); 

				//Get next query if available
				queryToken = strtok(nullptr, ";");
			}
		}
	}

	//parse queries into queue
	while(!lineQueue.empty()) {
		sqlQueryParse(lineQueue.front());
		lineQueue.pop();
	}

	fin.close();
	fin.clear();
}

void printQueue() {
	std::cout << "Query queue:" << std::endl;
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

		std::cout << "\tQuery #" << i++ << ":\n\t\tcommand = " << queryStr << "\n\t\tdata = " << dataStr << std::endl;
	}
}

int processErrors(const int& error) {
	if(error == -1) {
		std::cout << "ERROR CODE -1: SQL FILE NOT FOUND" << std::endl;
		return EXIT_FAILURE;
	}

   	if(error == -2) {
    	std::cout << "ERROR CODE -2: INVALID SQL FILE EXTENSION" << std::endl;
    	return EXIT_FAILURE;
   	}

  	if(error == -3) {
    	std::cout << "ERROR CODE -3: INVALID SQL COMMAND" << std::endl;
    	return EXIT_FAILURE;
  	}

	if(error == -4) {
    	std::cout << "ERROR CODE -4: EMPTY QUEUE" << std::endl;
    	return EXIT_FAILURE;
  	}

   return EXIT_FAILURE;
}