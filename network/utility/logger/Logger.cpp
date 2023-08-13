#include "Logger.h"

#include <iostream>
#include <ctime>
#include <cstdarg>
#include <cstring>

#include <stdexcept>

using namespace utility;

const char* Logger::s_level[LEVEL_COUNT] =
{
		"DEBUG",
		"INFO",
		"WARN",
		"ERROR",
		"FATAL"
};

Logger* Logger::m_instance = nullptr;
pthread_mutex_t Logger::mutex;

Logger::Logger() : m_max(0), m_len(0), m_level(DEBUG) {
	pthread_mutex_init(&(Logger::mutex), nullptr);
}

Logger::~Logger() {
	close();
}

Logger* Logger::instance() {
	if (m_instance == nullptr)
		m_instance = new Logger();
	return m_instance;
}

void Logger::open(const std::string& filename) {
	this->m_filename = filename;
	this->m_fileout.open(filename.c_str(), std::ios::app);
	if (this->m_fileout.fail()) {
		throw std::logic_error("open log file failed: " + filename);
	}
	this->m_fileout.seekp(0, std::ios::end);
	this->m_len = static_cast<int>(this->m_fileout.tellp());
}

void Logger::close() {
	this->m_fileout.close();
}

void Logger::log(Level level, const char* file, int line, const char* format, ...) {
	if (this->m_level > level) {
		return;
	}

	if (this->m_fileout.fail()) {
		throw std::logic_error("open log file failed: " + m_filename);
	}
	pthread_mutex_lock(&(Logger::mutex));
	time_t ticks = time(nullptr);
	struct tm* ptm = localtime(&ticks);
	char timestamp[32];
	memset(timestamp, 0, sizeof(timestamp));
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptm);

	int len = 0;
	const char* fmt = "%s %s %s:%d ";
	len = snprintf(nullptr, 0, fmt, timestamp, s_level[level], file, line);
	if (len > 0) {
		char* buffer = new char[len + 1];
		snprintf(buffer, len + 1, fmt, timestamp, s_level[level], file, line);
		buffer[len] = 0;

		this->m_fileout << buffer;

		std::cout << buffer;
		delete[] buffer;
		this->m_len += len;
	}
	this->m_fileout << ": ";
	va_list arg_ptr;
	va_start(arg_ptr, format);
	len = vsnprintf(nullptr, 0, format, arg_ptr);
	va_end(arg_ptr);
	if (len > 0) {
		char* content = new char[len + 1];
		va_start(arg_ptr, format);
		vsnprintf(content, len + 1, format, arg_ptr);
		va_end(arg_ptr);
		content[len] = 0;
		this->m_fileout << content;
		std::cout << content << std::endl;
		delete[] content;
		m_len += len;
	}

	this->m_fileout << "\n";
	this->m_fileout.flush();

	if (m_max > 0 && m_len >= m_max) {
		rotate();
	}
	pthread_mutex_unlock(&(Logger::mutex));
}

void Logger::max(int bytes) {
	m_max = bytes;
}

void Logger::level(int level) {
	m_level = level;
}

void Logger::rotate() {
	close();
	time_t ticks = time(nullptr);
	struct tm* ptm = localtime(&ticks);
	char timestamp[32];
	memset(timestamp, 0, sizeof(timestamp));
	strftime(timestamp, sizeof(timestamp), ".%Y-%m-%d_%H-%M-%S", ptm);
	std::string filename = m_filename + timestamp;
	if (rename(m_filename.c_str(), filename.c_str()) != 0) {
		throw std::logic_error("rename log file failed: " + std::string(strerror(errno)));
	}
	open(m_filename);
}