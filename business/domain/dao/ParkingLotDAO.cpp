#include "ParkingLotDAO.h"

#include <cstring>
#include <ctime>

#include "../../utility/Singleton.h"
#include "../../utility/db/SQLQueryBuilder.h"
using namespace utility;

ParkingLotDAO::ParkingLotDAO()
{
	this->db = Singleton<SingleDB>::getInstance();
}

ParkingLotDAO::~ParkingLotDAO()
{
}

ParkingLot* ParkingLotDAO::getParkingLotInfoById(int id)
{
	ParkingLot* data = nullptr;
	SQLQueryBuilder builder;
	std::string sql = builder.select("*")
		.from("parking_lots")
		.where("id = " + std::to_string(id))
		.build();
	char** qres;
	int row, col;
	int res = this->db->doSQL(sql.c_str(), qres, row, col);
	if (res == 0) {
		if (row > 0) {
			data = new ParkingLot(atoi(qres[7]), qres[8], qres[9], atoi(qres[10]), atoi(qres[11]), qres[12], qres[13]);
		}
	}
	return data;
}

bool ParkingLotDAO::updateParkingLotInfo(ParkingLot& data)
{
	SQLQueryBuilder builder;
	builder.update("parking_lots");
	if (!data.getName().empty()) {
		builder.set("name", "'" + data.getName() + "'");
	}

	if (!data.getLocation().empty()) {
		builder.set("location", "'" + data.getLocation() + "'");
	}

	if (data.getTotalSpaces() > 0) {
		builder.set("total_spaces", std::to_string(data.getTotalSpaces()));
	}

	if (data.getAvailableSpaces() >= 0) {
		builder.set("available_spaces", std::to_string(data.getAvailableSpaces()));
	}

	builder.where("id = " + std::to_string(data.getId()));

	std::string sql = builder
		.build();
	if (this->db->doExec(sql) == 0)
	{
		return true;
	}
	return false;
}

bool ParkingLotDAO::addParkingLotInfo(ParkingLot& data)
{
	// 构建SQL
	SQLQueryBuilder builder;
	std::string sql = builder.insertInto("parking_lots", "name, location, total_spaces, available_spaces, created_at, updated_at")
		.values("'" + data.getName() + "', '" + data.getLocation() + "', " + std::to_string(data.getTotalSpaces()) + ", " +
				std::to_string(data.getAvailableSpaces()) + ", DATETIME('now'), DATETIME('now')")
		.build();

	if (this->db->doExec(sql) == 0)
	{
		return true;
	}
	return false;
}