#include "TaskMonitor.h"
#include "../utility/logger/Logger.h"
#include "../share/SharedMemoryFIFO.h"
#include "../thread/TaskDispatcher.h"
#include "../task/TaskFactory.h"
#include "../utility/Singleton.h"

using namespace thr;
using namespace shm;
using namespace utility;

TaskMonitor::TaskMonitor() :isRunning(false)
{
}

TaskMonitor::~TaskMonitor()
{
	this->stop();
}

void TaskMonitor::start()
{
	if (!isRunning) {
		isRunning = true;
		pthread_create(&thread, nullptr, &TaskMonitor::threadFunction, this);
		debug("shared memory listener thread start, thread id = %d", this->thread);
	}
}

void TaskMonitor::stop()
{
	if (this->isRunning) {
		this->isRunning = false;
		pthread_join(this->thread, nullptr);
		debug("shared memory listener thread ends, thread id = %d", this->thread);
		this->thread = 0;
	}
}

void* TaskMonitor::threadFunction(void* arg)
{
	TaskMonitor* manager = static_cast<TaskMonitor*>(arg);
	while (manager->isRunning) {
		// 获取共享内存
		MemoryBlock* data = new MemoryBlock();
		Singleton<SharedMemoryFIFO>::getInstance()->read(data);
		// 派发任务
		BaseTask* task = TaskFactory::create(data, TaskEnum::ResponseTask);
		Singleton<TaskDispatcher>::getInstance()->assign(task);
	}
	return nullptr;
}