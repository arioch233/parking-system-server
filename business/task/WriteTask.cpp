#include "WriteTask.h"

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

WriteTask::WriteTask(MemoryBlock* block) :BaseTask(block)
{
}

WriteTask::~WriteTask() {
}

void WriteTask::run()
{
	debug("write task run");
	// 获取数据
	MemoryBlock* block = static_cast<MemoryBlock*>(this->data);
	// 解析数据
	JsonPacket packet;
	memcpy(&packet, block->data, sizeof(JsonPacket));
	error("json = %s", packet.jsonData);
	// 业务处理
	Parser p;
	p.load(packet.jsonData);
	Json obj = p.parse();
	Json result;
	switch (packet.packetType)
	{
	case BusinessEnum::Login:
	{
		if (obj["account"].asString() == "admin123" && obj["password"].asString() == "admin123")
		{
			result["flag"] = true;
			result["msg"] = "登录成功";
		}
		else
		{
			result["flag"] = false;
			result["msg"] = "账号或密码错误";
		}
		JsonPacket res(BusinessEnum::Login, result.str());

		break;
	}
	case BusinessEnum::Register:
	{
		break;
	}
	default:
		break;
	}

	// 写入共享内存
	JsonPacket resultPacket(BusinessEnum::Login, result.str());
	MemoryBlock data(block->socketfd, &resultPacket, resultPacket.packetLength);
	Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
}

void WriteTask::destroy()
{
	debug("write task destory");
	delete this;
}