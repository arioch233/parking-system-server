#include "AutoLock.h"

using namespace thr;

AutoLock::AutoLock(Mutex* mutex)
{
	this->mutex = mutex;
	this->mutex->lock();
}

AutoLock::~AutoLock() {
	this->mutex->unlock();
}