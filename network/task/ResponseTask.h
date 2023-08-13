#pragma once
/**
 * 响应任务
 */

#include "../thread/task/BaseTask.h"
#include "../share/SharedMemoryFIFO.h"

using namespace thr;
using namespace shm;
namespace task {
	class ResponseTask :
		public BaseTask
	{
	public:
		ResponseTask(MemoryBlock* block);
		~ResponseTask();

		void run() override;

		void destroy() override;
	};
}