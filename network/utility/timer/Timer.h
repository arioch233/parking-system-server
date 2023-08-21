#pragma once

/************************************************************************/
/* 基于线程的定时器                                                     */
/************************************************************************/

#include <thread>
#include <atomic>
#include <functional>
#include <iostream>

namespace utility {
	class Timer {
	public:
		Timer();

		Timer(int repeat);

		virtual ~Timer();

		// 启动定时任务
		template<typename F, typename... Args>
		void start(int milliseconds, F&& func, Args &&... args);

		// 关闭定时任务
		void stop();

	private:
		std::thread timerThread;
		std::atomic<bool> active;
		std::function<void()> func;
		int period; // ms
		int repeat; // 触发次数 -1 无限
	};

	template<typename F, typename... Args>
	void Timer::start(int milliseconds, F&& func, Args &&... args) {
		if (this->active.load()) {
			return;
		}
		this->period = milliseconds;
		this->func = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
		this->active.store(true);
		this->timerThread = std::thread([&]() {
			if (this->repeat < 0) {
				while (this->active.load()) {
					std::this_thread::sleep_for(std::chrono::milliseconds(this->period));
					if (!this->active.load()) {
						return;
					}
					this->func();
				}
			}
			else {
				while (this->repeat > 0) {
					if (!this->active.load()) {
						return;
					}
					std::this_thread::sleep_for(std::chrono::microseconds(this->period));
					if (!this->active.load()) {
						return;
					}
					this->func();
					--(this->repeat);
				}
			}
		});
		this->timerThread.detach();
	}
}