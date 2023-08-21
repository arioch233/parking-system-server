#include "System.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>

#include "../utility/logger/Logger.h"
#include "../utility/Singleton.h"
#include "../utility/db/SingleDB.h"

using namespace utility;
using namespace db;
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
	// 创建备份存储文件夹
	const string& backDir = this->rootPath + "/backup";
	dir = opendir(backDir.c_str());
	if (dir == nullptr)
	{
		mkdir(backDir.c_str(), 0755);
	}
	else
	{
		closedir(dir);
	}
	// 创建数据库存储文件夹
	const string& dbDir = this->rootPath + "/db";
	dir = opendir(dbDir.c_str());
	if (dir == nullptr)
	{
		mkdir(dbDir.c_str(), 0755);
	}
	else
	{
		closedir(dir);
	}

	// 初始化日志系统
	Logger* log = Logger::instance();
	log->open(logDir + "/log-dev.log");
	info("log module is successfully initialized");
	// 初始化数据库
	SingleDB* db = Singleton<SingleDB>::getInstance();
	db->openDataBase(this->rootPath + "/db/sys.db");
	bool isDatabaseInitialized = false;
	char** qres;
	int row, col;
	int rc = db->doSQL("SELECT name FROM sqlite_master WHERE type='table'", qres, row, col);
	if (rc != 0)
	{
		error("unable to check if the database is already initialized");
	}
	else {
		if (row != 0)
		{
			isDatabaseInitialized = true;
		}
	}

	if (!isDatabaseInitialized) {
		// 如果数据库未初始化，则从SQL文件初始化数据库
		std::ifstream sqlFile(this->rootPath + "/db/sys.sql");
		std::stringstream buffer;
		buffer << sqlFile.rdbuf();
		std::string sqlStatement = buffer.str();

		rc = db->doExec(sqlStatement);

		if (rc != SQLITE_OK) {
			error("failed to initialize the database");
		}
		else {
			debug("database initialized successfully");
		}
	}
	info("database module is successfully initialized");
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