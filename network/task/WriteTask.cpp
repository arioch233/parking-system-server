#include "WriteTask.h"

#include <unistd.h>
#include <cerrno>

#include "../protocol/protocol.h"
#include "../socket/SocketHandler.h"
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

WriteTask::WriteTask(Socket* socket) :BaseTask(socket)
{
}

WriteTask::~WriteTask() {
}

void WriteTask::run()
{
	debug("write task run");
	SocketHandler* handler = Singleton<SocketHandler>::getInstance();
	Socket* socket = static_cast<Socket*>(this->data);
	JsonPacket packet;

	int len = read(socket->getSocketFd(), &packet.packetLength, sizeof(packet.packetLength));
	if (len == 0)
	{
		error("socket closed by peer");
		handler->remove(socket);
		return;
	}
	if (len == -1 && errno == EAGAIN)
	{
		error("socket recv len: %d, error msg: EAGAIN errno: %d", len, errno);
		handler->attach(socket);
		return;
	}
	if (len == -1 && errno == EWOULDBLOCK)
	{
		error("socket recv len: %d, error msg: EWOULDBLOCK errno: %d", len, errno);
		handler->attach(socket);
		return;
	}
	if (len == -1 && errno == EINTR)
	{
		error("socket recv len: %d, error msg: EINTR errno: %d", len, errno);
		handler->attach(socket);
		return;
	}
	len = read(socket->getSocketFd(), &packet.packetType, sizeof(packet.packetType));
	if (len != sizeof(packet.packetType))
	{
		error("socket recv packetType len: %d, error msg: %d", len, errno);
		handler->attach(socket);
		return;
	}
	len = read(socket->getSocketFd(), &packet.jsonData, packet.packetLength - sizeof(packet.packetType) - sizeof(packet.packetLength));
	if (len != packet.packetLength - sizeof(packet.packetType) - sizeof(packet.packetLength))
	{
		error("socket recv jsonData len: %d, error msg: %d", len, errno);
		handler->attach(socket);
		return;
	}

	// 写入共享内存
	MemoryBlock data(socket->getSocketFd(), &packet, packet.packetLength);
	Singleton<SharedMemoryFIFO>::getInstance()->write(&data);

	handler->attach(socket);
}

void WriteTask::destroy()
{
	debug("write task destory");
	delete this;
}