#pragma once

#include <vector>
#include <string>

#include "../entity/SystemLog.h"
#include "../../utility/db/SingleDB.h"

using namespace db;
class SystemLogDAO
{
public:
	SystemLogDAO();
	~SystemLogDAO();

	// 添加系统日志
	bool addSystemLog(SystemLog log);
	// 获取所有的系统日志
	std::vector<SystemLog> getAllSystemLogList();

private:
	SingleDB* db;
};
