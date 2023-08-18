#pragma once

#include <string>
#include <vector>

#include "../entity/ParkingLot.h"
#include "../../utility/db/SingleDB.h"
using namespace db;

class ParkingLotDAO
{
public:
	ParkingLotDAO();
	~ParkingLotDAO();

public:
	// 获取停车场信息
	ParkingLot* getParkingLotInfoById(int id);
	// 更新停车场信息
	bool updateParkingLotInfo(ParkingLot& data);
	// 添加停车场信息
	bool addParkingLotInfo(ParkingLot& data);

private:
	SingleDB* db;
};
