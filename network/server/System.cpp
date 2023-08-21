#include "System.h"

#include <iostream>
#include <cstring>

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>

#include "../utility/Singleton.h"
#include "../utility/logger/Logger.h"
#include "../utility/ini/IniFile.h"

using namespace utility;
using std::cout;
using std::endl;

System::System() : rootPath("")
{
}

System::~System()
{
}

void System::init()
{
	this->draw();
	cout << "系统初始化..." << endl;
	this->rootPath = this->getRootPath();
	cout << "SYSTEM ROOT PATH: " << this->rootPath << endl;
	// 创建日志存储文件夹
	const string& logDir = this->rootPath + "/log";
	DIR* dir = opendir(logDir.c_str());
	if (dir == nullptr)
	{
		mkdir(logDir.c_str(), 0755);
	}
	else
	{
		closedir(dir);
	}
	// 初始化日志系统
	Logger* log = Logger::instance();
	log->open(logDir + "/log-dev.log");
	info("log module is successfully initialized");

	// 读取配置文件
	IniFile* ini = Singleton<IniFile>::getInstance();
	ini->load(this->getRootPath() + "/config.ini");
}

string System::getRootPath()
{
	if (this->rootPath != "")
	{
		return this->rootPath;
	}
	char path[1024];
	memset(path, 0, 1024);
	ssize_t cnt = readlink("/proc/self/exe", path, 1024);
	if (cnt < 0 || cnt >= 1024)
	{
		return "";
	}
	for (ssize_t i = cnt; i >= 0; --i)
	{
		if (path[i] == '/')
		{
			path[i] = '\0';
			break;
		}
	}
	return string(path);
}

void System::draw()
{
	cout << "////////////////////////////////////////////////////////////////////\n"
		<< "//                          _ooOoo_                               //\n"
		<< "//                         o8888888o                              //\n"
		<< "//                         88\" . \"88                              //\n"
		<< "//                         (| ^_^ |)                              //\n"
		<< "//                         O\\  =  /O                              //\n"
		<< "//                      ____/`---'\\____                           //\n"
		<< "//                    .'  \\\\|     |//  `.                         //\n"
		<< "//                   /  \\\\|||  :  |||//  \\                        //\n"
		<< "//                  /  _||||| -:- |||||-  \\                       //\n"
		<< "//                  |   | \\\\\\  -  /// |   |                       //\n"
		<< "//                  | \\_|  ''\\---/''  |   |                       //\n"
		<< "//                  \\  .-\\__  `-`  ___/-. /                       //\n"
		<< "//                ___`. .'  /--.--\\  `. . ___                     //\n"
		<< "//              .\"\" '<  `.___\\_<|>_/___.'  >'\"\".                  //\n"
		<< "//            | | :  `- \\`.;`\\ _ /`;.`/ - ` : | |                 //\n"
		<< "//            \\  \\ `-.   \\_ __\\ /__ _/   .-` /  /                 //\n"
		<< "//      ========`-.____`-.___\\_____/___.-`____.-'========         //\n"
		<< "//                           `=---='                              //\n"
		<< "//      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^        //\n"
		<< "//             佛祖保佑       永不宕机      永无BUG               //\n"
		<< "////////////////////////////////////////////////////////////////////" << endl;
}