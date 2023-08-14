#include "SystemLog.h"

SystemLog::SystemLog() : logType(""), logLevel(""), source(""), message(""), details(""), timestamp("") {}

SystemLog::SystemLog(const std::string& logType, const std::string& logLevel, const std::string& source,
					 const std::string& message, const std::string& details, const std::string& timestamp) : logType(
		logType), logLevel(logLevel), source(source), message(message), details(details), timestamp(timestamp) {
}

SystemLog::SystemLog(int id, const std::string& logType, const std::string& logLevel, const std::string& source,
					 const std::string& message, const std::string& details, const std::string& timestamp) : id(id),
	logType(logType),
	logLevel(
			logLevel),
	source(source),
	message(message),
	details(details),
	timestamp(
			timestamp) {}

SystemLog::~SystemLog() {
}

int SystemLog::getId() const {
	return id;
}

void SystemLog::setId(int id) {
	SystemLog::id = id;
}

const std::string& SystemLog::getLogType() const {
	return logType;
}

void SystemLog::setLogType(const std::string& logType) {
	SystemLog::logType = logType;
}

const std::string& SystemLog::getLogLevel() const {
	return logLevel;
}

void SystemLog::setLogLevel(const std::string& logLevel) {
	SystemLog::logLevel = logLevel;
}

const std::string& SystemLog::getSource() const {
	return source;
}

void SystemLog::setSource(const std::string& source) {
	SystemLog::source = source;
}

const std::string& SystemLog::getMessage() const {
	return message;
}

void SystemLog::setMessage(const std::string& message) {
	SystemLog::message = message;
}

const std::string& SystemLog::getDetails() const {
	return details;
}

void SystemLog::setDetails(const std::string& details) {
	SystemLog::details = details;
}

const std::string& SystemLog::getTimestamp() const {
	return timestamp;
}

void SystemLog::setTimestamp(const std::string& timestamp) {
	SystemLog::timestamp = timestamp;
}