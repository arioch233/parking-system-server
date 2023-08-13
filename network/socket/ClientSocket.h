#pragma once
#include "Socket.h"

#include <string>

using std::string;

namespace net {
	class ClientSocket :
		public Socket
	{
	public:
		ClientSocket();
		ClientSocket(const string& ip, int port);
		virtual ~ClientSocket();
	};
}
