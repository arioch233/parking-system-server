#include "WriteTask.h"

#include <unistd.h>
#include <cerrno>

#include "../protocol/protocol.h"
#include "../utility/logger/Logger.h"
#include "../utility/Singleton.h"
#include "../utility/json/Parser.h"
#include "../utility/json/Json.h"
#include "../share/SharedMemoryFIFO.h"
#include "../domain/controller/UserController.h"

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
	// 解析头数据
	PacketHeader packetHeader;
	memcpy(&packetHeader, block->data, sizeof(PacketHeader));
	// 业务处理
	switch (packetHeader.packetType)
	{
	case BusinessEnum::UploadFeatureImage:
	{
		FileContent fileReq;
		memcpy(&fileReq, block->data + sizeof(PacketHeader), sizeof(JsonContent));
		info("filename = %s, index = %d, total = %d, size = %d", fileReq.filename, fileReq.chunkIndex, fileReq.totalChunks, fileReq.fileSize);
		break;
	}
	case BusinessEnum::Login:
	{
		JsonContent loginReq;
		memcpy(&loginReq, block->data + sizeof(PacketHeader), sizeof(JsonContent));
		info("json data = ", loginReq.jsonData);
		Parser p;
		p.load(loginReq.jsonData);
		Json obj = p.parse();
		UserController userController;
		std::string jsonStr = userController.userLogin(obj["username"], obj["password"]);
		// 写入共享内存
		JsonPacket resultPacket(BusinessEnum::Login, jsonStr);
		MemoryBlock data(block->socketfd, &resultPacket, resultPacket.header.packetLength);
		Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
		break;
	}
	case BusinessEnum::Register:
	{
		JsonContent registerReq;
		memcpy(&registerReq, block->data + sizeof(PacketHeader), sizeof(JsonContent));
		info("json data = ", registerReq.jsonData);
		Parser p;
		p.load(registerReq.jsonData);
		Json obj = p.parse();
		UserController userController;
		std::string jsonStr = userController.userRegister(obj["username"], obj["password"], obj["nickname"]);
		// 写入共享内存
		JsonPacket resultPacket(BusinessEnum::Register, jsonStr);
		MemoryBlock data(block->socketfd, &resultPacket, resultPacket.header.packetLength);
		Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
		break;
	}
	default:
		error("unknown service");
		break;
	}
}

void WriteTask::destroy()
{
	debug("write task destory");
	delete this;
}