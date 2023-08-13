#include "SocketHandler.h"

#include <cerrno>

#include "ServerSocket.h"
#include "../task/TaskFactory.h"
#include "../thread/mutex/AutoLock.h"
#include "../thread/task/BaseTask.h"
#include "../thread/TaskDispatcher.h"
#include "../utility/logger/Logger.h"
#include "../utility/Singleton.h"

using namespace utility;

using namespace net;
using namespace thr;

SocketHandler::SocketHandler()
{
}

SocketHandler::~SocketHandler() {
	if (epoll != nullptr)
	{
		delete epoll;
		epoll = nullptr;
	}
	if (server != nullptr)
	{
		delete server;
		server = nullptr;
	}
}

void SocketHandler::listen(const string& ip, int port)
{
	server = new ServerSocket(ip, port);
}

void SocketHandler::attach(Socket* socket)
{
	AutoLock lock(&mutex);
	epoll->add(socket->getSocketFd(), (void*)socket, (EPOLLONESHOT | EPOLLIN | EPOLLHUP | EPOLLERR));
}

void SocketHandler::detach(Socket* socket)
{
	AutoLock lock(&mutex);
	epoll->del(socket->getSocketFd(), (void*)socket, (EPOLLONESHOT | EPOLLIN | EPOLLHUP | EPOLLERR));
}

void SocketHandler::remove(Socket* socket)
{
	socket->close();
}

void SocketHandler::handle(int maxConnections, int waitTime)
{
	epoll = new EventPoller(false);
	epoll->create(maxConnections);
	epoll->add(server->getSocketFd(), server, (EPOLLIN | EPOLLHUP | EPOLLERR));
	this->sockpool.init(maxConnections);

	debug("epoll wait time: %dms", waitTime);
	while (true)
	{
		int num = epoll->wait(waitTime);
		if (num == 0)
		{
			// debug("no events");
			continue;
		}
		for (int i = 0; i < num; i++)
		{
			if (this->server == static_cast<Socket*>(this->epoll->getEvent(i).data.ptr))
			{
				debug("socket accept event");
				int soctfd = this->server->accept();
				Socket* socket = this->sockpool.allocate();
				if (socket == nullptr)
				{
					error("socket pool is empty");
					break;
				}
				socket->setSocketFd(soctfd);
				socket->setNonBlocking();
				this->attach(socket);
			}
			else
			{
				Socket* socket = static_cast<Socket*>(this->epoll->getEvent(i).data.ptr);
				if (this->epoll->getEvent(i).events & EPOLLHUP)
				{
					error("socket %d closed by peer.", socket->getSocketFd());
					this->detach(socket);
					this->remove(socket);
				}
				else if (this->epoll->getEvent(i).events & EPOLLERR)
				{
					error("socket %d error.", socket->getSocketFd());
					this->detach(socket);
					this->remove(socket);
				}
				else if (this->epoll->getEvent(i).events & EPOLLIN)
				{
					debug("socket read event");
					this->detach(socket);
					BaseTask* task = TaskFactory::create(socket, TaskEnum::WriteTask);
					Singleton<TaskDispatcher>::getInstance()->assign(task);
				}
			}
		}
	}
}