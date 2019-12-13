/*



*/

#ifndef RELATION_CPP
#define RELATION_CPP

#include "Relation.h"

////////////////////////////////////////////////////////////////
//                      Relation Functions                    //
////////////////////////////////////////////////////////////////

Relation::Relation(const Attributes& vars) {
	variables = vars;
}

int Relation::getCardinality() {
	return relation.size(); //size of vector
}

int Relation::getDegree() {
	return variables.size(); //size of tuple
}

Relation::Attributes Relation::getAttributes() {
	return variables;
}

Relation::Table Relation::getRelation() {
	return relation;
}

void Relation::insertTuple(Tuple& tuple) {
	relation.emplace_back(std::make_shared<Tuple>(tuple));
}

void Relation::alterADD(const Attribute& attribute) { //add attribute & init zeros
	//Insert into variables
	variables.emplace_back(attribute);
}

void Relation::update(const std::string relName, const Relation::Condition& set, const Relation::Condition& condition) {
	//where condition
	std::string var;
	CONDITION cond;
	Data data;
	std::tie(var, cond, data) = condition;

	//Set condition
	std::string varS;
	CONDITION condS;
	Data dataS;
	std::tie(varS, condS, dataS) = set;

	//Update relevant tuples
	for(std::size_t i = 0; i < getCardinality(); i++) {
		//get tuple
		Tuple::Ptr tupl = relation.at(i);

		std::size_t whrIt = 0;
		for(; whrIt < getDegree(); whrIt++) {
			if(std::get<0>(variables.at(whrIt)).find(var) != std::string::npos) {
				break;
			}
		}

		//find set attribute
		std::size_t setIt = 0;
		for(; setIt < getDegree(); setIt++) {
			if(std::get<0>(variables.at(setIt)).find(varS) != std::string::npos) {
				break;
			}
		}

		//modify tuple to set condition if it meerts where condition
		bool updated = false;
		if(data.type.find("bool") != std::string::npos) {
			if(cond == EQ) { // where condition
				if(data.b == tupl->at(whrIt).getData().b) {
					tupl->modifyNode(setIt, TupleNode(dataS));
					updated = true;
				}
			} else if(cond == NEQ) {
				if(data.b != tupl->at(whrIt).getData().b) {
					tupl->modifyNode(setIt, TupleNode(dataS));
					updated = true;
				}
			}
		} else if(data.type.find("int") != std::string::npos) {
			if(cond == EQ) { // where condition
				if(data.i == tupl->at(whrIt).getData().i) {
					tupl->modifyNode(setIt, TupleNode(dataS));
					updated = true;
				}
			} else if(cond == NEQ) {
				if(data.i != tupl->at(whrIt).getData().i) {
					tupl->modifyNode(setIt, TupleNode(dataS));
					updated = true;
				}
			} else if(cond == GR) {
				if(data.i > tupl->at(whrIt).getData().i) {
					tupl->modifyNode(setIt, TupleNode(dataS));
					updated = true;
				}
			} else if(cond == GREQ) {
				if(data.i >= tupl->at(whrIt).getData().i) {
					tupl->modifyNode(setIt, TupleNode(dataS));
					updated = true;
				}
			} else if(cond == LSEQ) {
				if(data.i <= tupl->at(whrIt).getData().i) {
					tupl->modifyNode(setIt, TupleNode(dataS));
					updated = true;
				}
			} else if(cond == LS) {
				if(data.i < tupl->at(whrIt).getData().i) {
					tupl->modifyNode(i, TupleNode(dataS));
					updated = true;
				}
			}
		} else if(data.type.find("float") != std::string::npos) {
			if(cond == EQ) { // where condition
				if(data.f == tupl->at(whrIt).getData().f) {
					tupl->modifyNode(setIt, TupleNode(dataS));
					updated = true;
				}
			} else if(cond == NEQ) {
				if(data.f != tupl->at(whrIt).getData().f) {
					tupl->modifyNode(setIt, TupleNode(dataS));
					updated = true;
				}
			} else if(cond == GR) {
				if(data.f < tupl->at(whrIt).getData().f) {
					tupl->modifyNode(setIt, TupleNode(dataS));
					updated = true;
				}
			} else if(cond == GREQ) {
				if(data.f <= tupl->at(whrIt).getData().f) {
					tupl->modifyNode(setIt, TupleNode(dataS));
					updated = true;
				}
			} else if(cond == LSEQ) {
				if(data.f >= tupl->at(whrIt).getData().f) {
					tupl->modifyNode(setIt, TupleNode(dataS));
					updated = true;
				}
			} else if(cond == LS) {
				if(data.f > tupl->at(whrIt).getData().f) {
					tupl->modifyNode(setIt, TupleNode(dataS));
					updated = true;
				}
			}
		} else if(data.type.find("char") != std::string::npos || data.type.find("varchar") != std::string::npos) {
			if(cond == EQ) { // where condition
				if(data.s == tupl->at(whrIt).getData().s) {
					tupl->modifyNode(setIt, TupleNode(dataS));
					updated = true;
				}
			} else if(cond == NEQ) {
				if(data.s != tupl->at(whrIt).getData().s) {
					tupl->modifyNode(setIt, TupleNode(dataS));
					updated = true;
				}
			} 
		}

		if(updated) {
			//Display updated tuple
			std::cout << "\tTuple ";
			tupl->print(std::vector<int>());
			std::cout << " in [" << relName << "] updated." << std::endl;
		}
	}
}

