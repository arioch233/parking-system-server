#pragma once
/**
 * 自动锁
 * Mutex 进一步封装
 */

#include "Mutex.h"

namespace thr {
	class AutoLock
	{
	public:
		AutoLock(Mutex* mutex);
		~AutoLock();
	private:
		Mutex* mutex;
	};
};
