#pragma once

#include <string>
#include <vector>

#include "../entity/ParkingRecord.h"
#include "../../utility/db/SingleDB.h"
using namespace db;

class ParkingRecordDAO
{
public:
	ParkingRecordDAO();
	~ParkingRecordDAO();

	// 获取单条停车记录信息根据Id
	ParkingRecord* getParkingRecordById(int id);
	// 获取单条停车记录信息根据车牌号
	ParkingRecord* getParkingRecordByCarNumber(const std::string carNumber);
	// 更新停车记录信息
	bool updateParkingRecord(const ParkingRecord& data);
	// 添加停车记录信息
	bool addParkingRecord(const ParkingRecord& data);
	// 分页查询
	std::vector<ParkingRecord> getPageOfParkingRecords(int pageNumber, int recordsPerPage, std::string condition, std::string oder);

private:
	SingleDB* db;
};
