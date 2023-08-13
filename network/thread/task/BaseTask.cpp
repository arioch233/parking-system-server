#include "BaseTask.h"

#include "../mutex/AutoLock.h"

using namespace thr;

BaseTask::BaseTask() : data(NULL)
{
}

BaseTask::BaseTask(void* data) : data(data)
{
}

BaseTask::~BaseTask()
{
}

void* BaseTask::getData()
{
	AutoLock lock(&(this->mutex));
	return this->data;
}

void BaseTask::setData(void* data)
{
	AutoLock lock(&(this->mutex));
	this->data = data;
}