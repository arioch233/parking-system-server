#include "ThreadPool.h"

#include <cerrno>

#include "WorkerThread.h"
#include "../utility/logger/Logger.h"

using namespace thr;
using namespace utility;

ThreadPool::ThreadPool() :threads(0)
{
}

ThreadPool::~ThreadPool() {
}

void ThreadPool::create(int threads) {
	AutoLock lock(&(this->mutexIdle));
	this->threads = threads;
	for (int i = 0; i < threads; i++)
	{
		Thread* thread = new WorkerThread();
		debug("create thread %x", thread);
		listIdle.insert(thread);
		thread->start();
	}
}

Thread* ThreadPool::getIdleThread() {
	AutoLock lock(&(this->mutexIdle));
	while (this->listIdle.size() == 0) {
		this->condIdle.wait(&(this->mutexIdle));
	}
	return *this->listIdle.begin();
}

void ThreadPool::moveToIdleList(Thread* thread) {
	this->mutexIdle.lock();
	this->listIdle.insert(thread);
	this->condIdle.signal();
	this->mutexIdle.unlock();

	this->mutexBusy.lock();
	set<Thread*>::iterator it = this->listBusy.find(thread);
	if (it != this->listBusy.end()) {
		this->listBusy.erase(it);
	}
	this->condBusy.signal();
	this->mutexBusy.unlock();
}
void ThreadPool::moveToBusyList(Thread* thread) {
	this->mutexBusy.lock();
	while (this->listBusy.size() == (size_t)(this->threads)) {
		this->condBusy.wait(&(this->mutexBusy));
	}
	this->listBusy.insert(thread);
	this->mutexBusy.unlock();

	this->mutexIdle.lock();
	set<Thread*>::iterator it = this->listIdle.find(thread);
	if (it != this->listIdle.end())
		this->listIdle.erase(it);
	this->mutexIdle.unlock();
}

int ThreadPool::getIdleThreadNumbers() {
	AutoLock lock(&(this->mutexIdle));
	return this->listIdle.size();
}
int ThreadPool::getBusyThreadNumbers() {
	AutoLock lock(&(this->mutexBusy));
	return this->listBusy.size();
}

void ThreadPool::assign(BaseTask* task) {
	if (task == nullptr)
	{
		error("assign a null task to thread pool");
		return;
	}
	debug("assign a new task: %x to thread pool", task);

	Thread* thread = this->getIdleThread();
	if (thread != nullptr)
	{
		this->moveToBusyList(thread);
		thread->setTask(task);
	}
	else
	{
		error("thread is null, assign a task failed");
	}
}