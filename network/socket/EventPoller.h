#pragma once
#include <stddef.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include <string.h>

namespace net {
	class EventPoller
	{
	public:

		EventPoller(bool et = true);
		~EventPoller();

		void create(int max_connections);

		void add(int fd, void* ptr, __uint32_t events);

		void mod(int fd, void* ptr, __uint32_t events);

		void del(int fd, void* ptr, __uint32_t events);

		int wait(int millsecond);

		epoll_event getEvent(int i);

	protected:
		void ctrl(int fd, void* ptr, __uint32_t events, int op);

	protected:

		/**
		 * epoll
		 */
		int epollFd;

		/**
		 * 最大连接数
		 */
		int maxConnections;

		/**
		 * 事件集
		 */
		struct epoll_event* events;

		/**
		 * 是否是ET模式
		 */
		bool isET;
	};
}
