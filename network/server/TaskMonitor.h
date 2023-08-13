#pragma once
/**
 * 共享内存监听类
 */
#include <pthread.h>
#include <unistd.h>

namespace shm {
	class TaskMonitor
	{
	public:
		TaskMonitor();
		~TaskMonitor();

		void start();
		void stop();

	private:
		static void* threadFunction(void* arg);

	private:
		bool isRunning;
		pthread_t thread;
	};
}
