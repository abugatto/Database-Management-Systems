/*



*/

#ifndef DB_H
#define DB_H

/*
   Database Class
*/
class Database {
	public:
		Database(const string& dbName);

		create(const string& relName, const string& data);
		drop(const string& relName, const string& data);
		select(const string& relName, const string& data);
		alter(const string& relName, const string& data); //create new table and copy addresses

		void saveDB();
		void saveDBMetrics();

	private:
		uint numTB = 0;
		map<string, shared_ptr<Relation>> relations;
};

#endif DB_H