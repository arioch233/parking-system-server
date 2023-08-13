#pragma once

#include "../thread/task/BaseTask.h"
#include "../socket/Socket.h"

using namespace thr;
using namespace net;

namespace task {
	class WriteTask :
		public BaseTask
	{
	public:
		WriteTask(Socket* socket);
		~WriteTask();

		void run() override;

		void destroy() override;
	};
}
