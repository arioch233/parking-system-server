#include "Socket.h"

#include <cstring>
#include <cerrno>

#include "../utility/logger/Logger.h"

using namespace net;
using namespace utility;

Socket::Socket() : sockfd(0)
{
}

Socket::Socket(const string& ip, int port) : ip(ip), port(port), sockfd(0)
{
}

Socket::~Socket()
{
	this->close();
}

bool Socket::bind(const string& ip, int port)
{
	struct sockaddr_in sockaddr;
	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	if (ip != "")
	{
		sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
	}
	else
	{
		sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	sockaddr.sin_port = htons(port);

	if (::bind(this->sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
	{
		error("socket bind error: errno=%d errstr=%s", errno, strerror(errno));
		return false;
	}
	return true;
}

bool Socket::listen(int backlog)
{
	if (::listen(this->sockfd, backlog) < 0)
	{
		error("socket listen error: errno=%d errstr=%s", errno, strerror(errno));
		return false;
	}
	return true;
}

bool Socket::connect(const string& ip, int port)
{
	struct sockaddr_in sockaddr;
	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
	sockaddr.sin_port = htons(port);
	if (::connect(this->sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
	{
		error("socket connect error: errno=%d errstr=%s", errno, strerror(errno));
		return false;
	}
	return true;
}

bool Socket::close()
{
	if (this->sockfd > 0)
	{
		::close(this->sockfd);
		this->sockfd = 0;
	}
	return true;
}

int Socket::accept()
{
	int sockfd = ::accept(this->sockfd, NULL, NULL);
	if (sockfd < 0)
	{
		error("accept call error: errno=%d errstr=%s", errno, strerror(errno));
		sockfd = -1;
	}
	return sockfd;
}

int Socket::getSocketFd()
{
	return this->sockfd;
}

void Socket::setSocketFd(int fd)
{
	this->sockfd = fd;
}

bool Socket::setNonBlocking()
{
	int flags = fcntl(this->sockfd, F_GETFL, 0);
	if (flags < 0)
	{
		error("Socket::set_non_blocking(F_GETFL, O_NONBLOCK): errno=%d errstr=%s", errno, strerror(errno));
		return false;
	}
	flags |= O_NONBLOCK;
	if (fcntl(this->sockfd, F_SETFL, flags) < 0)
	{
		error("Socket::set_non_blocking(F_SETFL, O_NONBLOCK): errno=%d errstr=%s", errno, strerror(errno));
		return false;
	}
	return true;
}