#pragma once

#include <pthread.h>

#include "task/BaseTask.h"
#include "condition/Condition.h"
#include "mutex/Mutex.h"

namespace thr {
	class Thread
	{
	public:
		Thread();
		virtual ~Thread();

		virtual void run() = 0;

		void start();
		void stop();

		void setTask(BaseTask* task);
		BaseTask* getTask();

	protected:
		// 工作函数
		static void* worker(void* ptr);

	protected:
		pthread_t tid;
		BaseTask* task;
		Mutex mutex;
		Condition cond;
	};
}
