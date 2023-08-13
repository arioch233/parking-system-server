#pragma once

#include <list>
#include <set>

#include "Thread.h"
#include "mutex/Mutex.h"
#include "mutex/AutoLock.h"
#include "condition/Condition.h"
#include "task/BaseTask.h"

using std::list;
using std::set;

namespace thr {
	class ThreadPool
	{
	public:
		ThreadPool();
		~ThreadPool();

		void create(int threads);

		Thread* getIdleThread();

		void moveToIdleList(Thread* thread);
		void moveToBusyList(Thread* thread);

		int getIdleThreadNumbers();
		int getBusyThreadNumbers();

		void assign(BaseTask* task);

	private:
		int threads;

		set<Thread*> listIdle;
		set<Thread*> listBusy;

		Mutex mutexIdle;
		Mutex mutexBusy;

		Condition condIdle;
		Condition condBusy;
	};
}
