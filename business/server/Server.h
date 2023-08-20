#pragma once

#include <string>

#include "../utility/timer/Timer.h"

using std::string;
using namespace utility;

namespace server {
	class Server
	{
	public:
		Server();
		~Server();

		/**
		 * 启动服务
		 */
		void start();
		/**
		 * 设置线程池最大数目
		 */
		void setThreads(int threads);

	private:
		// 日志备份
		static void backupSystemLog();

		// 服务启动成功打印系统信息
		void draw();

	private:
		int threads;
		Timer backupTimer;
	};
}
