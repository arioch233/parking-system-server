#pragma once

#include <string>

namespace db {
	class SQLQueryBuilder {
	public:
		SQLQueryBuilder() {
			query = "";
		}

		SQLQueryBuilder& select(const std::string& columns) {
			query += "SELECT " + columns + " ";
			return *this;
		}

		SQLQueryBuilder& from(const std::string& table) {
			query += "FROM " + table + " ";
			return *this;
		}

		SQLQueryBuilder& join(const std::string& table, const std::string& condition) {
			query += "JOIN " + table + " ON " + condition + " ";
			return *this;
		}

		SQLQueryBuilder& where(const std::string& condition) {
			query += "WHERE " + condition + " ";
			return *this;
		}

		SQLQueryBuilder& groupBy(const std::string& columns) {
			query += "GROUP BY " + columns + " ";
			return *this;
		}

		SQLQueryBuilder& orderBy(const std::string& columns) {
			query += "ORDER BY " + columns + " ";
			return *this;
		}

		// SQL 插入
		SQLQueryBuilder& insertInto(const std::string& table, const std::string& columns) {
			query += "INSERT INTO " + table + " (" + columns + ") ";
			return *this;
		}
		SQLQueryBuilder& values(const std::string& values) {
			query += "VALUES (" + values + ") ";
			return *this;
		}

		std::string build() const {
			return query;
		}

	private:
		std::string query;
	};
}