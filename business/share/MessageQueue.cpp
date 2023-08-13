#include "MessageQueue.h"
#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#include "../utility/logger/Logger.h"

using namespace std;
using namespace utility;

MessageQueue::MessageQueue(int key)
{
	this->initialize(key);
}

MessageQueue::MessageQueue() :msgid(0)
{
}

MessageQueue::~MessageQueue()
{
	this->deleteQueue();
}

bool MessageQueue::initialize(int key)
{
	this->msgid = msgget((key_t)key, IPC_CREAT | 0666);
	if (this->msgid == -1)
	{
		error("create msg error : %s", strerror(errno));
		//perror("msgget error:");
		exit(0);
	}
	info("create message queue success,key = %d; msgid = %d", key, msgid);
	return true;
	//cout << "create message queue success,key = " << key << ";msgid = " << msgid << endl;
}

bool MessageQueue::sendMessage(MSGBUFPTR& buff, int flag)
{
	int res = msgsnd(msgid, (void*)buff, sizeof(buff->readIndex), flag);
	if (res < 0)
	{
		error("msg send error: %s", strerror(errno));
		//perror("msg send error:");
		return false;
	}
	info("send msg success,id = %d", msgid);
	//cout << "send msg success,id = " << msgid << endl;
	return true;
}

bool MessageQueue::receiveMessage(MSGBUFPTR& buff, int flag)
{
	// 接收消息时候先定义好buff的消息数据类型，传参需要直接通过buff指定消息类型
	int res = msgrcv(msgid, (void*)buff, sizeof(buff->readIndex), buff->mtype, flag);
	if (res < 0)
	{
		error("msg rec error: %s", strerror(errno));
		//perror("msg rec error:");
		return false;
	}
	info("rcv msg success,id = %d", msgid);
	//cout << "rcv msg success,id = " << msgid << endl;

	return true;
}

bool MessageQueue::deleteQueue()
{
	int res = msgctl(msgid, IPC_RMID, NULL);
	if (res == -1)
	{
		error("delete message error: %s", strerror(errno));
		//perror("delete message error:");
		return false;
	}
	return true;
}