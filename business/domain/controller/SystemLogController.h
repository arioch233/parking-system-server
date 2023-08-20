#pragma once

#include "../dao/SystemLogDAO.h"
#include "../entity/SystemLog.h"

#include <string>
#include <vector>

class SystemLogController
{
public:
	SystemLogController();
	~SystemLogController();

	bool addSystemLogRecord(std::string type, std::string level, std::string src, std::string msg, std::string detail);

	std::vector<SystemLog> getSystemLogRecordList();

private:
	SystemLogDAO systemLogDAO;
};
