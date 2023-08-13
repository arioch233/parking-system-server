#pragma once

#include "../thread/task/BaseTask.h"
#include "../socket/Socket.h"

using namespace thr;
using namespace net;

namespace task {
	class RequestTask :
		public BaseTask
	{
	public:
		RequestTask(Socket* socket);
		~RequestTask();

		void run() override;

		void destroy() override;
	};
}
