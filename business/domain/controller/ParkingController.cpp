#include "ParkingController.h"

#include <cerrno>

#include "../entity/User.h"
#include "../../utility/json/Json.h"
#include "../../utility/logger/Logger.h"

using namespace json;
using namespace utility; ParkingController::ParkingController()
{
}

ParkingController::~ParkingController()
{
}

std::string ParkingController::getParkingInfo()
{
	std::string jsonStr = "";
	Json result;
	result["flag"] = true;
	result["msg"] = "获取停车";
	Json data;
	data["name"] = "XXX停车场";
	data["location"] = "XXX路XXX号";
	data["total"] = 2222;
	data["available"] = 1111;
	result["data"] = data;
	jsonStr = result.str();
	return jsonStr;
}

std::string ParkingController::entryCar(int userId, std::string carNumber, std::string imgPath, std::string address, std::string time)
{
	std::string jsonStr = "";
	Json result;
	result["flag"] = true;
	result["msg"] = carNumber + " 入场";
	jsonStr = result.str();
	return jsonStr;
}

std::string ParkingController::exitCar(int userId, std::string carNumber, std::string imgPath, std::string address, std::string time)
{
	std::string jsonStr = "";
	Json result;
	result["flag"] = true;
	result["msg"] = carNumber + " 出场";
	jsonStr = result.str();
	return jsonStr;
}