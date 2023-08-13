#pragma once
#include "Socket.h"

#include <string>

using std::string;

namespace net {
	class ServerSocket :
		public Socket
	{
	public:
		ServerSocket();
		ServerSocket(const string& ip, int port);
		virtual ~ServerSocket();
	};
}
