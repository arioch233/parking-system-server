#include "Server.h"

#include <iostream>
#include "../socket/SocketHandler.h"
#include "../thread/TaskDispatcher.h"
#include "../utility/Singleton.h"
#include "../utility/logger/Logger.h"
#include "../share/SharedMemoryFIFO.h"
#include "TaskMonitor.h"

using namespace server;
using namespace utility;
using namespace net;
using namespace thr;
using namespace shm;
using std::cout;
using std::endl;

Server::Server() : ip(""), port(0), threads(200), connects(1024), waitTime(10) {
}

Server::~Server() {
}

void Server::listen(const string& ip, int port) {
	this->ip = ip;
	this->port = port;
}

void Server::start() {
	// 初始化共享内存
	SharedMemoryFIFO* shm = Singleton<SharedMemoryFIFO>::getInstance();
	shm->initialize(this->key, 2048, this->threads);

	// 初始化任务分配器
	TaskDispatcher* dispatcher = Singleton<TaskDispatcher>::getInstance();
	dispatcher->init(this->threads);

	// 共享内存监听
	TaskMonitor* monitor = Singleton<TaskMonitor>::getInstance();
	monitor->start();

	// 初始化socket监听
	SocketHandler* handler = Singleton<SocketHandler>::getInstance();
	handler->listen(this->ip, this->port);
	this->draw();
	handler->handle(this->connects, this->waitTime);
}

void Server::setThreads(int threads) {
	this->threads = threads;
}

void Server::setConnects(int connects) {
	this->connects = connects;
}

void Server::setWaitTime(int waitTime) {
	this->waitTime = waitTime;
}

void Server::setKey(int key)
{
	this->key = key;
}

void server::Server::draw()
{
	cout << "SYSTEM INFO\n"
		<< "IP: " << this->ip
		<< "\tPORT: " << this->port
		<< "\tMAX_THREAD_NUM: " << this->threads
		<< "\tMAX_CONNECT_NUM: " << this->connects
		<< "\tWAIT_TIME: " << this->waitTime
		<< endl;
	cout << "(♥◠‿◠)ﾉﾞ  前置服务启动成功   ლ(´ڡ`ლ)ﾞ " << endl;

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