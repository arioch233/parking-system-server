#pragma once

/**
 * socket 服务启动类
 */

#include <string>

using std::string;

namespace server {
	class Server
	{
	public:
		Server();
		~Server();

		/**
		 *  设置监听IP与端口
		 */
		void listen(const string& ip, int port);
		/**
		 * 启动服务
		 */
		void start();
		/**
		 * 设置线程池最大数目
		 */
		void setThreads(int threads);
		/**
		 * 设置最大链接数
		 */
		void setConnects(int connects);
		/**
		 * 设置超时等待时间
		 */
		void setWaitTime(int waitTime);
		/**
		 * 设置共享内存key
		 */
		void setKey(int key);

	private:
		// 服务启动成功打印系统信息
		void draw();

	private:
		string ip;
		int port;
		int threads;
		int connects;
		int waitTime;
		int key;
	};
}