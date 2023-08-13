#pragma once

#include <string>

using std::string;

namespace utility {
	class System
	{
	public:
		System();
		~System();

		// 系统初始化
		void init();
		// 获取系统根目录
		string getRootPath();

	private:
		// 打印赛博佛
		void draw();
	private:
		string rootPath;
	};
}
