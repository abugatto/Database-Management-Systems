/*



*/

#ifndef DB_H
#define DB_H

//
#include 

/*
   Database Class

   Stored in memory as ->[map of key=string and val=[Relation address]]
*/

class Database {
	public:
		Database();

		create(const std::string& relName, const std::string& data);
		drop(const std::string& relName);
		select(const std::string& relName, const std::string& data);
		alter(const std::string& relName, const std::string& data); //create new table and copy addresses

		//Relational Algebra Operations (using relation names)
			//joinInner
			//joinOuter
			//joinTheta(condiitons)

		void saveDB();
		void saveDBMetrics();

		//Shared Pointer
		typedef std::shared_ptr<Database> Ptr;

		//Shared pointer types for the database (attributes could be anything so we need this)
		typedef std::shared_ptr<int> IntPtr;
		typedef std::shared_ptr<float> FloatPtr;
		typedef std::shared_ptr<char> CharPtr;
		typedef std::shared_ptr<std::string> StringPtr;

		//Definition for Variables (name, type)
		typedef std::map<std::string, std::string> Attributes;
		typedef std::shared_ptr<Attributes> AttributesPtr;

	private:
		//Definition for Relation Class (pointer because large database can take up all the memory)
		std::map<std::string, Relation::Ptr> relations; //Pointer for polymorphism of different relations
};

#endif DB_H