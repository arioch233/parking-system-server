#pragma once

#include <string>

class SystemLog {
public:
	SystemLog();

	SystemLog(const std::string& logType, const std::string& logLevel, const std::string& source,
			  const std::string& message, const std::string& details, const std::string& timestamp);

	SystemLog(int id, const std::string& logType, const std::string& logLevel, const std::string& source,
			  const std::string& message, const std::string& details, const std::string& timestamp);

	virtual ~SystemLog();

	int getId() const;

	void setId(int id);

	const std::string& getLogType() const;

	void setLogType(const std::string& logType);

	const std::string& getLogLevel() const;

	void setLogLevel(const std::string& logLevel);

	const std::string& getSource() const;

	void setSource(const std::string& source);

	const std::string& getMessage() const;

	void setMessage(const std::string& message);

	const std::string& getDetails() const;

	void setDetails(const std::string& details);

	const std::string& getTimestamp() const;

	void setTimestamp(const std::string& timestamp);

private:
	int id;
	std::string logType;
	std::string logLevel;
	std::string source;
	std::string message;
	std::string details;
	std::string timestamp;
};
