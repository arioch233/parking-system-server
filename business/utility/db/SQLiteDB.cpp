#include "SQLiteDB.h"

#include <cerrno>
#include "../logger/Logger.h"

using namespace db;
using namespace utility;

SQLiteDB::SQLiteDB()
{
	debug("sqlite version: %d", sqlite3_libversion_number());
	this->db = nullptr;
}

SQLiteDB::SQLiteDB(const std::string& dbPath)
{
	debug("sqlite version: %d", sqlite3_libversion_number());
	this->openDataBase(dbPath);
}

SQLiteDB::~SQLiteDB()
{
	delete this->db;
}

int SQLiteDB::openDataBase(std::string path)
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

bool SQLiteDB::executeQuery(const std::string& query)
{
	char* errorMessage = nullptr;
	int result = sqlite3_exec(db, query.c_str(), nullptr, nullptr, &errorMessage);

	if (result != SQLITE_OK) {
		error("error executing query: %s", errorMessage);
		sqlite3_free(errorMessage);
		return false;
	}
	return true;
}

sqlite3_stmt* SQLiteDB::prepareQuery(const std::string& query)
{
	sqlite3_stmt* stmt = nullptr;
	int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
	if (rc != SQLITE_OK) {
		// 处理查询准备错误
		error("sqlite3 error, code = %d, msg = %s", sqlite3_errcode(this->db), sqlite3_errmsg(this->db));
	}
	return stmt;
}

void SQLiteDB::finalizeQuery(sqlite3_stmt* stmt)
{
	sqlite3_finalize(stmt);
}

bool SQLiteDB::beginTransaction()
{
	if (!inTransaction) {
		inTransaction = executeQuery("BEGIN");
	}
	return inTransaction;
}

bool SQLiteDB::commitTransaction()
{
	if (inTransaction) {
		inTransaction = !executeQuery("COMMIT");
	}
	return !inTransaction;
}

bool SQLiteDB::rollbackTransaction()
{
	if (inTransaction) {
		inTransaction = !executeQuery("ROLLBACK");
	}
	return !inTransaction;
}

int SQLiteDB::getRowCount(sqlite3_stmt* stmt)
{
	return sqlite3_column_int(stmt, 0);
}

bool SQLiteDB::stepQuery(sqlite3_stmt* stmt)
{
	int rc = sqlite3_step(stmt);
	if (rc == SQLITE_ROW || rc == SQLITE_DONE) {
		return true;
	}
	return false;
}

std::string SQLiteDB::buildInsertQuery(const std::string& table, const std::vector<std::pair<std::string, std::string>>& values)
{
	std::string query = "INSERT INTO " + table + " (";
	//std::string columns;
	//std::string valuePlaceholders;

	//for (const auto& pair : values) {
	//	columns += pair.first + ",";
	//	valuePlaceholders += "?,";
	//}
	//columns.pop_back();
	//valuePlaceholders.pop_back();

	//query += columns + ") VALUES (" + valuePlaceholders + ");";
	return query;
}

std::string SQLiteDB::buildSelectQuery(const std::string& table, const std::vector<std::string>& columns, const std::string& condition /*= ""*/, const std::string& orderBy /*= ""*/)
{
	std::string query = "SELECT ";

	/*if (!columns.empty()) {
		for (const std::string& column : columns) {
			query += column + ",";
		}
		query.pop_back();
	}
	else {
		query += "*";
	}

	query += " FROM " + table;

	if (!condition.empty()) {
		query += " WHERE " + condition;
	}

	if (!orderBy.empty()) {
		query += " ORDER BY " + orderBy;
	}

	query += ";";*/
	return query;
}

std::string SQLiteDB::buildUpdateQuery(const std::string& table, const std::vector<std::pair<std::string, std::string>>& values, const std::string& condition)
{
	std::string query = "UPDATE " + table + " SET ";

	//for (const auto& pair : values) {
	//	query += pair.first + " = ?,";
	//}
	//query.pop_back(); // Remove the trailing comma

	//if (!condition.empty()) {
	//	query += " WHERE " + condition;
	//}

	//query += ";";
	return query;
}

std::string SQLiteDB::buildDeleteQuery(const std::string& table, const std::string& condition)
{
	std::string query = "DELETE FROM " + table;

	if (!condition.empty()) {
		query += " WHERE " + condition;
	}

	query += ";";
	return query;
}