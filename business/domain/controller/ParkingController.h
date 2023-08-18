#pragma once

#include <string>

#include "../dao/ParkingRecordDAO.h"
#include "../dao/ParkingLotDAO.h"

class ParkingController
{
public:
	ParkingController();
	~ParkingController();
	// 获取停车场信息
	std::string getParkingInfo();
	// 车辆入场
	std::string entryCar(int userId, std::string carNumber, std::string imgPath, std::string address, std::string time);
	// 获取车辆入场信息
	std::string getCarEntryInfo(std::string carNumber);
	// 车辆出场
	std::string exitCar(int userId, std::string carNumber, std::string imgPath, std::string address, std::string time, double fee);
	// 获取入场车辆信息列表(最新4条入场记录)
	std::string getEntryCarRecords();
	// 获取车辆停车记录表分页查询
	std::string getParkingRecords(int pageNumber, int recordsPerPage, std::string carNumber, std::string startTime, std::string endTime);

private:
	ParkingRecordDAO parkingRecordDAO;
	ParkingLotDAO parkingLotDAO;
};
