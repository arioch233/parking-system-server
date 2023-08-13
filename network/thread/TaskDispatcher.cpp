#include "TaskDispatcher.h"

#include <cerrno>

#include "ThreadPool.h"
#include "../utility/logger/Logger.h"
#include "../utility/Singleton.h"

using namespace thr;
using namespace utility;

TaskDispatcher::TaskDispatcher()
{
}

TaskDispatcher::~TaskDispatcher()
{
}

void TaskDispatcher::init(int threads)
{
	Singleton<ThreadPool>::getInstance()->create(threads);
	this->start();
}

void TaskDispatcher::assign(BaseTask* task)
{
	debug("task dispatcher assign task");
	this->mutex.lock();
	this->tasks.push_back(task);
	this->mutex.unlock();
	this->cond.signal();
}

void TaskDispatcher::handle(BaseTask* task)
{
	debug("task dispatcher handle task");
	ThreadPool* threadpool = Singleton<ThreadPool>::getInstance();
	if (threadpool->getIdleThreadNumbers() > 0)
	{
		threadpool->assign(task);
	}
	else
	{
		this->mutex.lock();
		this->tasks.push_front(task);
		this->mutex.unlock();
		debug("all threads are busy!");
	}
}

void TaskDispatcher::run()
{
	sigset_t mask;
	if (0 != sigfillset(&mask))
	{
		error("thread manager sigfillset failed!");
		return;
	}
	if (0 != pthread_sigmask(SIG_SETMASK, &mask, NULL))
	{
		error("thread manager pthread_sigmask failed!");
		return;
	}
	while (true)
	{
		this->mutex.lock();
		while (this->tasks.empty()) {
			this->cond.wait(&(this->mutex));
		}
		BaseTask* task = this->tasks.front();
		this->tasks.pop_front();
		this->mutex.unlock();
		handle(task);
	}
}