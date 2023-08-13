#include "Mutex.h"

using namespace thr;

Mutex::Mutex()
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr); // 初始化互斥锁属性对象
	pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);  // 设置互斥锁属性为错误检查
	pthread_mutex_init(&(this->mutex), &attr); // 使用属性初始化互斥锁
	pthread_mutexattr_destroy(&attr); // 销毁互斥锁属性对象，不再需要
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&(this->mutex));
}

int Mutex::lock()
{
	return pthread_mutex_lock(&(this->mutex));
}

int Mutex::tryLock()
{
	return pthread_mutex_trylock(&(this->mutex));
}

int Mutex::unlock()
{
	return pthread_mutex_unlock(&(this->mutex));
}