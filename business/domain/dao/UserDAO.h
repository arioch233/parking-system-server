#pragma once

#include <vector>
#include <string>

#include "../entity/User.h"
#include "../../utility/db/SingleDB.h"
using namespace db;

class UserDAO
{
public:
	UserDAO();
	~UserDAO();

public:
	// 根据用户id查询用户信息
	User* getUserById(int id);
	// 根据用户名查询用户信息
	User* getUserByUsername(std::string username);
	// 添加用户
	bool addUser(User user);

private:
	SingleDB* db;
};
