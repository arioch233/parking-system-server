#include "RequestTask.h"

#include <iostream>
#include <unistd.h>
#include <cerrno>

#include "../protocol/protocol.h"
#include "../socket/SocketHandler.h"
#include "../utility/logger/Logger.h"
#include "../utility/Singleton.h"
#include "../utility/json/Parser.h"
#include "../utility/json/Json.h"
#include "../share/SharedMemoryFIFO.h"
#include "../utility/hash/CRC32.h"

using namespace task;
using namespace utility;
using namespace json;
using namespace protocol;
using namespace shm;

RequestTask::RequestTask(Socket* socket) :BaseTask(socket)
{
}

RequestTask::~RequestTask() {
}

void RequestTask::run()
{
	debug("request task run");
	SocketHandler* handler = Singleton<SocketHandler>::getInstance();
	Socket* socket = static_cast<Socket*>(this->data);

	// 准备协议头
	PacketHeader packetHeader;
	// 读头
	int len = read(socket->getSocketFd(), &packetHeader, sizeof(PacketHeader));

	std::cout << "包体长度: " << packetHeader.packetLength << std::endl;
	std::cout << "数据包类型" << (int)packetHeader.packetType << std::endl;

	if (len == 0)
	{
		error("socket closed by peer");
		handler->remove(socket);
		return;
	}
	if (len == -1 && errno == EAGAIN)
	{
		error("socket recv len: %d, error msg: EAGAIN errno: %d", len, strerror(errno));
		handler->attach(socket);
		return;
	}
	if (len == -1 && errno == EWOULDBLOCK)
	{
		error("socket recv len: %d, error msg: EWOULDBLOCK errno: %d", len, strerror(errno));
		handler->attach(socket);
		return;
	}
	if (len == -1 && errno == EINTR)
	{
		error("socket recv len: %d, error msg: EINTR errno: %d", len, strerror(errno));
		handler->attach(socket);
		return;
	}

	// 分别处理文件包与普通数据包
	switch (packetHeader.packetType) {
		// 与文件相关的数据包处理
	case BusinessEnum::UploadEntryFeatureImage:
	case BusinessEnum::UploadExitFeatureImage:
	case BusinessEnum::UploadFeatureImage:
	{
		std::cout << "文件" << sizeof(FilePacket) << std::endl;
		FileContent fileContent;
		// 读体
		len = read(socket->getSocketFd(), &fileContent, packetHeader.packetLength - sizeof(packetHeader));
		std::cout << "============================== index = " << fileContent.chunkIndex << std::endl;
		info("userId = %d, filename = %s, index = %d, total = %d, size = %d", fileContent.userid, fileContent.filename, fileContent.chunkIndex, fileContent.totalChunks, fileContent.fileSize);
		if (len != packetHeader.packetLength - sizeof(packetHeader))
		{
			error("socket recv data len: %d, error msg: %d", len, strerror(errno));
			handler->attach(socket);
			return;
		}
		// 校验数据完整性
		uint32_t currentCRC = CRC32::calculate(fileContent.chunkData);
		if (currentCRC != fileContent.checksum)
		{
			warn("packet content CRC check failure");
			handler->attach(socket);
			return;
		}
		// 重组JSON数据包
		FilePacket filePacket(packetHeader, fileContent);
		// 写入共享内存
		MemoryBlock data(socket->getSocketFd(), &filePacket, filePacket.header.packetLength);
		Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
		break;
	}
	// 默认即为普通数据包处理
	default:
	{
		std::cout << "JSON" << sizeof(JsonPacket) << std::endl;
		JsonContent jsonContent;
		// 读体
		len = read(socket->getSocketFd(), &jsonContent, packetHeader.packetLength - sizeof(packetHeader));
		if (len != packetHeader.packetLength - sizeof(packetHeader))
		{
			error("socket recv data len: %d, error msg: %d", len, strerror(errno));
			handler->attach(socket);
			return;
		}
		debug("recv json data = %s, CRC32 = %d", jsonContent.jsonData, jsonContent.crc);
		// 校验数据完整性
		uint32_t currentCRC = CRC32::calculate(jsonContent.jsonData);
		if (currentCRC != jsonContent.crc)
		{
			warn("packet content CRC check failure");
			handler->attach(socket);
			return;
		}
		// 重组JSON数据包
		JsonPacket jsonPacket(packetHeader, jsonContent);
		// 写入共享内存
		MemoryBlock data(socket->getSocketFd(), &jsonPacket, jsonPacket.header.packetLength);
		Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
		break;
	}
	}
	handler->attach(socket);
}

void RequestTask::destroy()
{
	debug("request task destory");
	delete this;
}