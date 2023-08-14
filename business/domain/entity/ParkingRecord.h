#pragma once
#include <string>

class ParkingRecord {
public:
	ParkingRecord();

	ParkingRecord(int userId, const std::string& carNumber, const std::string& entryUrl,
				  const std::string& entryAddress, const std::string& entryTime, const std::string& exitUrl,
				  const std::string& exitAddress, const std::string& exitTime, const std::string& parkingDuration,
				  float parkingFee, int paymentStatus, const std::string& createTime);

	ParkingRecord(int id, int userId, const std::string& carNumber, const std::string& entryUrl,
				  const std::string& entryAddress, const std::string& entryTime, const std::string& exitUrl,
				  const std::string& exitAddress, const std::string& exitTime, const std::string& parkingDuration,
				  float parkingFee, int paymentStatus, const std::string& createTime);

	virtual ~ParkingRecord();

	int getId() const;

	void setId(int id);

	int getUserId() const;

	void setUserId(int userId);

	const std::string& getCarNumber() const;

	void setCarNumber(const std::string& carNumber);

	const std::string& getEntryUrl() const;

	void setEntryUrl(const std::string& entryUrl);

	const std::string& getEntryAddress() const;

	void setEntryAddress(const std::string& entryAddress);

	const std::string& getEntryTime() const;

	void setEntryTime(const std::string& entryTime);

	const std::string& getExitUrl() const;

	void setExitUrl(const std::string& exitUrl);

	const std::string& getExitAddress() const;

	void setExitAddress(const std::string& exitAddress);

	const std::string& getExitTime() const;

	void setExitTime(const std::string& exitTime);

	const std::string& getParkingDuration() const;

	void setParkingDuration(const std::string& parkingDuration);

	float getParkingFee() const;

	void setParkingFee(float parkingFee);

	int getPaymentStatus() const;

	void setPaymentStatus(int paymentStatus);

	const std::string& getCreateTime() const;

	void setCreateTime(const std::string& createTime);

private:
	int id;
	int userId;
	std::string carNumber;
	std::string entryUrl;
	std::string entryAddress;
	std::string entryTime;
	std::string exitUrl;
	std::string exitAddress;
	std::string exitTime;
	std::string parkingDuration;
	float parkingFee;
	int paymentStatus;
	std::string createTime;
};
