#include "ResponseTask.h"

#include <unistd.h>
#include <cerrno>

#include "../protocol/protocol.h"
#include "../utility/logger/Logger.h"
#include "../utility/Singleton.h"
#include "../utility/json/Parser.h"
#include "../utility/json/Json.h"
#include "../share/SharedMemoryFIFO.h"

using namespace task;
using namespace utility;
using namespace json;
using namespace protocol;
using namespace shm;

ResponseTask::ResponseTask(MemoryBlock* block) : BaseTask(block)
{
}

ResponseTask::~ResponseTask()
{
}

void ResponseTask::run()
{
	debug("response task run");
	// 获取数据
	MemoryBlock* block = static_cast<MemoryBlock*>(this->data);
	// 解析数据
	JsonPacket packet;
	memcpy(&packet, block->data, sizeof(JsonPacket));
	error("json = %s", packet.content.jsonData);
	// 业务处理
	int len = write(block->socketfd, &packet, packet.header.packetLength);
	if (len > 0)
	{
		debug("response success");
	}
}

void ResponseTask::destroy()
{
	debug("response task destory");
	delete this;
}