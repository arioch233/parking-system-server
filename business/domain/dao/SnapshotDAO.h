#pragma once

#include <string>
#include <vector>

#include "../entity/Snapshot.h"
#include "../../utility/db/SingleDB.h"
using namespace db;

class SnapshotDAO
{
public:
	SnapshotDAO();
	~SnapshotDAO();

	// 添加录制视频记录
	bool addSnapshot(Snapshot snapshot);
	// 获取视频列表
	std::vector<Snapshot> getSnapShotList(int pageNumber, int recordsPerPage, std::string condition, std::string order);
	// 获取视频总数
	int getSnapShotCount(std::string condition);
	// 获取菜单按日
	std::vector<std::string> getMenuByDay();
	// 获取菜单按月
	std::vector<std::string> getMenuByMouth();

private:
	SingleDB* db;
};
