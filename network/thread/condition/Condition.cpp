#include "Condition.h"

using namespace thr;

Condition::Condition()
{
	pthread_cond_init(&(this->cond), NULL);
}

Condition::~Condition()
{
	pthread_cond_destroy(&(this->cond));
}

int Condition::wait(Mutex* _mutex)
{
	return pthread_cond_wait(&(this->cond), &(_mutex->mutex));
}

int Condition::signal()
{
	return pthread_cond_signal(&(this->cond));
}

int Condition::broadcast()
{
	return pthread_cond_broadcast(&(this->cond));
}