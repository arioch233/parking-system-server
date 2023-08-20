#include "server/System.h"
#include "server/Server.h"
#include "utility/Singleton.h"

using namespace utility;
using namespace server;

int main(int argc, char** argv)
{
	// 系统初始化
	System* sys = Singleton<System>::getInstance();
	sys->init();
	// 系统服务初始化
	Server* server = Singleton<Server>::getInstance();
	server->setThreads(300);
	server->setConnects(10);
	server->setWaitTime(30);
	server->setKey(1234);
	//server->listen(string("156.224.26.110"), 22222);
	server->listen(string("127.0.0.1"), 22222);

	server->start();
	return 0;
}