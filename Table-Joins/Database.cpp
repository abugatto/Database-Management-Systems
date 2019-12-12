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
			if(attribute.find("(") != std::string::npos) {
				std::size_t it = attribute.find("("); //find iterator to "("
				size = std::stoi(attribute.substr(it+1, attribute.length()));
				attribute = attribute.substr(0, it); //removes size and "("
			} else {
				attribute = attribute.substr(0, attribute.length()-1);
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
	if(data != "*") {
		//parse data
		std::string condStr;
		std::string varStr;

		std::istringstream ss(data);
		std::getline(ss, condStr, '|');
		std::getline(ss, varStr, '|');

		//build variable list
		std::vector<std::string> varNames;

		std::istringstream ssV(varStr + ",");
		std::string attrStr = ",";
		while(std::getline(ssV, attrStr, ',')) {
			varNames.emplace_back(attrStr);
		}

		//Build where condition
		Relation::Attributes variables = relations.at(relName)->getAttributes();

		std::string attr, val;
		CONDITION cond = NAN;
		Data dataC;
		if(condStr.find("!=") != std::string::npos) {
			cond = NEQ;

			std::size_t it = condStr.find("!=");
			attr = condStr.substr(0, it);
			val = condStr.substr(it+2, condStr.length());		

			for(std::size_t i = 0; i < variables.size(); i++) {
				Relation::Attribute var = variables.at(i);
				if(std::get<0>(var).find(attr) != std::string::npos) {
					if(std::get<1>(var) == "bool") {
						dataC.type = "bool";
						if(val == "1" || val == "true") {
							dataC.b = true;
						} else {
							dataC.b = false;
						}
					} else if(std::get<1>(var) == "int") {
						dataC.type = "int";
						dataC.i = std::stoi(val);
					} else if(std::get<1>(var) == "float") {
						dataC.type = "float";
						dataC.f = std::stof(val);
					} else if(std::get<1>(var) == "char" || std::get<1>(var) == "varchar") {
						dataC.type = "varchar";
						dataC.s = val.substr(1, val.length()-3);
					}

					break;
				}
			}
		} else if(condStr.find("=") != std::string::npos) {
			cond = EQ;

			std::size_t it = condStr.find("=");
			attr = condStr.substr(0, it);
			val = condStr.substr(it+1, condStr.length());

			for(std::size_t i = 0; i < variables.size(); i++) {
				Relation::Attribute var = variables.at(i);
				if(std::get<0>(var).find(attr) != std::string::npos) {
					if(std::get<1>(var).find("bool") != std::string::npos) {
						dataC.type = "bool";
						if(val == "1" || val == "true") {
							dataC.b = true;
						} else {
							dataC.b = false;
						}
					} else if(std::get<1>(var).find("int") != std::string::npos) {
						dataC.type = "int";
						dataC.i = std::stoi(val);
					} else if(std::get<1>(var).find("float") != std::string::npos) {
						dataC.type = "float";
						dataC.f = std::stof(val);
					} else if(std::get<1>(var).find("char") != std::string::npos || std::get<1>(var).find("varchar") != std::string::npos) {
						dataC.type = "varchar";
						dataC.s = val.substr(1, val.length()-3);
					}

					break;
				}
			}
		} else if(condStr.find("<=") != std::string::npos) {
			cond = LSEQ;

			std::size_t it = condStr.find("<=");
			attr = condStr.substr(0, it);
			val = condStr.substr(it+2, condStr.length());

			for(int i = 0; i < variables.size(); i++) {
				Relation::Attribute var = variables.at(i);
				if(std::get<0>(var).find(attr) != std::string::npos) {
					if(std::get<1>(var) == "int") {
						dataC.type = "int";
						dataC.i = std::stoi(val);
					} else if(std::get<1>(var) == "float") {
						dataC.type = "float";
						dataC.f = std::stof(val);
					}

					break;
				}
			}
		} else if(condStr.find("<") != std::string::npos) {
			cond = LS;

			std::size_t it = condStr.find("<");
			attr = condStr.substr(0, it);
			val = condStr.substr(it+1, condStr.length());

			for(std::size_t i = 0; i < variables.size(); i++) {
				Relation::Attribute var = variables.at(i);
				if(std::get<0>(var).find(attr) != std::string::npos) {
					if(std::get<1>(var) == "int") {
						dataC.type = "int";
						dataC.i = std::stoi(val);
					} else if(std::get<1>(var) == "float") {
						dataC.type = "float";
						dataC.f = std::stof(val);
					}

					break;
				}
			}
		} else if(condStr.find(">=") != std::string::npos) {
			cond = GREQ;

			std::size_t it = condStr.find(">=");
			attr = condStr.substr(0, it);
			val = condStr.substr(it+2, condStr.length());

			for(std::size_t i = 0; i < variables.size(); i++) {
				Relation::Attribute var = variables.at(i);
				if(std::get<0>(var).find(attr) != std::string::npos) {
					if(std::get<1>(var) == "int") {
						dataC.type = "int";
						dataC.i = std::stoi(val);
					} else if(std::get<1>(var) == "float") {
						dataC.type = "float";
						dataC.f = std::stof(val);
					}

					break;
				}
			}
		} else if(condStr.find(">") != std::string::npos) {
			cond = GR;

			std::size_t it = condStr.find(">");
			attr = condStr.substr(0, it);
			val = condStr.substr(it+1, condStr.length());

			for(std::size_t i = 0; i < variables.size(); i++) {
				Relation::Attribute var = variables.at(i);
				if(std::get<0>(var).find(attr) != std::string::npos) {
					if(std::get<1>(var) == "int") {
						dataC.type = "int";
						dataC.i = std::stoi(val);
					} else if(std::get<1>(var) == "float") {
						dataC.type = "float";
						dataC.f = std::stof(val);
					}

					break;
				}
			}
		}

		relations.at(relName)->print(relName, varNames, std::make_tuple(attr, cond, dataC));
	} else {
		relations.at(relName)->print(relName, std::vector<std::string>(), Relation::Condition());
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

void Database::insert(const std::string& relName, const std::string& data) {
	//checks for existance
	if(relations.find(relName) == relations.end()) {
		std::cout << "\tERROR: Relation name [" << relName << "] does not exist!" << std::endl;
		return;
	}

	//remove extraneous chars
	std::string temp = data;
	int idx = 0;
	for(std::size_t i = 0; i < data.length(); i++) {
		if(data[i] == ' ' || data[i] == '\t') {
			temp = temp.substr(0, i-idx) + temp.substr(i-idx+1, temp.length());
			idx++;
		}
	}

	std::istringstream ss(temp + ",");

	//build tuple based on variables
	Tuple tupleTemp;
	for(auto variable : relations.at(relName)->getAttributes()) {
		//parse data
		std::string val;
		std::getline(ss, val, ',');

		//extract variable
		std::string var, type;
		int size;
		std::tie(var, type, size) = variable;

		//Create 
		Data dataTemp;
		dataTemp.type = type;
		if(type == "bool") {
			if(val == "1" || val == "true") {
				dataTemp.b = true;
			} else {
				dataTemp.b = false;
			}
		} else if(type == "int") {
			dataTemp.i = std::stoi(val);
		} else if(type == "float") {
			dataTemp.f = std::stof(val);
		} else if(type == "char" || type == "varchar") {
			if(val.length() <= size) {
				dataTemp.s = val.substr(1, val.length()-2); //remove ''
			} else {
				dataTemp.s = val.substr(1, size);
			}
		}

		TupleNode::Ptr tempNode = std::make_shared<TupleNode>(TupleNode(dataTemp));
		tupleTemp.insertNode(tempNode);
	}

	//insert and print tuple
	relations.at(relName)->insertTuple(tupleTemp);

	std::cout << "\tTuple";
	tupleTemp.print(std::vector<int>());
	std::cout << " inserted into [" << relName << "]." << std::endl;
}

void Database::update(const std::string& relName, const std::string& data) {
	//checks for existance
	if(relations.find(relName) == relations.end()) {
		std::cout << "\tERROR: Relation name [" << relName << "] does not exist!" << std::endl;
		return;
	}

	//find extraneous chars
	std::string temp = data;
	int idx = 0;
	for(std::size_t i = 0; i < data.length(); i++) {
		if(data[i] == ' ' || data[i] == '\t') {
			temp = temp.substr(0, i-idx) + temp.substr(i-idx+1, temp.length());
			idx++;
		}
	}

	//parse data
	std::string setStr, condStr;
	std::istringstream ss(temp + ",");
	std::getline(ss, setStr, '|');
	std::getline(ss, condStr, ';');

	//get attributes
	Relation::Attributes variables = relations.at(relName)->getAttributes();

	//Build set condition
	std::size_t itS = setStr.find("=");
	std::string attrS = setStr.substr(0, itS);
	std::string valS = setStr.substr(itS+1, setStr.length());

	Data dataS;
	for(std::size_t i = 0; i < variables.size(); i++) {
		Relation::Attribute var = variables.at(i);
		if(std::get<0>(var).find(attrS) != std::string::npos) {
			if(std::get<1>(var).find("bool") != std::string::npos) {
				dataS.type = "bool";
				if(valS == "1" || valS == "true") {
					dataS.b = true;
				} else {
					dataS.b = false;
				}
			} else if(std::get<1>(var).find("int") != std::string::npos) {
				dataS.type = "int";
				dataS.i = std::stoi(valS);
			} else if(std::get<1>(var).find("float") != std::string::npos) {
				dataS.type = "float";
				dataS.f = std::stof(valS);
			} else if(std::get<1>(var).find("char")  != std::string::npos || std::get<1>(var).find("varchar") != std::string::npos) {
				dataS.type = "varchar";
				dataS.s = valS.substr(1, valS.length()-2);
			}

			break;
		}
	}

	//Build where condition
	std::string attr, val;
	CONDITION cond = NAN;
	Data dataC;
	if(condStr.find("!=") != std::string::npos) {
		cond = NEQ;

		std::size_t it = condStr.find("!=");
		attr = condStr.substr(0, it);
		val = condStr.substr(it+2, condStr.length());

		for(std::size_t i = 0; i < variables.size(); i++) {
			Relation::Attribute var = variables.at(i);
			if(std::get<0>(var).find(attr) != std::string::npos) {
				if(std::get<1>(var) == "bool") {
					dataC.type = "bool";
					if(val == "1" || val == "true") {
						dataC.b = true;
					} else {
						dataC.b = false;
					}
				} else if(std::get<1>(var) == "int") {
					dataC.type = "int";
					dataC.i = std::stoi(val);
				} else if(std::get<1>(var) == "float") {
					dataC.type = "float";
					dataC.f = std::stof(val);
				} else if(std::get<1>(var) == "char" || std::get<1>(var) == "varchar") {
					dataC.type = "varchar";
					dataC.s = val.substr(1, val.length()-3);
				}

				break;
			}
		}
	} else if(condStr.find("=") != std::string::npos) {
		cond = EQ;

		std::size_t it = condStr.find("=");
		attr = condStr.substr(0, it);
		val = condStr.substr(it+1, condStr.length());

		for(std::size_t i = 0; i < variables.size(); i++) {
			Relation::Attribute var = variables.at(i);
			if(std::get<0>(var).find(attr) != std::string::npos) {
				if(std::get<1>(var).find("bool") != std::string::npos) {
					dataC.type = "bool";
					if(val == "1" || val == "true") {
						dataC.b = true;
					} else {
						dataC.b = false;
					}
				} else if(std::get<1>(var).find("int") != std::string::npos) {
					dataC.type = "int";
					dataC.i = std::stoi(val);
				} else if(std::get<1>(var).find("float") != std::string::npos) {
					dataC.type = "float";
					dataC.f = std::stof(val);
				} else if(std::get<1>(var).find("char") != std::string::npos || std::get<1>(var).find("varchar") != std::string::npos) {
					dataC.type = "varchar";
					dataC.s = val.substr(1, val.length()-3);
				}

				break;
			}
		}
	} else if(condStr.find("<=") != std::string::npos) {
		cond = LSEQ;

		std::size_t it = condStr.find("<=");
		attr = condStr.substr(0, it);
		val = condStr.substr(it+2, condStr.length());

		for(int i = 0; i < variables.size(); i++) {
			Relation::Attribute var = variables.at(i);
			if(std::get<0>(var).find(attr) != std::string::npos) {
				if(std::get<1>(var) == "int") {
					dataC.type = "int";
					dataC.i = std::stoi(val);
				} else if(std::get<1>(var) == "float") {
					dataC.type = "float";
					dataC.f = std::stof(val);
				}

				break;
			}
		}
	} else if(condStr.find("<") != std::string::npos) {
		cond = LS;

		std::size_t it = condStr.find("<");
		attr = condStr.substr(0, it);
		val = condStr.substr(it+1, condStr.length());

		for(std::size_t i = 0; i < variables.size(); i++) {
			Relation::Attribute var = variables.at(i);
			if(std::get<0>(var).find(attr) != std::string::npos) {
				if(std::get<1>(var) == "int") {
					dataC.type = "int";
					dataC.i = std::stoi(val);
				} else if(std::get<1>(var) == "float") {
					dataC.type = "float";
					dataC.f = std::stof(val);
				}

				break;
			}
		}
	} else if(condStr.find(">=") != std::string::npos) {
		cond = GREQ;

		std::size_t it = condStr.find(">=");
		attr = condStr.substr(0, it);
		val = condStr.substr(it+2, condStr.length());

		for(std::size_t i = 0; i < variables.size(); i++) {
			Relation::Attribute var = variables.at(i);
			if(std::get<0>(var).find(attr) != std::string::npos) {
				if(std::get<1>(var) == "int") {
					dataC.type = "int";
					dataC.i = std::stoi(val);
				} else if(std::get<1>(var) == "float") {
					dataC.type = "float";
					dataC.f = std::stof(val);
				}

				break;
			}
		}
	} else if(condStr.find(">") != std::string::npos) {
		cond = GR;

		std::size_t it = condStr.find(">");
		attr = condStr.substr(0, it);
		val = condStr.substr(it+1, condStr.length());

		for(std::size_t i = 0; i < variables.size(); i++) {
			Relation::Attribute var = variables.at(i);
			if(std::get<0>(var).find(attr) != std::string::npos) {
				if(std::get<1>(var) == "int") {
					dataC.type = "int";
					dataC.i = std::stoi(val);
				} else if(std::get<1>(var) == "float") {
					dataC.type = "float";
					dataC.f = std::stof(val);
				}

				break;
			}
		}
	}

	//update tuple by setting [set] when [condition]
	relations.at(relName)->update(relName, std::make_tuple(attrS, EQ, dataS), std::make_tuple(attr, cond, dataC));
}

void Database::remove(const std::string& relName, const std::string& data) {
	//checks for existance
	if(relations.find(relName) == relations.end()) {
		std::cout << "\tERROR: Relation name [" << relName << "] does not exist!" << std::endl;
		return;
	}

	//find extraneous chars
	std::string temp = data;
	int idx = 0;
	for(std::size_t i = 0; i < data.length(); i++) {
		if(data[i] == ' ' || data[i] == '\t') {
			temp = temp.substr(0, i-idx) + temp.substr(i-idx+1, temp.length());
			idx++;
		}
	}

	//parse data
	std::string condStr;
	std::istringstream ss(temp + ",");
	std::getline(ss, condStr, ';');

	//get attributes
	Relation::Attributes variables = relations.at(relName)->getAttributes();

	//Build where condition
	std::string attr, val;
	CONDITION cond = NAN;
	Data dataC;
	if(condStr.find("!=") != std::string::npos) {
		cond = NEQ;

		std::size_t it = condStr.find("!=");
		attr = condStr.substr(0, it);
		val = condStr.substr(it+2, condStr.length());

		for(std::size_t i = 0; i < variables.size(); i++) {
			Relation::Attribute var = variables.at(i);
			if(std::get<0>(var).find(attr) != std::string::npos) {
				if(std::get<1>(var) == "bool") {
					dataC.type = "bool";
					if(val == "1" || val == "true") {
						dataC.b = true;
					} else {
						dataC.b = false;
					}
				} else if(std::get<1>(var) == "int") {
					dataC.type = "int";
					dataC.i = std::stoi(val);
				} else if(std::get<1>(var) == "float") {
					dataC.type = "float";
					dataC.f = std::stof(val);
				} else if(std::get<1>(var) == "char" || std::get<1>(var) == "varchar") {
					dataC.type = "varchar";
					dataC.s = val.substr(1, val.length()-3);
				}

				break;
			}
		}
	} else if(condStr.find("=") != std::string::npos) {
		cond = EQ;

		std::size_t it = condStr.find("=");
		attr = condStr.substr(0, it);
		val = condStr.substr(it+1, condStr.length());

		for(std::size_t i = 0; i < variables.size(); i++) {
			Relation::Attribute var = variables.at(i);
			if(std::get<0>(var).find(attr) != std::string::npos) {
				if(std::get<1>(var).find("bool") != std::string::npos) {
					dataC.type = "bool";
					if(val == "1" || val == "true") {
						dataC.b = true;
					} else {
						dataC.b = false;
					}
				} else if(std::get<1>(var).find("int") != std::string::npos) {
					dataC.type = "int";
					dataC.i = std::stoi(val);
				} else if(std::get<1>(var).find("float") != std::string::npos) {
					dataC.type = "float";
					dataC.f = std::stof(val);
				} else if(std::get<1>(var).find("char") != std::string::npos || std::get<1>(var).find("varchar") != std::string::npos) {
					dataC.type = "varchar";
					dataC.s = val.substr(1, val.length()-3);
				}

				break;
			}
		}
	} else if(condStr.find("<=") != std::string::npos) {
		cond = LSEQ;

		std::size_t it = condStr.find("<=");
		attr = condStr.substr(0, it);
		val = condStr.substr(it+2, condStr.length());

		for(int i = 0; i < variables.size(); i++) {
			Relation::Attribute var = variables.at(i);
			if(std::get<0>(var).find(attr) != std::string::npos) {
				if(std::get<1>(var) == "int") {
					dataC.type = "int";
					dataC.i = std::stoi(val);
				} else if(std::get<1>(var) == "float") {
					dataC.type = "float";
					dataC.f = std::stof(val);
				}

				break;
			}
		}
	} else if(condStr.find("<") != std::string::npos) {
		cond = LS;

		std::size_t it = condStr.find("<");
		attr = condStr.substr(0, it);
		val = condStr.substr(it+1, condStr.length());

		for(std::size_t i = 0; i < variables.size(); i++) {
			Relation::Attribute var = variables.at(i);
			if(std::get<0>(var).find(attr) != std::string::npos) {
				if(std::get<1>(var) == "int") {
					dataC.type = "int";
					dataC.i = std::stoi(val);
				} else if(std::get<1>(var) == "float") {
					dataC.type = "float";
					dataC.f = std::stof(val);
				}

				break;
			}
		}
	} else if(condStr.find(">=") != std::string::npos) {
		cond = GREQ;

		std::size_t it = condStr.find(">=");
		attr = condStr.substr(0, it);
		val = condStr.substr(it+2, condStr.length());

		for(std::size_t i = 0; i < variables.size(); i++) {
			Relation::Attribute var = variables.at(i);
			if(std::get<0>(var).find(attr) != std::string::npos) {
				if(std::get<1>(var) == "int") {
					dataC.type = "int";
					dataC.i = std::stoi(val);
				} else if(std::get<1>(var) == "float") {
					dataC.type = "float";
					dataC.f = std::stof(val);
				}

				break;
			}
		}
	} else if(condStr.find(">") != std::string::npos) {
		cond = GR;

		std::size_t it = condStr.find(">");
		attr = condStr.substr(0, it);
		val = condStr.substr(it+1, condStr.length());

		for(std::size_t i = 0; i < variables.size(); i++) {
			Relation::Attribute var = variables.at(i);
			if(std::get<0>(var).find(attr) != std::string::npos) {
				if(std::get<1>(var) == "int") {
					dataC.type = "int";
					dataC.i = std::stoi(val);
				} else if(std::get<1>(var) == "float") {
					dataC.type = "float";
					dataC.f = std::stof(val);
				}

				break;
			}
		}
	}

	//update tuple by setting [set] when [condition]
	relations.at(relName)->remove(relName, std::make_tuple(attr, cond, dataC));
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
