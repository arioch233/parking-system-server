#include "server/System.h"
#include "server/Server.h"
#include "utility/Singleton.h"
#include "utility/ini/IniFile.h"

using namespace utility;
using namespace server;

int main(int argc, char** argv)
{
	// 系统初始化
	System* sys = Singleton<System>::getInstance();
	sys->init();

	// 获取配置信息
	IniFile* ini = Singleton<IniFile>::getInstance();
	const string& ip = (*ini)["server"]["ip"];
	int port = (*ini)["server"]["port"];
	int threads = (*ini)["server"]["threads"];
	int max_conn = (*ini)["server"]["max_conn"];
	int wait_time = (*ini)["server"]["wait_time"];
	int shm_key = (*ini)["server"]["shm_key"];

	// 系统服务初始化
	Server* server = Singleton<Server>::getInstance();
	server->setThreads(threads);
	server->setConnects(max_conn);
	server->setWaitTime(wait_time);
	server->setKey(shm_key);
	server->listen(ip, port);

	server->start();
	return 0;
}