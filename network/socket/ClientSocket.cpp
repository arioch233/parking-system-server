#include "ClientSocket.h"

#include <cerrno>
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>

#include "../utility/logger/Logger.h"

using namespace net;
using namespace utility;

ClientSocket::ClientSocket() : Socket()
{
}

ClientSocket::ClientSocket(const string& ip, int port) : Socket(ip, port)
{
	this->sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->sockfd < 0)
	{
		error("create client socket error: errno=%d errstr=%s", errno, strerror(errno));
		return;
	}
	this->connect(ip, port);
}

ClientSocket::~ClientSocket()
{
	this->close();
}