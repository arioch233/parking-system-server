#pragma once

#include <sqlite3.h>

#include <string>

using std::string;

namespace db {
	class SingleDB
	{
	public:
		SingleDB();
		~SingleDB();

		int openDataBase(string path);
		int doSQL(string sql, char**& result, int& row, int& col);
		int doExec(string sql);
		int doExec(string sql, int& id);
		int closeDataBase();

	private:
		sqlite3* db;
		char* errorMsg;
	};
}
