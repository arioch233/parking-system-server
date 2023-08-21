#include "SnapshotController.h"

#include <cerrno>
#include <vector>

#include "../entity/Snapshot.h"
#include "../entity/SurveillanceVideo.h"
#include "../../utility/json/Json.h"
#include "../../utility/logger/Logger.h"

using namespace json;
using namespace utility;

SnapshotController::SnapshotController()
{
}
SnapshotController::~SnapshotController()
{
}

std::string SnapshotController::addSnapshotRecord(int userId, std::string time, std::string desc, std::string fileName, std::string cameraName, std::string location, std::string videoUrl, std::string coverUrl)
{
	std::string jsonStr = "";
	SurveillanceVideo video;
	video.setCameraName(cameraName);
	video.setCoverUrl(coverUrl);
	video.setVideoUrl(videoUrl);
	video.setUserId(userId);
	video.setLocation(location);
	int videoId = this->surveillanceVideoDAO.addVideoInfo(video);
	Snapshot snapshot;
	snapshot.setCaptureTime(time);
	snapshot.setCaptureType(1);
	snapshot.setVideoId(videoId);
	snapshot.setUserId(userId);
	snapshot.setFilePath(fileName);
	snapshot.setDescription(desc);
	Json result;
	if (this->snapshotDAO.addSnapshot(snapshot))
	{
		result["flag"] = true;
		result["msg"] = "添加视频记录成功";
	}
	else
	{
		result["flag"] = false;
		result["msg"] = "添加视频记录失败";
	}
	jsonStr = result.str();
	return jsonStr;
}

std::string SnapshotController::getSnapShotRecordList(int pageNumber, int recordsPerPage, std::string dateTime)
{
	std::string jsonStr = "";
	std::string condition = "";
	// 拼接条件查询
	if (!dateTime.empty())
	{
		condition = "created_at LIKE '%" + dateTime + "%'";
	}
	std::vector<Snapshot> records = this->snapshotDAO.getSnapShotList(pageNumber, recordsPerPage, condition, "created_at DESC");
	int counts = this->snapshotDAO.getSnapShotCount(condition);
	Json result;
	if (records.empty() && records.size() == 0)
	{
		result["flag"] = true;
		result["msg"] = "视频记录为空";
		Json arr(Json::JSON_ARRAY);
		result["data"] = arr;
	}
	else
	{
		result["flag"] = true;
		result["msg"] = "共有 " + std::to_string(records.size()) + "记录";
		result["count"] = counts;
		Json arr(Json::JSON_ARRAY);
		for (int i = 0; i < records.size(); ++i)
		{
			int videoId = records[i].getVideoId();
			SurveillanceVideo* video = this->surveillanceVideoDAO.getVideoInfoById(videoId);
			Json temp;
			temp["videoName"] = records[i].getFilePath();
			temp["videoUrl"] = video->getVideoUrl();
			temp["coverUrl"] = video->getCoverUrl();
			arr.append(temp);
			delete video;
		}
		result["data"] = arr;
	}
	jsonStr = result.str();
	return jsonStr;
}

std::string SnapshotController::getMenuOfDay()
{
	std::string jsonStr = "";
	Json result;
	std::vector<std::string> resultList = this->snapshotDAO.getMenuByDay();
	if (resultList.empty() && resultList.size() == 0)
	{
		result["flag"] = true;
		result["msg"] = "捕获记录为空";
		Json arr(Json::JSON_ARRAY);
		result["data"] = arr;
	}
	else
	{
		result["flag"] = true;
		result["msg"] = "共有 " + std::to_string(resultList.size()) + "记录";
		Json arr(Json::JSON_ARRAY);
		for (int i = 0; i < resultList.size(); ++i)
		{
			Json temp = resultList[i];
			arr.append(temp);
		}
		result["data"] = arr;
	}
	jsonStr = result.str();
	return jsonStr;
}

std::string SnapshotController::getMenuOfMouth()
{
	std::string jsonStr = "";
	Json result;
	std::vector<std::string> resultList = this->snapshotDAO.getMenuByMouth();
	if (resultList.empty() && resultList.size() == 0)
	{
		result["flag"] = true;
		result["msg"] = "捕获记录为空";
		Json arr(Json::JSON_ARRAY);
		result["data"] = arr;
	}
	else
	{
		result["flag"] = true;
		result["msg"] = "共有 " + std::to_string(resultList.size()) + "记录";
		Json arr(Json::JSON_ARRAY);
		for (int i = 0; i < resultList.size(); ++i)
		{
			Json temp = resultList[i];
			arr.append(temp);
		}
		result["data"] = arr;
	}
	jsonStr = result.str();
	return jsonStr;
}