#include "ResponseTask.h"

#include <unistd.h>
#include <cerrno>

#include "../server/HeartbeatMonitor.h"
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
using namespace server;

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
	// 用户登录成功注册处理心跳包
	if (packet.header.packetType == BusinessEnum::Login)
	{
		Parser p;
		p.load(packet.content.jsonData);
		Json obj = p.parse();
		if (obj["flag"].asBool() == true)
		{
			Json user = obj["data"];
			int userId = user["id"];
			// 注册心跳包
			Singleton<HeartbeatMonitor>::getInstance()->addOnlineUser(userId, block->socketfd);
		}
	}
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