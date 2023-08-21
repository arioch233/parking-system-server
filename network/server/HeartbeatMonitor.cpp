#include "HeartbeatMonitor.h"

using namespace server;

std::map<int, OnlineUser> HeartbeatMonitor::onlineUsers;

HeartbeatMonitor::HeartbeatMonitor()
{
}

HeartbeatMonitor::~HeartbeatMonitor()
{
}

void HeartbeatMonitor::addOnlineUser(int userId, int socketfd)
{
	HeartbeatMonitor::onlineUsers[socketfd] = OnlineUser(socketfd, userId);
}

void HeartbeatMonitor::updateOnlneUserStatus(int fd)
{
	std::map<int, OnlineUser>::iterator temp = HeartbeatMonitor::onlineUsers.find(fd);
	if (temp != HeartbeatMonitor::onlineUsers.end())
	{
		temp->second.updateTimestamp();
	}
}

void HeartbeatMonitor::startMonitor(double timeInterval, int verifyInterval)
{
	this->timeInterval = timeInterval;
	this->verifyInterval = verifyInterval;

	// 初始化定时器
	this->verifyTimer.start(verifyInterval, HeartbeatMonitor::verifyUserOnline, this->timeInterval);
}

void HeartbeatMonitor::verifyUserOnline(double timeInterval)
{
	std::cout << "verify the validity of the user connection" << std::endl;
	for (auto it = HeartbeatMonitor::onlineUsers.begin(); it != HeartbeatMonitor::onlineUsers.end(); it++)
	{
		if (it->second.verifyTimestamp(timeInterval) == true) {
			std::cout << "userId = " << it->second.getUserId() << " socketId = " << it->second.getSocketfd() << " verify success " << std::endl;
			continue;
		}
		else
		{
			std::cout << "userId = " << it->second.getUserId() << " socketId = " << it->second.getSocketfd() << " verify fail, close connect [remove socketfd] " << std::endl;
			HeartbeatMonitor::onlineUsers.erase(it->second.getSocketfd());
		}
	}
}