#include "ParkingRecordDAO.h"

#include "../../utility/Singleton.h"
#include "../../utility/db/SQLQueryBuilder.h"
using namespace utility;

ParkingRecordDAO::ParkingRecordDAO()
{
	this->db = Singleton<SingleDB>::getInstance();
}

ParkingRecordDAO::~ParkingRecordDAO()
{
}

ParkingRecord* ParkingRecordDAO::getParkingRecordById(int id)
{
	ParkingRecord* data = nullptr;
	SQLQueryBuilder builder;
	std::string sql = builder.select("*")
		.from("parking_records")
		.where("id = " + std::to_string(id))
		.build();
	char** qres;
	int row, col;
	int res = this->db->doSQL(sql.c_str(), qres, row, col);
	if (res == 0) {
		if (row > 0) {
			data = new ParkingRecord();
			data->setId(atoi(qres[13]));
			data->setUserId(atoi(qres[14]));
			data->setCarNumber(std::string(qres[15]));
			data->setEntryUrl(std::string(qres[16]));
			data->setEntryAddress(std::string(qres[17]));
			data->setEntryTime(std::string(qres[18]));
			data->setExitUrl(std::string(qres[19]));
			data->setExitAddress(std::string(qres[20]));
			data->setExitTime(std::string(qres[21]));
			data->setParkingDuration(std::string(qres[22]));
			data->setParkingFee(std::atof(qres[23]));
			data->setPaymentStatus(std::atoi(qres[24]));
			data->setCreateTime(std::string(qres[25]));
		}
	}
	return data;
}

ParkingRecord* ParkingRecordDAO::getParkingRecordByCarNumber(const std::string carNumber)
{
	ParkingRecord* data = nullptr;
	SQLQueryBuilder builder;
	std::string sql = builder.select("*")
		.from("parking_records")
		.where("id = '" + carNumber + "'")
		.build();
	char** qres;
	int row, col;
	int res = this->db->doSQL(sql.c_str(), qres, row, col);
	if (res == 0) {
		if (row > 0) {
			data = new ParkingRecord();
			data->setId(atoi(qres[13]));
			data->setUserId(atoi(qres[14]));
			data->setCarNumber(std::string(qres[15]));
			data->setEntryUrl(std::string(qres[16]));
			data->setEntryAddress(std::string(qres[17]));
			data->setEntryTime(std::string(qres[18]));
			data->setExitUrl(std::string(qres[19]));
			data->setExitAddress(std::string(qres[20]));
			data->setExitTime(std::string(qres[21]));
			data->setParkingDuration(std::string(qres[22]));
			data->setParkingFee(std::atof(qres[23]));
			data->setPaymentStatus(std::atoi(qres[24]));
			data->setCreateTime(std::string(qres[25]));
		}
	}
	return data;
}

bool ParkingRecordDAO::updateParkingRecord(const ParkingRecord& data)
{
	SQLQueryBuilder builder;
	builder.update("parking_records");

	if (!data.getUserId() >= 0) {
		builder.set("user_id", "'" + std::to_string(data.getUserId()) + "'");
	}

	if (!data.getCarNumber().empty()) {
		builder.set("car_number", "'" + data.getCarNumber() + "'");
	}

	if (!data.getEntryUrl().empty()) {
		builder.set("entry_url", "'" + data.getEntryUrl() + "'");
	}

	if (!data.getExitUrl().empty()) {
		builder.set("exit_url", "'" + data.getExitUrl() + "'");
	}

	if (!data.getEntryAddress().empty()) {
		builder.set("entry_address", "'" + data.getEntryAddress() + "'");
	}

	if (!data.getExitAddress().empty()) {
		builder.set("exit_address", "'" + data.getExitAddress() + "'");
	}

	if (data.getParkingDuration().empty()) {
		builder.set("parking_duration", data.getParkingDuration());
	}

	if (data.getParkingFee() >= 0.0) {
		builder.set("parking_fee", std::to_string(data.getParkingFee()));
	}

	if (data.getPaymentStatus() >= 1 && data.getPaymentStatus() <= 2) {
		builder.set("payment_status", std::to_string(data.getPaymentStatus()));
	}

	builder.where("id = " + std::to_string(data.getId()));

	std::string sql = builder.build();

	if (this->db->doExec(sql) == 0) {
		return true;
	}

	return false;
}

bool ParkingRecordDAO::addParkingRecord(const ParkingRecord& data)
{
	// 构建SQL
	SQLQueryBuilder builder;
	std::string sql = builder.insertInto("parking_records", "user_id, car_number, entry_url, entry_address, entry_time, exit_url, exit_address, exit_time, parking_duration, parking_fee, payment_status, created_at")
		.values(std::to_string(data.getUserId()) + ", '" + data.getCarNumber() + "', '" + data.getEntryUrl() + "', '" + data.getEntryAddress() + data.getEntryTime() +
				data.getExitUrl() + "', '" + data.getExitAddress() + data.getExitTime() + data.getParkingDuration() + ", " +
				std::to_string(data.getParkingFee()) + ", " + std::to_string(data.getPaymentStatus()) + ", DATETIME('now')")
		.build();

	if (this->db->doExec(sql) == 0)
	{
		return true;
	}
	return false;
}

std::vector<ParkingRecord> ParkingRecordDAO::getPageOfParkingRecords(int pageNumber, int recordsPerPage, std::string condition, std::string order)
{
	std::vector<ParkingRecord> records;
	SQLQueryBuilder builder;

	builder.select("*")
		.from("parking_records");

	if (!condition.empty()) {
		builder.where(condition);
	}

	if (!order.empty()) {
		builder.orderBy(order);
	}

	int offset = (pageNumber - 1) * recordsPerPage;
	builder.limit(recordsPerPage, offset);
	std::string sql = builder.build();

	char** qres;
	int row, col;
	int res = this->db->doSQL(sql.c_str(), qres, row, col);
	if (res == 0) {
		if (row > 0) {
			for (int i = 1; i <= row; ++i) {
				int index = i * col;
				ParkingRecord record(
					std::stoi(qres[index]),                    // id
					std::stoi(qres[index + 1]),                // userId
					qres[index + 2],                           // carNumber
					qres[index + 3],                           // entryUrl
					qres[index + 4],                           // entryAddress
					qres[index + 5],                           // entryTime
					qres[index + 6],                           // exitUrl
					qres[index + 7],                           // exitAddress
					qres[index + 8],                           // exitTime
					qres[index + 9],                           // parkingDuration
					std::stod(qres[index + 10]),               // parkingFee
					std::stoi(qres[index + 11]),               // paymentStatus
					qres[index + 12]                           // createTime
				);
				records.push_back(record);
			}
		}
	}
	return records;
}