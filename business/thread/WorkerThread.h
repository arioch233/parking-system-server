#pragma once
#include "Thread.h"

namespace thr {
	class WorkerThread :
		public Thread
	{
	public:
		WorkerThread();
		virtual ~WorkerThread();

		virtual void run();

		static void cleanup(void* ptr);
	};
}
