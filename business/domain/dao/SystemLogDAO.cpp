#include "SystemLogDAO.h"

#include<iostream>
#include <cstring>
#include <ctime>

#include "../../utility/Singleton.h"
#include "../../utility/db/SQLQueryBuilder.h"

using namespace utility;

SystemLogDAO::SystemLogDAO()
{
	this->db = Singleton<SingleDB>::getInstance();
}

SystemLogDAO::~SystemLogDAO()
{
}

bool SystemLogDAO::addSystemLog(SystemLog log)
{
	// 构建SQL
	SQLQueryBuilder builder;
	std::string sql = builder.insertInto("system_logs", "log_type, log_level, source, message, details, timestamp")
		.values("'" + log.getLogType() + "', '" + log.getLogLevel() + "', '" + log.getSource() + "', '" +
				log.getMessage() + "' , '" + log.getDetails() + "', DATETIME('now')")
		.build();

	if (this->db->doExec(sql) == 0)
	{
		return true;
	}
	return false;
}

std::vector<SystemLog> SystemLogDAO::getAllSystemLogList()
{
	std::vector<SystemLog> records;
	SQLQueryBuilder builder;

	std::string sql = builder.select("*")
		.from("system_logs")
		.where("STRFTIME('%Y-%m-%d', DATETIME('now')) = STRFTIME('%Y-%m-%d', timestamp)")
		.build();

	char** qres;
	int row, col;
	int res = this->db->doSQL(sql.c_str(), qres, row, col);
	std::cout << res << " " << row << std::endl;
	if (res == 0) {
		if (row > 0) {
			for (int i = 1; i <= row; ++i) {
				int index = i * col;
				SystemLog record(
					std::stoi(qres[index]),                    // id
					qres[index + 1],                           // type
					qres[index + 2],                           // level
					qres[index + 3],                           // source
					qres[index + 4],                           // message
					qres[index + 5],                           // details
					qres[index + 6]                            // timestamp
				);
				records.push_back(record);
			}
		}
	}
	return records;
}