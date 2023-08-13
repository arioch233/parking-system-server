#pragma once

#include <pthread.h>

#include "../mutex/Mutex.h"

namespace thr {
	class Condition
	{
	public:
		Condition();
		~Condition();

		int wait(Mutex* mutex);
		int signal();
		int broadcast();
	private:
		pthread_cond_t cond;
	};
}
