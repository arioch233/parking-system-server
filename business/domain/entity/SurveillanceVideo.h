#pragma once

#include <string>

class SurveillanceVideo {
public:
	SurveillanceVideo();

	SurveillanceVideo(int userId, const std::string& cameraName, const std::string& location,
					  const std::string& videoUrl, const std::string& coverUrl, const std::string& createTime);

	SurveillanceVideo(int id, int userId, const std::string& cameraName, const std::string& location,
					  const std::string& videoUrl, const std::string& coverUrl, const std::string& createTime);

	virtual ~SurveillanceVideo();

	int getId() const;

	void setId(int id);

	int getUserId() const;

	void setUserId(int userId);

	const std::string& getCameraName() const;

	void setCameraName(const std::string& cameraName);

	const std::string& getLocation() const;

	void setLocation(const std::string& location);

	const std::string& getVideoUrl() const;

	void setVideoUrl(const std::string& videoUrl);

	const std::string& getCoverUrl() const;

	void setCoverUrl(const std::string& coverUrl);

	const std::string& getCreateTime() const;

	void setCreateTime(const std::string& createTime);

private:
	int id;
	int userId;
	std::string cameraName;
	std::string location;
	std::string videoUrl;
	std::string coverUrl;
	std::string createTime;
};
