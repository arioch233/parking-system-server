#pragma once

#include <string>

#include "../dao/SnapshotDAO.h"
#include "../dao/SurveillanceVideoDAO.h"

class SnapshotController
{
public:
	SnapshotController();
	~SnapshotController();

	// 添加录制视频记录
	std::string addSnapshotRecord(int userId, std::string time, std::string desc, std::string fileName, std::string cameraName, std::string location, std::string videoUrl, std::string coverUrl);
	// 获取视频列表
	std::string getSnapShotRecordList(int pageNumber, int recordsPerPage, std::string dateTime);
	// 获取菜单按日
	std::string getMenuOfDay();
	// 获取菜单按月
	std::string getMenuOfMouth();

private:
	SnapshotDAO snapshotDAO;
	SurveillanceVideoDAO surveillanceVideoDAO;
};
