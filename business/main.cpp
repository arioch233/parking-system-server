#include "server/Server.h"
#include "utility/Singleton.h"
#include "server/System.h"

#include <iostream>

using namespace utility;
using namespace server;

int main(int argc, char** argv)
{
	// 系统初始化
	System* system = Singleton<System>::getInstance();
	system->init();

	// 系统服务初始化
	Server* server = Singleton<Server>::getInstance();
	server->setThreads(100);
	server->start();
	return 0;
}