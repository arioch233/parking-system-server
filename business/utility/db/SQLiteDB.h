#pragma once

#include <string>
#include <vector>

#include <sqlite3.h>

namespace db {
	class SQLiteDB
	{
	public:
		SQLiteDB();
		SQLiteDB(const std::string& dbPath);
		~SQLiteDB();

		int openDataBase(std::string path);

		bool executeQuery(const std::string& query); // 执行非查询SQL语句
		sqlite3_stmt* prepareQuery(const std::string& query); // 准备查询语句
		void finalizeQuery(sqlite3_stmt* stmt); // 结束查询语句

		bool beginTransaction(); // 开始事务
		bool commitTransaction(); // 提交事务
		bool rollbackTransaction(); // 回滚事务

		int getRowCount(sqlite3_stmt* stmt); // 获取查询结果行数
		bool stepQuery(sqlite3_stmt* stmt); // 执行查询语句的一步

		// 插入语句拼接
		std::string buildInsertQuery(const std::string& table, const std::vector<std::pair<std::string, std::string>>& values);
		// 单表查询拼接
		std::string buildSelectQuery(const std::string& table, const std::vector<std::string>& columns,
									 const std::string& condition = "", const std::string& orderBy = "");
		// 更新语句拼接
		std::string buildUpdateQuery(const std::string& table, const std::vector<std::pair<std::string, std::string>>& values,
									 const std::string& condition);
		// 查询语句拼接
		std::string buildDeleteQuery(const std::string& table, const std::string& condition);

	private:
		sqlite3* db;
		bool inTransaction;
	};
}
