#include "UserDAO.h"

#include<iostream>
#include <cstring>
#include <ctime>

#include "../../utility/Singleton.h"
#include "../../utility/db/SQLQueryBuilder.h"

using namespace utility;

UserDAO::UserDAO()
{
	this->db = Singleton<SingleDB>::getInstance();
}

UserDAO::~UserDAO()
{
}

User* UserDAO::getUserById(int id)
{
	User* user = nullptr;
	SQLQueryBuilder builder;
	std::string sql = builder.select("*")
		.from("users")
		.where("id = " + id)
		.build();
	char** qres;
	int row, col;
	int res = this->db->doSQL(sql.c_str(), qres, row, col);
	if (res == 0) {
		if (row > 0) { //如果row大于0用户存在
			user = new User(atoi(qres[6]), qres[7], qres[8], qres[9], qres[10], qres[11]);
		}
	}
	return user;
}

User* UserDAO::getUserByUsername(std::string username)
{
	User* user = nullptr;
	SQLQueryBuilder builder;
	std::string sql = builder.select("*")
		.from("users")
		.where("username = '" + username + "'")
		.build();
	char** qres;
	int row, col;
	int res = this->db->doSQL(sql, qres, row, col);
	if (res == 0) {
		if (row > 0) { //如果row大于0用户存在
			user = new User(atoi(qres[6]), qres[7], qres[8], qres[9], qres[10], qres[11]);
		}
	}
	return user;
}

bool UserDAO::addUser(User user)
{
	// 获取时间
	time_t ticks = time(nullptr);
	struct tm* ptm = localtime(&ticks);
	char timestamp[32];
	memset(timestamp, 0, sizeof(timestamp));
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptm);
	// 构建SQL
	SQLQueryBuilder builder;
	std::string sql = builder.insertInto("users", "username, password, nickname, created_at, updated_at")
		.values("'" + user.getUsername() + "','" + user.getPassword() + "','" + user.getNickname() + "','" + string(timestamp) + "','" + string(timestamp) + "'")
		.build();

	if (this->db->doExec(sql) == 0)
	{
		return true;
	}
	return false;
}