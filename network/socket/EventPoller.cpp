#include "EventPoller.h"

using namespace net;

EventPoller::EventPoller(bool et) : epollFd(0), maxConnections(0), events(NULL), isET(et)
{
}

EventPoller::~EventPoller()
{
	if (epollFd > 0)
	{
		close(epollFd);
		epollFd = 0;
	}

	if (events != NULL)
	{
		delete[] events;
		events = NULL;
	}
}

void EventPoller::ctrl(int fd, void* ptr, __uint32_t events, int op)
{
	struct epoll_event ev;
	ev.data.ptr = ptr;
	if (isET)
	{
		ev.events = events | EPOLLET;
	}
	else
	{
		ev.events = events;
	}
	epoll_ctl(epollFd, op, fd, &ev);
}

void EventPoller::create(int max_connections)
{
	maxConnections = max_connections;
	epollFd = epoll_create(max_connections + 1);
	if (epollFd < 0)
	{
		return;
	}
	if (events != NULL)
	{
		delete[] events;
		events = NULL;
	}
	events = new epoll_event[max_connections + 1];
}

void EventPoller::add(int fd, void* ptr, __uint32_t events)
{
	ctrl(fd, ptr, events, EPOLL_CTL_ADD);
}

void EventPoller::mod(int fd, void* ptr, __uint32_t events)
{
	ctrl(fd, ptr, events, EPOLL_CTL_MOD);
}

void EventPoller::del(int fd, void* ptr, __uint32_t events)
{
	ctrl(fd, ptr, events, EPOLL_CTL_DEL);
}

int EventPoller::wait(int millsecond)
{
	return epoll_wait(epollFd, events, maxConnections + 1, millsecond);
}

epoll_event EventPoller::getEvent(int i)
{
	return this->events[i];
}