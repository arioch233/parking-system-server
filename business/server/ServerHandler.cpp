#include "ServerHandler.h"

#include <cerrno>
#include <cstring>

#include "../utility/json/Json.h"
#include "../utility/json/Parser.h"
#include "../utility/logger/Logger.h"
#include "../utility/Singleton.h"
#include "../share/SharedMemoryFIFO.h"
#include "../protocol/protocol.h"
#include "../thread/task/BaseTask.h"
#include "../thread/TaskDispatcher.h"
#include "../task/TaskFactory.h";

using namespace server;
using namespace utility;
using namespace shm;
using namespace json;
using namespace protocol;
using namespace thr;

ServerHandler::ServerHandler()
{
}

ServerHandler::~ServerHandler()
{
}

void ServerHandler::handle()
{
	debug("server handler start, start connect shm message queue, wait message");
	while (true)
	{
		MemoryBlock* data = new MemoryBlock();
		SharedMemoryFIFO* shmFIFO = Singleton<SharedMemoryFIFO>::getInstance();
		shmFIFO->read(data);

		// 派发任务
		BaseTask* task = TaskFactory::create(data);
		Singleton<TaskDispatcher>::getInstance()->assign(task);
	}
}