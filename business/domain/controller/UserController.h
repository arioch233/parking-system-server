#pragma once

#include "../dao/UserDAO.h"

#include <string>

class UserController
{
public:
	UserController();
	~UserController();

public:
	// 用户登录
	std::string userLogin(std::string username, std::string password);
	// 用户注册
	std::string userRegister(std::string username, std::string password, std::string nickname);

private:
	UserDAO userDao;
};
