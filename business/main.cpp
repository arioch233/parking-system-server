#include "server/Server.h"
#include "utility/Singleton.h"
#include "server/System.h"
#include "utility/ini/IniFile.h"

using namespace utility;
using namespace server;

int main(int argc, char** argv)
{
	// 系统初始化
	System* system = Singleton<System>::getInstance();
	system->init();

	// 获取配置信息
	IniFile* ini = Singleton<IniFile>::getInstance();
	int threads = (*ini)["server"]["threads"];

	// 系统服务初始化
	Server* server = Singleton<Server>::getInstance();
	server->setThreads(threads);
	server->start();
	return 0;
}