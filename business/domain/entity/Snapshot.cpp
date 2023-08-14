#include "Snapshot.h"

Snapshot::Snapshot() {}

Snapshot::Snapshot(int captureType, const std::string& captureTime, int userId, int videoId,
				   const std::string& filePath, const std::string& description, const std::string& createTime)
	: captureType(captureType), captureTime(captureTime), userId(userId), videoId(videoId), filePath(filePath),
	description(description), createTime(createTime) {}

Snapshot::Snapshot(int id, int captureType, const std::string& captureTime, int userId, int videoId,
				   const std::string& filePath, const std::string& description, const std::string& createTime)
	: id(id),
	captureType(
			captureType),
	captureTime(
			captureTime),
	userId(userId),
	videoId(videoId),
	filePath(
			filePath),
	description(
			description),
	createTime(
			createTime) {
}

Snapshot::~Snapshot() {
}

int Snapshot::getId() const {
	return id;
}

void Snapshot::setId(int id) {
	Snapshot::id = id;
}

int Snapshot::getCaptureType() const {
	return captureType;
}

void Snapshot::setCaptureType(int captureType) {
	Snapshot::captureType = captureType;
}

const std::string& Snapshot::getCaptureTime() const {
	return captureTime;
}

void Snapshot::setCaptureTime(const std::string& captureTime) {
	Snapshot::captureTime = captureTime;
}

int Snapshot::getUserId() const {
	return userId;
}

void Snapshot::setUserId(int userId) {
	Snapshot::userId = userId;
}

int Snapshot::getVideoId() const {
	return videoId;
}

void Snapshot::setVideoId(int videoId) {
	Snapshot::videoId = videoId;
}

const std::string& Snapshot::getFilePath() const {
	return filePath;
}

void Snapshot::setFilePath(const std::string& filePath) {
	Snapshot::filePath = filePath;
}

const std::string& Snapshot::getDescription() const {
	return description;
}

void Snapshot::setDescription(const std::string& description) {
	Snapshot::description = description;
}

const std::string& Snapshot::getCreateTime() const {
	return createTime;
}

void Snapshot::setCreateTime(const std::string& createTime) {
	Snapshot::createTime = createTime;
}