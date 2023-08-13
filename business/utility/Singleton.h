#pragma once

/**
 * Singleton 单例模板类
 */

#include <pthread.h>

namespace utility {
	template <typename T>
	class Singleton {
	public:
		static T* getInstance()
		{
			if (instance == nullptr)
			{
				pthread_mutex_lock(&mutex);
				if (instance == nullptr) {
					instance = new T();
				}
				pthread_mutex_unlock(&mutex);
			}
			return instance;
		}

		// 销毁
		void clean() {
			pthread_mutex_lock(&mutex);
			delete instance;
			instance = nullptr;
			pthread_mutex_unlock(&mutex);
		}

	private:
		Singleton() = default;
		~Singleton() = default;

		// 禁用拷贝构造函数
		Singleton(Singleton&) = delete;
		// 禁用赋值运算符
		Singleton& operator=(const Singleton&) = delete;

	private:
		// 静态实例
		static T* instance;
		// 静态互斥锁
		static pthread_mutex_t mutex;
	};
	// 初始化静态实例
	template <typename T>
	T* Singleton<T>::instance = nullptr;
	// 初始化静态互斥锁
	template <typename T>
	pthread_mutex_t Singleton<T>::mutex = PTHREAD_MUTEX_INITIALIZER;
}