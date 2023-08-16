#pragma once
#include <string>

class ParkingController
{
public:
	ParkingController();
	~ParkingController();
	// 获取停车场信息
	std::string getParkingInfo();
	// 车辆入场
	std::string entryCar(int userId, std::string carNumber, std::string imgPath, std::string address, std::string time);
	// 车辆出场
	std::string exitCar(int userId, std::string carNumber, std::string imgPath, std::string address, std::string time);

private:
};
