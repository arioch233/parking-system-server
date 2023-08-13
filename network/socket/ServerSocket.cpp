#include "ServerSocket.h"

#include <cerrno>
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>

#include "../utility/logger/Logger.h"

using namespace net;
using namespace utility;

ServerSocket::ServerSocket() : Socket()
{
}

ServerSocket::ServerSocket(const string& ip, int port) : Socket(ip, port)
{
	this->sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->sockfd < 0)
	{
		error("create server socket error: errno=%d errstr=%s", errno, strerror(errno));
		return;
	}
	this->bind(ip, port);
	this->listen(1024);
}

ServerSocket::~ServerSocket()
{
	this->close();
}