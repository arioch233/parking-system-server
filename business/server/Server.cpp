#include "Server.h"

#include <cerrno>
#include <iostream>
#include <vector>

#include <cstring>
#include <ctime>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>

#include "System.h"
#include "../server/ServerHandler.h"
#include "../thread/TaskDispatcher.h"
#include "../utility/Singleton.h"
#include "../utility/logger/Logger.h"
#include "../utility/csv/CSVTool.h"
#include "../share/SharedMemoryFIFO.h"
#include "../domain/controller/SystemLogController.h"
#include "../domain/entity/SystemLog.h"

using namespace server;
using namespace thr;
using namespace shm;
using std::cout;
using std::endl;

Server::Server() : threads(200) {
}

Server::~Server() {
}

void Server::start() {
	// 初始化共享内存
	SharedMemoryFIFO* shmFIFO = Singleton<SharedMemoryFIFO>::getInstance();
	shmFIFO->open(1234, 1024 * 11, this->threads);
	// 初始化日志分配器
	TaskDispatcher* dispatcher = Singleton<TaskDispatcher>::getInstance();
	dispatcher->init(this->threads);
	// 初始化服务监听
	ServerHandler* handler = Singleton<ServerHandler>::getInstance();
	this->draw();
	// 初始化定时器
	this->backupTimer.start(300 * 1000, Server::backupSystemLog);
	// 启动服务监听
	handler->handle();
}

void Server::setThreads(int threads) {
	this->threads = threads;
}

void Server::draw()
{
	cout << "SYSTEM INFO\n"
		<< "MAX_THREAD_NUM: " << this->threads
		<< endl;
	cout << "(♥◠‿◠)ﾉﾞ  后置服务启动成功   ლ(´ڡ`ლ)ﾞ " << endl;

	const string colors[] = {
		"\033[31m", // 红色
		"\033[33m", // 黄色
		"\033[32m", // 绿色
		"\033[36m", // 青色
		"\033[34m", // 蓝色
		"\033[35m", // 紫色
		"\033[37m"  // 白色
	};

	const string reset = "\033[0m";

	const string asciiArt[] = {
		"     ___      .______       __    ______     ______  __    __  ",
		"    /   \\     |   _  \\     |  |  /  __  \\   /      ||  |  |  | ",
		"   /  ^  \\    |  |_)  |    |  | |  |  |  | |  ,----'|  |__|  | ",
		"  /  /_\\  \\   |      /     |  | |  |  |  | |  |     |   __   | ",
		" /  _____  \\  |  |\\  \\----.|  | |  `--'  | |  `----.|  |  |  | ",
		"/__/     \\__\\ | _| `._____||__|  \\______/   \\______||__|  |__| "
	};

	const int numColors = sizeof(colors) / sizeof(colors[0]);
	const int numLines = sizeof(asciiArt) / sizeof(asciiArt[0]);

	for (int i = 0; i < numLines; ++i) {
		for (size_t j = 0; j < asciiArt[i].length(); ++j) {
			cout << colors[(j + i) % numColors] << asciiArt[i][j];
		}
		cout << reset << endl;
	}
	cout << endl;
}

void Server::backupSystemLog()
{
	SystemLogController systemLogController;
	std::vector<std::string> headers = { "ID", "日志类型", "日志级别", "日志来源", "日志消息内容", "日志详细信息", "日志记录时间" };
	debug("initiate system operation log backup");
	// 获取日志列表信息
	std::vector<SystemLog> records = systemLogController.getSystemLogRecordList();
	// 处理数据
	std::vector<std::vector<std::string>> data;
	for (auto it = records.begin(); it != records.end(); ++it)
	{
		std::vector<std::string> item;
		item.push_back(std::to_string(it->getId()));
		item.push_back(it->getLogType());
		item.push_back(it->getLogLevel());
		item.push_back(it->getSource());
		item.push_back(it->getMessage());
		item.push_back(it->getDetails());
		item.push_back(it->getTimestamp());
		data.push_back(item);
	}
	// 获取时间
	time_t ticks = time(nullptr);
	struct tm* ptm = localtime(&ticks);
	char timestamp[32];
	memset(timestamp, 0, sizeof(timestamp));
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d-%H-%M-%S", ptm);
	// 保存数据
	std::string CSVPath = Singleton<System>::getInstance()->getRootPath();
	CSVPath += "/backup/";
	CSVPath += std::string(timestamp);
	CSVPath += ".csv";
	cout << CSVPath << endl;
	CSVTool csvWriter(CSVPath, headers);
	csvWriter.writeCSV(data);
	debug("system operation log backup completed");
}