#include "ParkingRecord.h"

ParkingRecord::ParkingRecord() : userId(0), carNumber(""), entryUrl(""),
entryAddress(""), entryTime(""),
exitUrl(""), exitAddress(""),
exitTime(""), parkingDuration(""),
parkingFee(0.0), paymentStatus(0),
createTime("") {
}

ParkingRecord::~ParkingRecord() {
}

ParkingRecord::ParkingRecord(int userId, const std::string& carNumber, const std::string& entryUrl,
							 const std::string& entryAddress, const std::string& entryTime, const std::string& exitUrl,
							 const std::string& exitAddress, const std::string& exitTime,
							 const std::string& parkingDuration, float parkingFee, int paymentStatus,
							 const std::string& createTime) : userId(userId), carNumber(carNumber), entryUrl(entryUrl),
	entryAddress(entryAddress), entryTime(entryTime),
	exitUrl(exitUrl), exitAddress(exitAddress),
	exitTime(exitTime), parkingDuration(parkingDuration),
	parkingFee(parkingFee), paymentStatus(paymentStatus),
	createTime(createTime) {}

ParkingRecord::ParkingRecord(int id, int userId, const std::string& carNumber, const std::string& entryUrl,
							 const std::string& entryAddress, const std::string& entryTime, const std::string& exitUrl,
							 const std::string& exitAddress, const std::string& exitTime,
							 const std::string& parkingDuration, float parkingFee, int paymentStatus,
							 const std::string& createTime) : id(id), userId(userId), carNumber(carNumber),
	entryUrl(entryUrl), entryAddress(entryAddress),
	entryTime(entryTime), exitUrl(exitUrl),
	exitAddress(exitAddress), exitTime(exitTime),
	parkingDuration(parkingDuration), parkingFee(parkingFee),
	paymentStatus(paymentStatus), createTime(createTime) {}

int ParkingRecord::getId() const {
	return id;
}

void ParkingRecord::setId(int id) {
	ParkingRecord::id = id;
}

int ParkingRecord::getUserId() const {
	return userId;
}

void ParkingRecord::setUserId(int userId) {
	ParkingRecord::userId = userId;
}

const std::string& ParkingRecord::getCarNumber() const {
	return carNumber;
}

void ParkingRecord::setCarNumber(const std::string& carNumber) {
	ParkingRecord::carNumber = carNumber;
}

const std::string& ParkingRecord::getEntryUrl() const {
	return entryUrl;
}

void ParkingRecord::setEntryUrl(const std::string& entryUrl) {
	ParkingRecord::entryUrl = entryUrl;
}

const std::string& ParkingRecord::getEntryAddress() const {
	return entryAddress;
}

void ParkingRecord::setEntryAddress(const std::string& entryAddress) {
	ParkingRecord::entryAddress = entryAddress;
}

const std::string& ParkingRecord::getEntryTime() const {
	return entryTime;
}

void ParkingRecord::setEntryTime(const std::string& entryTime) {
	ParkingRecord::entryTime = entryTime;
}

const std::string& ParkingRecord::getExitUrl() const {
	return exitUrl;
}

void ParkingRecord::setExitUrl(const std::string& exitUrl) {
	ParkingRecord::exitUrl = exitUrl;
}

const std::string& ParkingRecord::getExitAddress() const {
	return exitAddress;
}

void ParkingRecord::setExitAddress(const std::string& exitAddress) {
	ParkingRecord::exitAddress = exitAddress;
}

const std::string& ParkingRecord::getExitTime() const {
	return exitTime;
}

void ParkingRecord::setExitTime(const std::string& exitTime) {
	ParkingRecord::exitTime = exitTime;
}

const std::string& ParkingRecord::getParkingDuration() const {
	return parkingDuration;
}

void ParkingRecord::setParkingDuration(const std::string& parkingDuration) {
	ParkingRecord::parkingDuration = parkingDuration;
}

float ParkingRecord::getParkingFee() const {
	return parkingFee;
}

void ParkingRecord::setParkingFee(float parkingFee) {
	ParkingRecord::parkingFee = parkingFee;
}

int ParkingRecord::getPaymentStatus() const {
	return paymentStatus;
}

void ParkingRecord::setPaymentStatus(int paymentStatus) {
	ParkingRecord::paymentStatus = paymentStatus;
}

const std::string& ParkingRecord::getCreateTime() const {
	return createTime;
}

void ParkingRecord::setCreateTime(const std::string& createTime) {
	ParkingRecord::createTime = createTime;
}