void Relation::remove(const std::string relName, const Relation::Condition& condition) {
	//where condition
	std::string var;
	CONDITION cond;
	Data data;
	std::tie(var, cond, data) = condition;

	//Update relevant tuples
	for(std::size_t i = 0; i < getCardinality(); i++) {
		//get tuple
		Tuple::Ptr tupl = relation.at(i);

		std::size_t whrIt = 0;
		for(; whrIt < getDegree(); whrIt++) {
			if(std::get<0>(variables.at(whrIt)).find(var) != std::string::npos) {
				break;
			}
		}

		//modify tuple to set condition if it meerts where condition
		bool removed = false;
		if(data.type.find("bool") != std::string::npos) {
			if(cond == EQ) { // where condition
				if(data.b == tupl->at(whrIt).getData().b) {
					relation.erase(relation.begin() + i);
					removed = true;
				}
			} else if(cond == NEQ) {
				if(data.b != tupl->at(whrIt).getData().b) {
					relation.erase(relation.begin() + i);
					removed = true;
				}
			}
		} else if(data.type.find("int") != std::string::npos) {
			if(cond == EQ) { // where condition
				if(data.i == tupl->at(whrIt).getData().i) {
					relation.erase(relation.begin() + i);
					removed = true;
				}
			} else if(cond == NEQ) {
				if(data.i != tupl->at(whrIt).getData().i) {
					relation.erase(relation.begin() + i);
					removed = true;
				}
			} else if(cond == GR) {
				if(data.i > tupl->at(whrIt).getData().i) {
					relation.erase(relation.begin() + i);
					removed = true;
				}
			} else if(cond == GREQ) {
				if(data.i >= tupl->at(whrIt).getData().i) {
					relation.erase(relation.begin() + i);
					removed = true;
				}
			} else if(cond == LSEQ) {
				if(data.i <= tupl->at(whrIt).getData().i) {
					relation.erase(relation.begin() + i);
					removed = true;
				}
			} else if(cond == LS) {
				if(data.i < tupl->at(whrIt).getData().i) {
					relation.erase(relation.begin() + i);
					removed = true;
				}
			}
		} else if(data.type.find("float") != std::string::npos) {
			if(cond == EQ) { // where condition
				if(data.f == tupl->at(whrIt).getData().f) {
					relation.erase(relation.begin() + i);
					removed = true;
				}
			} else if(cond == NEQ) {
				if(data.f != tupl->at(whrIt).getData().f) {
					relation.erase(relation.begin() + i);
					removed = true;
				}
			} else if(cond == GR) {
				if(data.f < tupl->at(whrIt).getData().f) {
					relation.erase(relation.begin() + i);
					removed = true;
				}
			} else if(cond == GREQ) {
				if(data.f <= tupl->at(whrIt).getData().f) {
					relation.erase(relation.begin() + i);
					removed = true;
				}
			} else if(cond == LSEQ) {
				if(data.f >= tupl->at(whrIt).getData().f) {
					relation.erase(relation.begin() + i);
					removed = true;
				}
			} else if(cond == LS) {
				if(data.f > tupl->at(whrIt).getData().f) {
					relation.erase(relation.begin() + i);
					removed = true;
				}
			}
		} else if(data.type.find("char") != std::string::npos || data.type.find("varchar") != std::string::npos) {
			if(cond == EQ) { // where condition
				if(data.s == tupl->at(whrIt).getData().s) {
					relation.erase(relation.begin() + i);
					removed = true;
				}
			} else if(cond == NEQ) {
				if(data.s != tupl->at(whrIt).getData().s) {
					relation.erase(relation.begin() + i);
					removed = true;
				}
			} 
		}

		if(removed) {
			//Display updated tuple
			std::cout << "\tTuple in [" << relName << "] removed." << std::endl;
		}
	}
}

