#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <vector>

#include "../protocol/protocol.h"
#include "../thread/mutex/Mutex.h"
#include "../thread/mutex/AutoLock.h"

using namespace thr;
using namespace protocol;

namespace task {
	class UploadFileHandler
	{
	public:
		UploadFileHandler();
		~UploadFileHandler();

		// 添加文件块进入 map 容器
		void addFileChunk(int userId, int blockIndex, FileContent& data);
		// 尝试保存文件
		int saveFile(int userId, int total, int size, std::vector<int>& indexList, std::string& path);

		// 容器清空函数
		void clear();

	private:
		// 对所属某个用户的 文件索引key排序 并 检验文件完整性
		std::vector<int> verifyAndSortChunks(int& userId, int& fileSize);
		// 文件合成
		std::string saveChunkToFile(int& userId);
		// 创建文件夹
		void makeDir(std::string path);

	private:
		// 文件数据缓冲区(unordered_map 查询效率高; map 顺序效率高)
		std::unordered_map<int, std::map<int, FileContent>> fileMap;

		// 锁(自动锁)
		Mutex mutex;
	};
}
