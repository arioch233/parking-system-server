#pragma once

#include "../thread/task/BaseTask.h"
#include "../share/SharedMemoryFIFO.h"

using namespace thr;
using namespace shm;

namespace task {
	class WriteTask :
		public BaseTask
	{
	public:
		WriteTask(MemoryBlock* block);
		~WriteTask();

		void run() override;

		void destroy() override;
	};
}