Relation::Ptr Relation::join(const JOIN& join, const JoinCondition& condition, const Relation::Ptr& rel) {
	//OUTER NEEDS RECURSION

	//Get indices of self condition attributes
	std::size_t itA = 0;
	for(std::size_t i = 0; i < getDegree(); i++) {
		if(std::get<1>(condition) == std::get<0>(variables.at(i))) {
			itA = i;
			break;
		}
	}

	//get indices of rel condition attributes
	std::size_t itB = 0;
	for(std::size_t i = 0; i < rel->getDegree(); i++) {
		if(std::get<3>(condition) == std::get<0>(rel->getAttributes().at(i))) {
			itB = i;
			break;
		}
	}

	//check if the condition is the same type
	if(std::get<1>(variables.at(itA)) != std::get<1>(rel->getAttributes().at(itB))) {
		throw -5;
	}

	//Build new attribute list
	Attributes newVariables;
	if(join == INNER || join == LEFT_OUTER) {
		//add from self
		for(std::size_t i = 0; i < getDegree(); i++) {
			newVariables.emplace_back(variables.at(i));
		}

		//add from rel
		for(std::size_t i = 0; i < rel->getDegree(); i++) {
			newVariables.emplace_back(rel->getAttributes().at(i));
		}
	} else if(join == RIGHT_OUTER) {
		//add from rel
		for(std::size_t i = 0; i < rel->getDegree(); i++) {
			newVariables.emplace_back(rel->getAttributes().at(i));
		}

		//add from self
		for(std::size_t i = 0; i < getDegree(); i++) {
			newVariables.emplace_back(variables.at(i));
		}
	}

	//build tuple of null attributes for self
	Tuple selfNullTuple;
	for(auto attribute : variables) {
		TupleNode::Ptr nullTupleNode;

		//use string so it brints nothing
		Data data;
		data.type = "string";

		//add node
		nullTupleNode = std::make_shared<TupleNode>(TupleNode(data)); 
		selfNullTuple.insertNode(nullTupleNode);
	}

	//build tuple of null attributes for rel
	Tuple relNullTuple;
	for(auto attribute : rel->getAttributes()) {
		TupleNode::Ptr nullTupleNode;
		
		//use string so it brints nothing
		Data data;
		data.type = "string";

		//add node
		nullTupleNode = std::make_shared<TupleNode>(TupleNode(data)); 
		relNullTuple.insertNode(nullTupleNode);
	}

	//set loops depending on LEFT || RIGHT
	int leftCardinality = 0;
	int rightCardinality = 0;
	if(join == INNER || join == LEFT_OUTER) {
		leftCardinality = getCardinality();
		rightCardinality = rel->getCardinality();
	} else if(join == RIGHT_OUTER) {
		leftCardinality = rel->getCardinality();
		rightCardinality = getCardinality();
	}

	//join relations
	Relation newRelation(newVariables);
	for(std::size_t i = 0; i < leftCardinality; i++) {
		bool inCondition = false;
		std::vector<int> marked;
		for(std::size_t j = 0; j < rightCardinality; j++) {
			bool jMarked = false;
			for(std::size_t m = 0; m < marked.size(); m++) {
				if(marked.at(m) == j) {
					jMarked = true;
					break;
				}
			}

			if(jMarked) {
				break;
			}

			//set iterators
			int itSelf = (join == INNER || join == LEFT_OUTER) ? i : j;
			int itRel = (join == INNER || join == LEFT_OUTER) ? j : i;

			//get condition
			bool conditionSatisfied = false;
			if(std::get<1>(variables.at(itA)).find("bool") != std::string::npos) {
				if(std::get<2>(condition) == EQ) { // where condition
					if(relation.at(itSelf)->at(itA).getData().b == rel->getRelation().at(itRel)->at(itB).getData().b) {
						conditionSatisfied = true;
					}
				} else if(std::get<2>(condition) == NEQ) {
					if(relation.at(itSelf)->at(itA).getData().b != rel->getRelation().at(itRel)->at(itB).getData().b) {
						conditionSatisfied = true;
					}
				}
			} else if(std::get<1>(variables.at(itA)).find("int") != std::string::npos) {
				if(std::get<2>(condition) == EQ) { // where condition
					if(relation.at(itSelf)->at(itA).getData().i == rel->getRelation().at(itRel)->at(itB).getData().i) {
						conditionSatisfied = true;
					}
				} else if(std::get<2>(condition) == NEQ) {
					if(relation.at(itSelf)->at(itA).getData().i != rel->getRelation().at(itRel)->at(itB).getData().i) {
						conditionSatisfied = true;
					}
				} else if(std::get<2>(condition) == GR) {
					if(relation.at(itSelf)->at(itA).getData().i > rel->getRelation().at(itRel)->at(itB).getData().i) {
						conditionSatisfied = true;
					}
				} else if(std::get<2>(condition) == GREQ) {
					if(relation.at(itSelf)->at(itA).getData().i >= rel->getRelation().at(itRel)->at(itB).getData().i) {
						conditionSatisfied = true;
					}
				} else if(std::get<2>(condition) == LSEQ) {
					if(relation.at(itSelf)->at(itA).getData().i <= rel->getRelation().at(itRel)->at(itB).getData().i) {
						conditionSatisfied = true;
					}
				} else if(std::get<2>(condition) == LS) {
					if(relation.at(itSelf)->at(itA).getData().i < rel->getRelation().at(itRel)->at(itB).getData().i) {
						conditionSatisfied = true;
					}
				}
			} else if(std::get<1>(variables.at(itA)).find("float") != std::string::npos) {
				if(std::get<2>(condition) == EQ) { // where condition
					if(relation.at(itSelf)->at(itA).getData().f == rel->getRelation().at(itRel)->at(itB).getData().f) {
						conditionSatisfied = true;
					}
				} else if(std::get<2>(condition) == NEQ) {
					if(relation.at(itSelf)->at(itA).getData().f != rel->getRelation().at(itRel)->at(itB).getData().f) {
						conditionSatisfied = true;
					}
				} else if(std::get<2>(condition) == GR) {
					if(relation.at(itSelf)->at(itA).getData().f > rel->getRelation().at(itRel)->at(itB).getData().f) {
						conditionSatisfied = true;
					}
				} else if(std::get<2>(condition) == GREQ) {
					if(relation.at(itSelf)->at(itA).getData().f >= rel->getRelation().at(itRel)->at(itB).getData().f) {
						conditionSatisfied = true;
					}
				} else if(std::get<2>(condition) == LSEQ) {
					if(relation.at(itSelf)->at(itA).getData().f <= rel->getRelation().at(itRel)->at(itB).getData().f) {
						conditionSatisfied = true;
					}
				} else if(std::get<2>(condition) == LS) {
					if(relation.at(itSelf)->at(itA).getData().f < rel->getRelation().at(itRel)->at(itB).getData().f) {
						conditionSatisfied = true;
					}
				}
			} else if(std::get<1>(variables.at(itA)).find("char") != std::string::npos || std::get<1>(variables.at(itA)).find("varchar") != std::string::npos) {
				if(std::get<2>(condition) == EQ) { // where condition
					if(relation.at(itSelf)->at(itA).getData().s == rel->getRelation().at(itRel)->at(itB).getData().s) {
						conditionSatisfied = true;
					}
				} else if(std::get<2>(condition) == NEQ) {
					if(relation.at(itSelf)->at(itA).getData().s != rel->getRelation().at(itRel)->at(itB).getData().s) {
						conditionSatisfied = true;
					}
				}
			}

			//Make new tuple and insert into relation
			if(join == INNER || join == LEFT_OUTER) { //add if condition
				if(conditionSatisfied) {
					Tuple tupl = *relation.at(itSelf) + *rel->getRelation().at(itRel);

					//insert new tuple into relation
					newRelation.insertTuple(tupl);
					
					inCondition = true;
					marked.emplace_back(j);
				}
			} else if(join == RIGHT_OUTER) { //add if condition and and if not set self's to null
				if(conditionSatisfied) {								
					Tuple tupl = *rel->getRelation().at(itRel) + *relation.at(itSelf);

					//insert new tuple into relation
					newRelation.insertTuple(tupl);
					
					inCondition = true;
					marked.emplace_back(j);
				}
			}

			//If the tuple doesn't satisfy condition but LEFT/RIGHT joined
			if(j == rightCardinality-1 && !inCondition) {
				if(join == LEFT_OUTER) {							
					Tuple tupl = *relation.at(itSelf) + relNullTuple;

					//insert new tuple into relation
					newRelation.insertTuple(tupl);
				} else if(join == RIGHT_OUTER) {
					Tuple tupl = selfNullTuple + *rel->getRelation().at(itRel);

					//insert new tuple into relation
					newRelation.insertTuple(tupl);
				}
			}
		}
	}

	return std::make_shared<Relation>(newRelation);
}

