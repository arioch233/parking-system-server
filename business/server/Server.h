#pragma once

#include <string>

using std::string;

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
		// 服务启动成功打印系统信息
		void draw();

	private:
		int threads;
	};
}
