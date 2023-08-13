#pragma once

/**
 * 任务工程类
 */

#include "../share/SharedMemoryFIFO.h"
#include "../thread/task/BaseTask.h"
#include "WriteTask.h"

using namespace task;
using namespace shm;

namespace thr {
	class TaskFactory
	{
	public:
		static BaseTask* create(MemoryBlock* block) {
			return new WriteTask(block);
		}
	};
}
