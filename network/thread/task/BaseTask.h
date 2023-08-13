#pragma once

#include "../mutex/Mutex.h"

namespace thr {
	class BaseTask
	{
	public:
		BaseTask();
		BaseTask(void* data);
		virtual ~BaseTask();

		void* getData();
		void setData(void* data);

		virtual void run() = 0;
		virtual void destroy() = 0;

	protected:
		void* data;
		Mutex mutex;
	};
}
