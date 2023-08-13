#pragma once
/**
 * socket 封装类
 */

#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

using std::string;

namespace net {
	class Socket
	{
	public:
		Socket();
		Socket(const string& ip, int port);
		virtual ~Socket();

		bool bind(const string& ip, int port);
		bool listen(int backlog);
		bool connect(const string& ip, int port);
		bool close();

		int accept();

		bool setNonBlocking();

		int getSocketFd();
		void setSocketFd(int fd);

	protected:
		string ip;
		int port;
		int sockfd;
	};
}
