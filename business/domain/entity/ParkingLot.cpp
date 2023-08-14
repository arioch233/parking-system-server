#include "ParkingLot.h"

ParkingLot::ParkingLot() {}

ParkingLot::ParkingLot(const std::string& name, const std::string& location, int totalSpaces, int availableSpaces,
					   const std::string& createTime, const std::string& updateTime) : name(name), location(location),
	totalSpaces(totalSpaces),
	availableSpaces(availableSpaces),
	createTime(createTime),
	updateTime(updateTime) {}

ParkingLot::ParkingLot(int id, const std::string& name, const std::string& location, int totalSpaces,
					   int availableSpaces, const std::string& createTime, const std::string& updateTime) : id(id),
	name(name),
	location(
			location),
	totalSpaces(
			totalSpaces),
	availableSpaces(
			availableSpaces),
	createTime(
			createTime),
	updateTime(
			updateTime) {}

ParkingLot::~ParkingLot() {
}

int ParkingLot::getId() const {
	return id;
}

void ParkingLot::setId(int id) {
	ParkingLot::id = id;
}

const std::string& ParkingLot::getName() const {
	return name;
}

void ParkingLot::setName(const std::string& name) {
	ParkingLot::name = name;
}

const std::string& ParkingLot::getLocation() const {
	return location;
}

void ParkingLot::setLocation(const std::string& location) {
	ParkingLot::location = location;
}

int ParkingLot::getTotalSpaces() const {
	return totalSpaces;
}

void ParkingLot::setTotalSpaces(int totalSpaces) {
	ParkingLot::totalSpaces = totalSpaces;
}

int ParkingLot::getAvailableSpaces() const {
	return availableSpaces;
}

void ParkingLot::setAvailableSpaces(int availableSpaces) {
	ParkingLot::availableSpaces = availableSpaces;
}

const std::string& ParkingLot::getCreateTime() const {
	return createTime;
}

void ParkingLot::setCreateTime(const std::string& createTime) {
	ParkingLot::createTime = createTime;
}

const std::string& ParkingLot::getUpdateTime() const {
	return updateTime;
}

void ParkingLot::setUpdateTime(const std::string& updateTime) {
	ParkingLot::updateTime = updateTime;
}