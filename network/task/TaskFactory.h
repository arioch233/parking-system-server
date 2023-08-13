#pragma once

/**
 * 任务工程类
 */

#include "../socket/Socket.h"
#include "../thread/task/BaseTask.h"
#include "../share/SharedMemoryFIFO.h"
#include "RequestTask.h"
#include "ResponseTask.h"

using namespace task;
using namespace net;
using namespace shm;

namespace thr {
	/**
	 * 任务类型枚举类
	 */
	enum class TaskEnum : uint8_t {
		RequestTask = 0x00,		// 写入共享内存任务
		ResponseTask,			// 响应任务
	};

	class TaskFactory
	{
	public:
		static BaseTask* create(void* data, TaskEnum type) {
			switch (type)
			{
			case thr::TaskEnum::RequestTask:
				return new RequestTask(static_cast<Socket*>(data));
			case thr::TaskEnum::ResponseTask:
				return new ResponseTask(static_cast<MemoryBlock*>(data));
			default:
				return nullptr;
			}
		}
	};
}
