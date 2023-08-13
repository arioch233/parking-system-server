#include "WorkerThread.h"

#include <cerrno>

#include <pthread.h>
#include <signal.h>

#include "ThreadPool.h"
#include "../utility/logger/Logger.h"
#include "../utility/Singleton.h"

using namespace thr;
using namespace utility;

WorkerThread::WorkerThread() : Thread()
{
}

WorkerThread::~WorkerThread()
{
}

void WorkerThread::run()
{
	sigset_t mask;
	if (0 != sigfillset(&mask))
	{
		error("worker thread sigfillset faile!");
	}
	if (0 != pthread_sigmask(SIG_SETMASK, &mask, NULL))
	{
		error("worker thread pthread_sigmask failed");
	}
	pthread_cleanup_push(cleanup, this);
	while (true)
	{
		// start wait for task
		this->mutex.lock();
		while (this->task == NULL)
			this->cond.wait(&(this->mutex));
		this->mutex.unlock();
		// end wait for task

		int rc = 0;
		int oldState = 0;
		rc = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldState);

		this->task->run();
		this->task->destroy();
		this->task = NULL;

		Singleton<ThreadPool>::getInstance()->moveToIdleList(this);

		rc = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldState);
		pthread_testcancel();
	}
	pthread_cleanup_pop(1);
}

void WorkerThread::cleanup(void* ptr)
{
	info("worker thread cleanup handler: %x", ptr);
}