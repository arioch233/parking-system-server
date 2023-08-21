#pragma once
/************************************************************************/
/* 心跳包监听类                                                         */
/************************************************************************/

#include <iostream>
#include <map>
#include <chrono>
#include "../utility/timer/Timer.h"

using namespace utility;

namespace server {
	class OnlineUser;

	// 监听类
	class HeartbeatMonitor
	{
	public:
		HeartbeatMonitor();
		~HeartbeatMonitor();

		// 启动监听服务
		void startMonitor(double timeInterval, int verifyInterval);

		// 注册心跳包监听
		void addOnlineUser(int userId, int socketfd);

		// 更新心跳状态
		void updateOnlneUserStatus(int fd);

	private:
		// 验证客户端是否存活
		static void verifyUserOnline(double timeInterval);
	private:
		// 在线用户
		static std::map<int, OnlineUser> onlineUsers;
		double timeInterval;
		int verifyInterval;

		Timer verifyTimer;
	};

	// 用户类
	class OnlineUser
	{
	public:
		OnlineUser() :socketfd(0), userId(0) {
		}
		OnlineUser(int fd, int id) : socketfd(fd), userId(id) {
			auto s_timestamp = this->getCurrentTimestamp();
			std::cout << "add user [userId=" << userId << "], current timestamp = " << s_timestamp.time_since_epoch().count() << std::endl;
			this->timestamp = s_timestamp;
		}
		~OnlineUser() {
		}

		// 更新记录时间戳
		void updateTimestamp() {
			auto s_timestamp = this->getCurrentTimestamp();
			std::cout << "update user [userId=" << userId << "], current timestamp = " << s_timestamp.time_since_epoch().count() << std::endl;
			this->timestamp = s_timestamp;
		}

		// 获取秒级的时间戳
		std::chrono::time_point<std::chrono::system_clock> getCurrentTimestamp() {
			return std::chrono::system_clock::now();
		}

		// 判断时间戳是否有效
		bool verifyTimestamp(double timeInterval) {
			// 获取当前时间戳
			auto currentTime = std::chrono::system_clock::now();
			std::chrono::duration<double> currentTimestamp = currentTime.time_since_epoch();
			double currentTimestampInSeconds = currentTimestamp.count();
			// 转换上一次更新的时间戳
			std::chrono::duration<double> lastTimestamp = this->timestamp.time_since_epoch();
			double lastTimestampInSeconds = lastTimestamp.count();
			// 计算时间戳差值
			double timestampDifference = currentTimestampInSeconds - lastTimestampInSeconds;
			// 判断差值是否小于XXX秒
			return timestampDifference < timeInterval;
		}

		void setSokcetfd(int fd) {
			this->socketfd = fd;
		}
		int getSocketfd() {
			return this->socketfd;
		}

		void setUserId(int id) {
			this->userId = id;
		}
		int getUserId() {
			return this->userId;
		}

	private:
		int socketfd;
		int userId;
		std::chrono::time_point<std::chrono::system_clock> timestamp;
	};
}
