#include "ParkingController.h"

#include <cerrno>
#include <vector>

#include "../entity/User.h"
#include "../../utility/json/Json.h"
#include "../../utility/logger/Logger.h"

using namespace json;
using namespace utility;
ParkingController::ParkingController()
{
}

ParkingController::~ParkingController()
{
}

std::string ParkingController::getParkingInfo()
{
	std::string jsonStr = "";
	ParkingLot* parkingLot = this->parkingLotDAO.getParkingLotInfoById(1);
	if (parkingLot == nullptr)
	{
		ParkingLot temp;
		temp.setName("XXX停车场");
		temp.setLocation("XXX路XXX号");
		temp.setTotalSpaces(2000);
		temp.setAvailableSpaces(0);
		this->parkingLotDAO.addParkingLotInfo(temp);
		parkingLot = this->parkingLotDAO.getParkingLotInfoById(1);
	}
	Json result;
	result["flag"] = true;
	result["msg"] = "获取停车";
	Json data;
	data["name"] = parkingLot->getName();
	data["location"] = parkingLot->getLocation();
	data["total"] = parkingLot->getTotalSpaces();
	data["available"] = parkingLot->getAvailableSpaces();
	result["data"] = data;
	jsonStr = result.str();
	return jsonStr;
}

std::string ParkingController::entryCar(int userId, std::string carNumber, std::string imgPath, std::string address, std::string time)
{
	std::string jsonStr = "";
	ParkingRecord record;
	record.setUserId(userId);
	record.setCarNumber(carNumber);
	record.setEntryUrl(imgPath);
	record.setEntryAddress(address);
	record.setEntryTime(time);
	record.setPaymentStatus(0);
	Json result;
	if (this->parkingRecordDAO.addParkingRecord(record)) {
		result["flag"] = true;
		result["msg"] = carNumber + " 入场";
	}
	else
	{
		result["flag"] = false;
		result["msg"] = carNumber + " 入场失败";
	}
	jsonStr = result.str();
	return jsonStr;
}

std::string ParkingController::getCarEntryInfo(std::string carNumber)
{
	std::string jsonStr = "";
	ParkingRecord* record = this->parkingRecordDAO.getParkingRecordByCarNumber(carNumber);
	Json result;
	if (record == nullptr)
	{
		result["flag"] = false;
		result["msg"] = "不存在改车辆的入场记录";
	}
	else
	{
		result["flag"] = true;
		result["msg"] = "获取入场记录成功";
		Json content;
		content["entry_time"] = record->getEntryTime();
		result["data"] = content;
	}
	jsonStr = result.str();
	return jsonStr;
}

std::string ParkingController::exitCar(int userId, std::string carNumber, std::string imgPath, std::string address, std::string time, double fee)
{
	std::string jsonStr = "";
	ParkingRecord record;
	record.setUserId(userId);
	record.setCarNumber(carNumber);
	record.setExitUrl(imgPath);
	record.setExitAddress(address);
	record.setExitTime(time);
	record.setParkingFee(fee);
	record.setPaymentStatus(1);
	Json result;
	if (this->parkingRecordDAO.updateParkingRecord(record))
	{
		result["flag"] = true;
		result["msg"] = carNumber + " 出场";
	}
	else
	{
		result["flag"] = false;
		result["msg"] = carNumber + " 出场失败";
	}

	jsonStr = result.str();
	return jsonStr;
}

std::string ParkingController::getEntryCarRecords()
{
	std::string jsonStr = "";
	std::vector<ParkingRecord> records = this->parkingRecordDAO.getPageOfParkingRecords(0, 4, "", "entry_time DESC");
	Json result;
	if (records.empty() && records.size() == 0)
	{
		result["flag"] = true;
		result["msg"] = "入场记录为空";
		Json arr(Json::JSON_ARRAY);
		result["data"] = arr;
	}
	else
	{
		result["flag"] = true;
		result["msg"] = "共有 " + std::to_string(records.size()) + "记录";
		Json arr(Json::JSON_ARRAY);
		for (int i = 0; i < records.size(); ++i)
		{
			Json temp;
			temp["id"] = records[i].getId();
			temp["car_number"] = records[i].getCarNumber();
			temp["entry_time"] = records[i].getEntryTime();
			temp["entry_address"] = records[i].getEntryAddress();
			arr.append(temp);
		}
		result["data"] = arr;
	}
	jsonStr = result.str();
	return jsonStr;
}

std::string ParkingController::getParkingRecords(int pageNumber, int recordsPerPage, std::string carNumber, std::string startTime, std::string endTime)
{
	std::string jsonStr = "";
	std::string condition = "";
	// 拼接条件查询
	if (!carNumber.empty())
	{
		condition = "car_number LIKE '%" + carNumber + "%'";
	}
	if (!startTime.empty())
	{
		condition += "AND created_at >= ";
		condition += startTime;
	}
	if (!endTime.empty())
	{
		condition += "AND created_at <= ";
		condition += endTime;
	}
	condition += "AND payment_status = 1";
	std::vector<ParkingRecord> records = this->parkingRecordDAO.getPageOfParkingRecords(pageNumber, recordsPerPage, condition, "entry_time DESC");
	Json result;
	if (records.empty() && records.size() == 0)
	{
		result["flag"] = true;
		result["msg"] = "入场记录为空";
		Json arr(Json::JSON_ARRAY);
		result["data"] = arr;
	}
	else
	{
		result["flag"] = true;
		result["msg"] = "共有 " + std::to_string(records.size()) + "记录";
		Json arr(Json::JSON_ARRAY);
		for (int i = 0; i < records.size(); ++i)
		{
			Json temp;
			temp["id"] = records[i].getId();
			temp["car_number"] = records[i].getCarNumber();
			temp["entry_time"] = records[i].getEntryTime();
			temp["exit_time"] = records[i].getExitTime();
			temp["entry_url"] = records[i].getEntryUrl();
			temp["exit_url"] = records[i].getExitUrl();
			temp["parking_fee"] = records[i].getParkingFee();
			arr.append(temp);
		}
		result["data"] = arr;
	}
	jsonStr = result.str();
	return jsonStr;
}