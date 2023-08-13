#include "Server.h"

#include <cerrno>
#include <iostream>

#include "../server/ServerHandler.h"
#include "../thread/TaskDispatcher.h"
#include "../utility/Singleton.h"
#include "../utility/logger/Logger.h"
#include "../share/SharedMemoryFIFO.h"

using namespace server;
using namespace thr;
using namespace utility;
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
	shmFIFO->open(1234, 2048, this->threads);

	TaskDispatcher* dispatcher = Singleton<TaskDispatcher>::getInstance();
	dispatcher->init(this->threads);
	ServerHandler* handler = Singleton<ServerHandler>::getInstance();
	this->draw();
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