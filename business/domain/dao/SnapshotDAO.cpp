#include "SnapshotDAO.h"

#include "../../utility/Singleton.h"
#include "../../utility/db/SQLQueryBuilder.h"

using namespace utility;

SnapshotDAO::SnapshotDAO()
{
	this->db = Singleton<SingleDB>::getInstance();
}

SnapshotDAO::~SnapshotDAO()
{
}

bool SnapshotDAO::addSnapshot(Snapshot snapshot)
{
	// 构建SQL
	SQLQueryBuilder builder;
	std::string values = "'" + snapshot.getCaptureTime() + "', " +
		std::to_string(snapshot.getCaptureType()) + ", " +
		std::to_string(snapshot.getVideoId()) + ", " +
		std::to_string(snapshot.getUserId()) + ", '" +
		snapshot.getFilePath() + "', '" +
		snapshot.getDescription() + "', " +
		"DATETIME('now')";
	std::string sql = builder
		.insertInto("snapshots", "capture_time, capture_type, video_id, user_id, file_path, description, created_at")
		.values(values)
		.build();

	if (this->db->doExec(sql) == 0)
	{
		return true;
	}
	return false;
}

std::vector<Snapshot> SnapshotDAO::getSnapShotList(int pageNumber, int recordsPerPage, std::string condition, std::string order)
{
	std::vector<Snapshot> records;
	SQLQueryBuilder builder;

	builder.select("*")
		.from("snapshots");

	if (!condition.empty()) {
		builder.where(condition);
	}

	if (!order.empty()) {
		builder.orderBy(order);
	}

	int offset = (pageNumber - 1) * recordsPerPage;
	builder.limit(offset, recordsPerPage);
	std::string sql = builder.build();

	char** qres;
	int row, col;
	int res = this->db->doSQL(sql.c_str(), qres, row, col);
	if (res == 0) {
		if (row > 0) {
			for (int i = 1; i <= row; i++) {
				int id = atoi(qres[i * col]);
				int captureType = atoi(qres[i * col + 2]);

				Snapshot snapshot(id, captureType, qres[i * col + 1], atoi(qres[i * col + 4]), atoi(qres[i * col + 3]),
								  qres[i * col + 5], qres[i * col + 6], qres[i * col + 7]);
				records.push_back(snapshot);
			}
		}
	}
	return records;
}

int SnapshotDAO::getSnapShotCount(std::string condition)
{
	SQLQueryBuilder builder;

	builder.select("*")
		.from("snapshots");

	if (!condition.empty()) {
		builder.where(condition);
	}

	std::string sql = builder.build();

	char** qres;
	int row, col;
	int res = this->db->doSQL(sql.c_str(), qres, row, col);
	if (res == 0) {
		if (row > 0) {
			return row;
		}
	}
	return 0;
}

std::vector<std::string> SnapshotDAO::getMenuByDay()
{
	std::vector<std::string> results;
	std::string sql = "SELECT DISTINCT strftime('%Y-%m-%d', created_at) FROM snapshots;";
	char** qres;
	int row, col;
	int res = this->db->doSQL(sql.c_str(), qres, row, col);
	if (res == SQLITE_OK) {
		for (int i = 1; i <= row; i++) {
			results.push_back(qres[i]);
		}
	}
	return results;
}

std::vector<std::string> SnapshotDAO::getMenuByMouth()
{
	std::vector<std::string> results;
	std::string sql = "SELECT DISTINCT strftime('%Y-%m', created_at) FROM snapshots;";
	char** qres;
	int row, col;
	int res = this->db->doSQL(sql.c_str(), qres, row, col);
	if (res == SQLITE_OK) {
		for (int i = 1; i <= row; i++) {
			results.push_back(qres[i]);
		}
	}
	return results;
}