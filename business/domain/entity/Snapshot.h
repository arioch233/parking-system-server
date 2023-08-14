#pragma once
#include <string>

class Snapshot {
public:
	Snapshot();

	Snapshot(int captureType, const std::string& captureTime, int userId, int videoId, const std::string& filePath,
			 const std::string& description, const std::string& createTime);

	Snapshot(int id, int captureType, const std::string& captureTime, int userId, int videoId,
			 const std::string& filePath, const std::string& description, const std::string& createTime);

	virtual ~Snapshot();

	int getId() const;

	void setId(int id);

	int getCaptureType() const;

	void setCaptureType(int captureType);

	const std::string& getCaptureTime() const;

	void setCaptureTime(const std::string& captureTime);

	int getUserId() const;

	void setUserId(int userId);

	int getVideoId() const;

	void setVideoId(int videoId);

	const std::string& getFilePath() const;

	void setFilePath(const std::string& filePath);

	const std::string& getDescription() const;

	void setDescription(const std::string& description);

	const std::string& getCreateTime() const;

	void setCreateTime(const std::string& createTime);

private:
	int id;
	int captureType;
	std::string captureTime;
	int userId;
	int videoId;
	std::string  filePath;
	std::string description;
	std::string createTime;
};
