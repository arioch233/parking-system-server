#include "SurveillanceVideo.h"

SurveillanceVideo::SurveillanceVideo() : id(0), userId(0), cameraName(""),
location(""), videoUrl(""),
coverUrl(""), createTime("") {
}

SurveillanceVideo::SurveillanceVideo(int userId, const std::string& cameraName, const std::string& location,
									 const std::string& videoUrl, const std::string& coverUrl,
									 const std::string& createTime) : userId(userId), cameraName(cameraName),
	location(location), videoUrl(videoUrl),
	coverUrl(coverUrl), createTime(createTime) {}

SurveillanceVideo::SurveillanceVideo(int id, int userId, const std::string& cameraName, const std::string& location,
									 const std::string& videoUrl, const std::string& coverUrl,
									 const std::string& createTime) : id(id), userId(userId), cameraName(cameraName),
	location(location), videoUrl(videoUrl),
	coverUrl(coverUrl), createTime(createTime) {}

int SurveillanceVideo::getId() const {
	return id;
}

void SurveillanceVideo::setId(int id) {
	SurveillanceVideo::id = id;
}

int SurveillanceVideo::getUserId() const {
	return userId;
}

void SurveillanceVideo::setUserId(int userId) {
	SurveillanceVideo::userId = userId;
}

const std::string& SurveillanceVideo::getCameraName() const {
	return cameraName;
}

void SurveillanceVideo::setCameraName(const std::string& cameraName) {
	SurveillanceVideo::cameraName = cameraName;
}

const std::string& SurveillanceVideo::getLocation() const {
	return location;
}

void SurveillanceVideo::setLocation(const std::string& location) {
	SurveillanceVideo::location = location;
}

const std::string& SurveillanceVideo::getVideoUrl() const {
	return videoUrl;
}

void SurveillanceVideo::setVideoUrl(const std::string& videoUrl) {
	SurveillanceVideo::videoUrl = videoUrl;
}

const std::string& SurveillanceVideo::getCoverUrl() const {
	return coverUrl;
}

void SurveillanceVideo::setCoverUrl(const std::string& coverUrl) {
	SurveillanceVideo::coverUrl = coverUrl;
}

const std::string& SurveillanceVideo::getCreateTime() const {
	return createTime;
}

void SurveillanceVideo::setCreateTime(const std::string& createTime) {
	SurveillanceVideo::createTime = createTime;
}

SurveillanceVideo::~SurveillanceVideo() {
}