#include "Timer.h"
using namespace utility;

Timer::Timer() : active(false), period(0), repeat(-1) {}

Timer::Timer(int repeat) : active(false), period(0), repeat(repeat) {
}

Timer::~Timer() {
	this->stop();
}

void Timer::stop() {
	this->active.store(false);
}