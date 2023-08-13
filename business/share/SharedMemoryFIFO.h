#pragma once

/**
 * 共享内存封装
 */

#include <semaphore.h>
#include "../protocol/protocol.h"

#include "MessageQueue.h"

namespace shm {
	/**
	 * 共享内存块
	 */
	struct MemoryBlock
	{
		int socketfd;
		char data[2048 - sizeof(socketfd)];

		MemoryBlock() :socketfd(0) {
			memset(data, 0, sizeof(data));
		}

		MemoryBlock(int fd, void* packet, uint32_t size) : socketfd(fd) {
			memset(data, 0, sizeof(data));
			memcpy(data, packet, size);
		}
	};
	/**
	 * 共享内存头
	 */
	typedef struct SharedMemoryHeader {
		int shmid;                  // 共享内存ID

		unsigned int blockSize;     // 块大小
		unsigned int totalBlocks;   // 总块数
		unsigned int readIndex;     // 读索引
		unsigned int writeIndex;    // 写索引

		sem_t mutexSemaphore;       // 用于互斥操作的信号量
		sem_t fullSemaphore;        // 用于表示共享内存已满的信号量
		sem_t emptySemaphore;       // 用于表示共享内存为空的信号量
	} SharedMemoryHeader;

	class SharedMemoryFIFO {
	public:

		/**
		* 默认构造函数，创建一个空的 SharedMemoryFIFO 实例。
		*/
		SharedMemoryFIFO();

		/**
		 * 构造函数，用于创建 SharedMemoryFIFO 实例。
		 *
		 * @param key 共享内存关键字。
		 * @param blockSize 单个块的大小。
		 * @param blocks 总块数。
		 */
		SharedMemoryFIFO(int key, int blockSize, int blocks);

		/**
		 * 析构函数，释放资源。
		 */
		virtual ~SharedMemoryFIFO();

		/**
		 * 初始化共享内存。
		 *
		 * @param key 共享内存关键字。
		 * @param blockSize 单个块的大小。
		 * @param blocks 总块数。
		 * @return 初始化是否成功。
		 */
		bool initialize(int key, int blockSize, int blocks);

		/**
		 * 销毁共享内存及相关资源。
		 */
		void destroy();

		/**
		 * 静态方法：根据关键字销毁共享内存。
		 *
		 * @param key 共享内存关键字。
		 */
		static void destroyMemory(int key);

		/**
		 * 打开已存在的共享内存。
		 *
		 * @param key 共享内存关键字。
		 * @param blockSize 单个块的大小。
		 * @param blocks 总块数。
		 * @return 是否成功打开。
		 */
		bool open(int key, int blockSize, int blocks);

		/**
		 * 关闭已打开的共享内存。
		 */
		void close();

		/**
		 * 将数据写入共享内存。
		 *
		 * @param data 要写入的数据。
		 */
		void write(const void* data);

		/**
		 * 从共享内存读取数据。
		 *
		 * @param data 存储读取数据的缓冲区。
		 */
		void read(void* data);

	protected:
		bool isOpen;         // 标志共享内存是否已打开
		void* headerPtr;     // 共享内存头指针
		char* payloadPtr;    // 负载起始地址

	private:
		MessageQueue msgQueue; // 消息队列
	};
}