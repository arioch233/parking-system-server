#pragma once

#include <pthread.h>
#include <signal.h>
#include <list>

#include "Thread.h"
#include "ThreadPool.h"
#include "task/BaseTask.h"

using std::list;

namespace thr {
	class TaskDispatcher :
		public Thread
	{
	public:
		TaskDispatcher();
		~TaskDispatcher();

		void init(int threads);
		void assign(BaseTask* task);
		void handle(BaseTask* task);
		virtual void run();

	protected:
		list<BaseTask*> tasks;
	};
}
