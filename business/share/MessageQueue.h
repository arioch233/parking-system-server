#pragma once
/**
 * 消息队列封装
 */

typedef struct msg {
	long mtype;      // 消息类型
	int readIndex;   // 读取索引
}MSGBUF, * MSGBUFPTR;

class MessageQueue
{
public:
	MessageQueue();
	MessageQueue(int key);
	~MessageQueue();

	bool initialize(int key);
	bool sendMessage(MSGBUFPTR& buff, int flag);
	bool receiveMessage(MSGBUFPTR& buff, int flag);
	bool deleteQueue();

private:
	int msgid;
};
