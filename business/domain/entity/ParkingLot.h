#pragma once
#include <string>

class ParkingLot {
public:
	ParkingLot();

	ParkingLot(const std::string& name, const std::string& location, int totalSpaces, int availableSpaces,
			   const std::string& createTime, const std::string& updateTime);

	ParkingLot(int id, const std::string& name, const std::string& location, int totalSpaces, int availableSpaces,
			   const std::string& createTime, const std::string& updateTime);

	virtual ~ParkingLot();

	int getId() const;

	void setId(int id);

	const std::string& getName() const;

	void setName(const std::string& name);

	const std::string& getLocation() const;

	void setLocation(const std::string& location);

	int getTotalSpaces() const;

	void setTotalSpaces(int totalSpaces);

	int getAvailableSpaces() const;

	void setAvailableSpaces(int availableSpaces);

	const std::string& getCreateTime() const;

	void setCreateTime(const std::string& createTime);

	const std::string& getUpdateTime() const;

	void setUpdateTime(const std::string& updateTime);

private:
	int id;
	std::string name;
	std::string location;
	int totalSpaces;
	int availableSpaces;
	std::string createTime;
	std::string updateTime;
};