#pragma once
/**
 * 线程锁封装
 * 基于 Linux 的 pthread_mutex [带有错误检查属性的互斥锁的类]
 */

#include <pthread.h>

namespace thr {
	class Mutex
	{
		friend class Condition;

	public:
		Mutex();
		~Mutex();

		/**
		 * 锁定互斥锁
		 * 返回值:
		 *	0 - 成功锁定互斥锁
		 * 	错误代码 - 发生错误（例如死锁或无效的互斥锁）
		 */
		int lock();
		// 尝试上锁
		int tryLock();
		// 解锁
		int unlock();
	private:
		pthread_mutex_t mutex;
	};
};
