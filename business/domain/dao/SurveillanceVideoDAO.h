#pragma once
#include <string>
#include <vector>

#include "../entity/SurveillanceVideo.h"
#include "../../utility/db/SingleDB.h"
using namespace db;

class SurveillanceVideoDAO
{
public:
	SurveillanceVideoDAO();
	~SurveillanceVideoDAO();

	// 根据ID获取视频信息
	SurveillanceVideo* getVideoInfoById(int id);

	// 添加视频信息
	int addVideoInfo(SurveillanceVideo video);

private:
	SingleDB* db;
};