////////////////////////////////////////////////////////////////
//                  Save and Print Functions                  //
//////////////////////////////////////////////////////////////// 

void Relation::print(const std::string& relName, const std::vector<std::string>& varNames, const Relation::Condition& condition) {
	//Print table data
	std::cout << "\t[" << relName << std::endl;

	//check for attributes and find indices
	std::vector<int> indices;
	if(!varNames.empty()) {
		for(std::size_t i = 0; i < getDegree(); i++) {
			for(std::size_t j = 0; j < varNames.size(); j++) {
				if(std::get<0>(variables.at(i)).find(varNames.at(j)) != std::string::npos) {
					indices.emplace_back(i);
					break;
				}
			}
		}
	}
	
	//Print Table Formatting and Attributes
	std::cout << "\t\t[";
	for(std::size_t i = 0; i < variables.size(); i++) {
		bool allowed = false;
		if(!indices.empty()) {
			for(std::size_t j = 0; j < indices.size(); j++) {
				if(indices.at(j) == i) {
					allowed = true;
					break;
				}
			}
		} else {
			allowed = true;
		}

		if(allowed) {
			std::string var, type;
			int size = 0;
			std::tie(var, type, size) = variables.at(i);

			std::cout << var << " " << type;
			if(type == "char" || type == "varchar") {
				std::cout << "(" << std::to_string(size) << ")";
			}
		}

		if(i == getDegree()-1) {
			std::cout << "]" << std::endl; 
		} else if(allowed) {
			std::cout << "|";
		}
	}

	//Print tuples
	if(std::get<1>(condition) == NAN) {
		for(std::size_t i = 0; i < getCardinality(); i++) {
			std::cout << "\t\t";
			relation.at(i)->print(indices);
			std::cout << std::endl;
		}
	} else {
		for(std::size_t i = 0; i < getCardinality(); i++) {
			//get tuple
			Tuple::Ptr tupl = relation.at(i);

			//where condition
			std::string var;
			CONDITION cond;
			Data data;
			std::tie(var, cond, data) = condition;

			//Print relevant tuples
			std::size_t whrIt = 0;
			for(; whrIt < getDegree(); whrIt++) {
				if(std::get<0>(variables.at(whrIt)).find(var) != std::string::npos) {
					break;
				}
			}

			//modify tuple to set condition if it meerts where condition
			bool allowed = false;
			if(data.type.find("bool") != std::string::npos) {
				if(cond == EQ) { // where condition
					if(data.b == tupl->at(whrIt).getData().b) {
						allowed = true;
					}
				} else if(cond == NEQ) {
					if(data.b != tupl->at(whrIt).getData().b) {
						allowed = true;
					}
				}
			} else if(data.type.find("int") != std::string::npos) {
				if(cond == EQ) { // where condition
					if(data.i == tupl->at(whrIt).getData().i) {
						allowed = true;
					}
				} else if(cond == NEQ) {
					if(data.i != tupl->at(whrIt).getData().i) {
						allowed = true;
					}
				} else if(cond == GR) {
					if(data.i > tupl->at(whrIt).getData().i) {
						allowed = true;
					}
				} else if(cond == GREQ) {
					if(data.i >= tupl->at(whrIt).getData().i) {
						allowed = true;
					}
				} else if(cond == LSEQ) {
					if(data.i <= tupl->at(whrIt).getData().i) {
						allowed = true;
					}
				} else if(cond == LS) {
					if(data.i < tupl->at(whrIt).getData().i) {
						allowed = true;
					}
				}
			} else if(data.type.find("float") != std::string::npos) {
				if(cond == EQ) { // where condition
					if(data.f == tupl->at(whrIt).getData().f) {
						allowed = true;
					}
				} else if(cond == NEQ) {
					if(data.f != tupl->at(whrIt).getData().f) {
						allowed = true;
					}
				} else if(cond == GR) {
					if(data.f < tupl->at(whrIt).getData().f) {
						allowed = true;
					}
				} else if(cond == GREQ) {
					if(data.f <= tupl->at(whrIt).getData().f) {
						allowed = true;
					}
				} else if(cond == LSEQ) {
					if(data.f >= tupl->at(whrIt).getData().f) {
						allowed = true;
					}
				} else if(cond == LS) {
					if(data.f > tupl->at(whrIt).getData().f) {
						allowed = true;
					}
				}
			} else if(data.type.find("char") != std::string::npos || data.type.find("varchar") != std::string::npos) {
				if(cond == EQ) { // where condition
					if(data.s == tupl->at(whrIt).getData().s) {
						allowed = true;
					}
				} else if(cond == NEQ) {
					if(data.s != tupl->at(whrIt).getData().s) {
						allowed = true;
					}
				} 
			}

			if(allowed) {
				std::cout << "\t\t";
				tupl->print(indices);
				std::cout << std::endl;
			}
		} 
	}

	std::cout << "\t]" << std::endl;
}

void saveRelation(std::ifstream& fout, const std::string& relName, const std::vector<std::string>& varNames, const Relation::Condition& condition) { //assume that each tuple type is printable with cout
	return;
}

void Relation::saveRelationMetrics() {
	return;
}

#endif