#pragma once
/**
 * 对象池类
 */

#include <list>

#include "../thread/mutex/Mutex.h"
#include "../thread/mutex/AutoLock.h"

using std::list;
using namespace thr;

namespace utility {
	template <typename T>
	class ObjectPool
	{
	public:
		ObjectPool();
		~ObjectPool();

		void init(int max);
		T* allocate();
		void release(T* p);

	private:
		list<T*> pool;
		Mutex mutex;
	};

	template <typename T>
	void ObjectPool<T>::release(T* p)
	{
		AutoLock lock(&(this->mutex));
		this->pool.push_back(p);
	}

	template <typename T>
	T* ObjectPool<T>::allocate()
	{
		AutoLock lock(&(this->mutex));
		if (this->pool.size() == 0)
		{
			return nullptr;
		}
		T* p = this->pool.front();
		this->pool.pop_front();
		return p;
	}

	template <typename T>
	void ObjectPool<T>::init(int max)
	{
		AutoLock lock(&(this->mutex));
		for (int i = 0; i < max; ++i)
		{
			T* p = new T();
			this->pool.push_back(p);
		}
	}

	template <typename T>
	ObjectPool<T>::ObjectPool()
	{
	}

	template <typename T>
	ObjectPool<T>::~ObjectPool()
	{
		AutoLock lock(&(this->mutex));
		for (typename list<T*>::iterator it = this->pool.begin(); it != this->pool.end(); it++)
		{
			if ((*it) != nullptr)
			{
				delete (*it);
			}
		}
		this->pool.clear();
	}
}