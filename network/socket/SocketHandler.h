#pragma once

#include "Socket.h"
#include "EventPoller.h"
#include "../thread/mutex/Mutex.h"
#include "../utility/ObjectPool.h"

using namespace thr;
using namespace utility;

namespace net {
	class SocketHandler
	{
	public:
		SocketHandler();
		~SocketHandler();

		void listen(const string& ip, int port);
		void attach(Socket* socket);
		void detach(Socket* socket);
		void remove(Socket* socket);
		void handle(int max_connections, int wait_time);
	private:
		ObjectPool<Socket> sockpool;
		EventPoller* epoll;
		Socket* server;
		Mutex mutex;
	};
}
