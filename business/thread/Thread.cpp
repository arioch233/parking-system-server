#include "Thread.h"

#include "mutex/AutoLock.h"

using namespace thr;

Thread::Thread() : tid(0), task(NULL)
{
}
Thread::~Thread()
{
}

void Thread::start()
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&this->tid, &attr, worker, (void*)this);
	pthread_attr_destroy(&attr);
}

void Thread::stop()
{
	pthread_exit(PTHREAD_CANCELED);
}

void Thread::setTask(BaseTask* task)
{
	this->mutex.lock();
	this->task = task;
	this->cond.signal();
	this->mutex.unlock();
}

BaseTask* Thread::getTask()
{
	AutoLock lock(&(this->mutex));
	return this->task;
}

void* Thread::worker(void* ptr)
{
	Thread* thread = (Thread*)ptr;
	thread->run();
	return ptr;
}