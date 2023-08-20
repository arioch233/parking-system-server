#include "SystemLogController.h"

SystemLogController::SystemLogController()
{
}

SystemLogController::~SystemLogController()
{
}

bool SystemLogController::addSystemLogRecord(std::string type, std::string level, std::string src, std::string msg, std::string detail)
{
	SystemLog log;
	log.setLogType(type);
	log.setLogLevel(level);
	log.setSource(src);
	log.setMessage(msg);
	log.setDetails(detail);
	return this->systemLogDAO.addSystemLog(log);
}

std::vector<SystemLog> SystemLogController::getSystemLogRecordList()
{
	return this->systemLogDAO.getAllSystemLogList();
}