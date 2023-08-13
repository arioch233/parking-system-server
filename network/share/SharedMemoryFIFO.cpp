#include "SharedMemoryFIFO.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

#include "../utility/logger/Logger.h"

using namespace shm;
using namespace utility;

SharedMemoryFIFO::SharedMemoryFIFO()
{
	this->headerPtr = nullptr;
	this->payloadPtr = nullptr;
	this->isOpen = false;
}

SharedMemoryFIFO::SharedMemoryFIFO(int key, int blockSize, int blocks)
{
	this->open(key, blockSize, blocks);
}

SharedMemoryFIFO::~SharedMemoryFIFO()
{
	this->msgQueue.deleteQueue();
	this->close();
	this->destroy();
}

bool SharedMemoryFIFO::initialize(int key, int blockSize, int blocks)
{
	int shmid = 0;

	// 1. 查看是否已经存在共享内存，如果有则删除旧的
	shmid = shmget((key_t)key, 0, 0);
	if (shmid != -1) {
		// 删除已经存在的共享内存
		if (shmctl(shmid, IPC_RMID, nullptr) == -1) {
			error("error deleting existing shared memory: %s", strerror(errno));
			//std::cerr << "error deleting existing shared memory: " << strerror(errno) << std::endl;
		}
	}
	// 2. 创建共享内存
	shmid = shmget((key_t)key, sizeof(SharedMemoryHeader) + blockSize * blocks, 0666 | IPC_CREAT | IPC_EXCL);
	if (shmid == -1) {
		error("error creating shared memory: %s", strerror(errno));
		//std::cerr << "error creating shared memory: " << strerror(errno) << std::endl;
		exit(-1);
	}
	//printf("create shmid=%d size=%ld \n", shmid, sizeof(SharedMemoryHeader) + blockSize * blocks);
	info("create shmid=%d size=%ld", shmid, sizeof(SharedMemoryHeader) + blockSize * blocks);
	// 3. 连接共享内存
	this->headerPtr = shmat(shmid, (void*)0, 0);
	if (this->headerPtr == (void*)-1) {
		error("error attaching to shared memory: %s", strerror(errno));
		//std::cerr << "error attaching to shared memory: " << strerror(errno) << std::endl;
		exit(-1);
	}
	memset(this->headerPtr, 0, sizeof(SharedMemoryHeader) + blockSize * blocks);

	// 4. 初始化共享内存信息
	SharedMemoryHeader* pHead = (SharedMemoryHeader*)(this->headerPtr);
	pHead->shmid = shmid;
	pHead->blockSize = blockSize;
	pHead->totalBlocks = blocks;
	pHead->readIndex = 0;
	pHead->writeIndex = 0;
	sem_init(&pHead->mutexSemaphore, 1, 1);
	sem_init(&pHead->emptySemaphore, 1, 0);
	sem_init(&pHead->fullSemaphore, 1, blocks);

	// 5. 填充控制共享内存的信息
	this->payloadPtr = (char*)(pHead + 1);
	this->isOpen = true;

	// 初始化消息队列
	this->msgQueue.initialize(key + 5);

	return true;
}

void SharedMemoryFIFO::destroy()
{
	SharedMemoryHeader* pHead = (SharedMemoryHeader*)this->headerPtr;
	int shmid = pHead->shmid;

	sem_destroy(&pHead->fullSemaphore);
	sem_destroy(&pHead->emptySemaphore);
	sem_destroy(&pHead->mutexSemaphore);
	shmdt(this->headerPtr);

	if (shmctl(shmid, IPC_RMID, 0) == -1) {
		info("delete shmid=%d", shmid);
		//printf("Delete shmid=%d \n", shmid);
		//std::cerr << "error deleting shared memory: " << strerror(errno) << std::endl;
		error("error deleting shared memory: %s", strerror(errno));
		exit(-1);
	}

	this->headerPtr = nullptr;
	this->payloadPtr = nullptr;
	this->isOpen = false;
}

void SharedMemoryFIFO::destroyMemory(int key)
{
	int shmid = 0;

	shmid = shmget((key_t)key, 0, 0);
	if (shmid != -1) {
		info("delete shmid=%d", shmid);
		//printf("delete shmid=%d \n", shmid);
		if (shmctl(shmid, IPC_RMID, NULL) == -1)
		{
			error("error deleting shared memory: %s", strerror(errno));
			//std::cerr << "error deleting shared memory: " << strerror(errno) << std::endl;
		}
	}
}

bool SharedMemoryFIFO::open(int key, int blockSize, int blocks)
{
	int shmid;

	this->close();

	shmid = shmget((key_t)key, 0, 0);
	if (shmid == -1) {
		return this->initialize(key, blockSize, blocks);
	}

	headerPtr = shmat(shmid, (void*)0, 0);
	if (headerPtr == (void*)-1) {
		error("error attaching to shared memory: ", strerror(errno));
		//std::cerr << "error attaching to shared memory: " << strerror(errno) << std::endl;
	}

	this->payloadPtr = (char*)((SharedMemoryHeader*)this->headerPtr + 1);
	this->isOpen = true;

	return true;
}

void SharedMemoryFIFO::close()
{
	if (this->isOpen) {
		shmdt(this->headerPtr);
		this->headerPtr = nullptr;
		this->payloadPtr = nullptr;
		this->isOpen = false;
	}
}

void SharedMemoryFIFO::write(const void* data)
{
	// 初始化消息
	MSGBUFPTR msg = new MSGBUF();
	msg->mtype = 1;
	// 操作共享内存
	SharedMemoryHeader* pHead = (SharedMemoryHeader*)this->headerPtr;

	sem_wait(&pHead->fullSemaphore);
	sem_wait(&pHead->mutexSemaphore);
	msg->readIndex = pHead->writeIndex; // 设置读索引
	memcpy(this->payloadPtr + (pHead->writeIndex) * (pHead->blockSize), data, pHead->blockSize);
	pHead->writeIndex = (pHead->writeIndex + 1) % (pHead->totalBlocks);

	sem_post(&pHead->mutexSemaphore);
	sem_post(&pHead->emptySemaphore);
	// 发送消息
	this->msgQueue.sendMessage(msg, 0);
	delete msg;
}

void SharedMemoryFIFO::read(void* data)
{
	// 初始化消息
	MSGBUFPTR msg = new MSGBUF();
	msg->mtype = 2;
	// 等待消息
	this->msgQueue.receiveMessage(msg, 0);
	// 操作共享内存
	SharedMemoryHeader* pHead = (SharedMemoryHeader*)this->headerPtr;
	//pHead->readIndex = msg->readIndex;

	sem_wait(&pHead->emptySemaphore);
	sem_wait(&pHead->mutexSemaphore);

	memcpy(data, this->payloadPtr + (pHead->readIndex) * (pHead->blockSize), pHead->blockSize);
	pHead->readIndex = (pHead->readIndex + 1) % (pHead->totalBlocks);

	sem_post(&pHead->mutexSemaphore);
	sem_post(&pHead->fullSemaphore);
	delete msg;
}