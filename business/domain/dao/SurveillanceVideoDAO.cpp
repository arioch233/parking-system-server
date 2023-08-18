#include "SurveillanceVideoDAO.h"

#include "../../utility/Singleton.h"
#include "../../utility/db/SQLQueryBuilder.h"

using namespace utility;

SurveillanceVideoDAO::SurveillanceVideoDAO()
{
	this->db = Singleton<SingleDB>::getInstance();
}

SurveillanceVideoDAO::~SurveillanceVideoDAO()
{
}

SurveillanceVideo* SurveillanceVideoDAO::getVideoInfoById(int id)
{
	SurveillanceVideo* video = nullptr;
	SQLQueryBuilder builder;
	std::string sql = builder.select("*")
		.from("surveillance_videos")
		.where("id = :id")
		.build();
	char** qres;
	int row, col;
	int res = this->db->doSQL(sql.c_str(), qres, row, col);
	if (res == 0) {
		if (row > 0) {
			video = new SurveillanceVideo(atoi(qres[7]), atoi(qres[8]), qres[9], qres[10], qres[11], qres[12], qres[13]);
		}
	}
	return video;
}

int SurveillanceVideoDAO::addVideoInfo(SurveillanceVideo video)
{
	// 构建SQL
	SQLQueryBuilder builder;
	std::string values = std::to_string(video.getUserId()) + ", '" +
		video.getCameraName() + "', '" +
		video.getLocation() + "', '" +
		video.getVideoUrl() + "', '" +
		video.getCoverUrl() + "', " +
		"DATETIME('now')";
	std::string sql = builder.insertInto("surveillance_videos", "user_id, camera_name, location, video_url, cover_url, created_at")
		.values(values)
		.build();
	int id;
	if (this->db->doExec(sql, id) == 0)
	{
		return id;
	}
	return false;
}