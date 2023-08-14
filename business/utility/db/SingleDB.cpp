#include "SingleDB.h"

#include <cerrno>
#include "../logger/Logger.h"

using namespace db;
using namespace utility;

SingleDB::SingleDB()
{
	debug("sqlite version: %d", sqlite3_libversion_number());
	this->db = nullptr;
	this->errorMsg = nullptr;
}

SingleDB::~SingleDB()
{
	delete this->db;
}

int SingleDB::openDataBase(string path)
{
	int res = sqlite3_open(path.c_str(), &(this->db));
	if (res != SQLITE_OK)
	{
		error("sqlite3 error, code = %d, msg = %s", sqlite3_errcode(this->db), sqlite3_errmsg(this->db));
		// 当数据库关闭失败，返回对应的错误码
		return res;
	}
	debug("create database connect session success!");
	// 打开成功返回 0【#define SQLITE_OK 0】
	return SQLITE_OK;
}

int SingleDB::doSQL(string sql, char**& result, int& row, int& col)
{
	debug("do sql: %s", sql.c_str());
	int res = sqlite3_get_table(this->db, sql.c_str(), &result, &row, &col, &(this->errorMsg));
	if (res != SQLITE_OK)
	{
		error("sqlite3 error, code = %d, msg = %s", sqlite3_errcode(this->db), sqlite3_errmsg(this->db));
		return res;
	}
	debug("result: OK, %d piece of data was obtained", row);
	return 0;
}

int SingleDB::doExec(string sql)
{
	debug("do sql: %s", sql.c_str());
	int res = sqlite3_exec(this->db, sql.c_str(), nullptr, nullptr, &(this->errorMsg));
	if (res != SQLITE_OK)
	{
		error("sqlite3 error, code = %d, msg = %s", sqlite3_errcode(this->db), sqlite3_errmsg(this->db));
		return res;
	}
	debug("result: OK, a record has been modified");
	return 0;
}

int SingleDB::closeDataBase()
{
	int res = sqlite3_close(this->db);
	if (res != SQLITE_OK) {
		error("sqlite3 error, code = %d, msg = %s", sqlite3_errcode(this->db), sqlite3_errmsg(this->db));
		// 当数据库关闭失败，返回对应的错误码
		return res;
	}
	// 关闭成功返回 0【#define SQLITE_OK 0】
	return SQLITE_OK;
}