#include "UserController.h"

#include <cerrno>

#include "../entity/User.h"
#include "../../utility/json/Json.h"
#include "../../utility/logger/Logger.h"

using namespace json;
using namespace utility;

UserController::UserController()
{
}

UserController::~UserController()
{
}

std::string UserController::userLogin(std::string username, std::string password)
{
	std::string jsonStr = "";
	User* user = this->userDao.getUserByUsername(username);
	if (user != nullptr)
	{
		// 账号密码正确
		if (user->getPassword().compare(password) == 0)
		{
			Json obj;
			obj["flag"] = true;
			obj["msg"] = "登录成功";
			Json userInfo;
			userInfo["id"] = user->getId();
			userInfo["nickname"] = user->getNickname();
			obj["data"] = userInfo;
			jsonStr = obj.str();
			delete user;
			return jsonStr;
		}
	}
	// 用户不存在以及账号密码错误反馈
	Json obj;
	obj["flag"] = false;
	obj["msg"] = "用户名或密码错误";
	jsonStr = obj.str();
	delete user;
	return jsonStr;
}

std::string UserController::userRegister(std::string username, std::string password, std::string nickname)
{
	std::string jsonStr = "";
	User* user = this->userDao.getUserByUsername(username);
	if (user == nullptr)
	{
		User user;
		user.setUsername(username);
		user.setPassword(password);
		user.setNickname(nickname);
		// 注册成功
		if (this->userDao.addUser(user)) {
			//info("user register success");
			Json obj;
			obj["flag"] = true;
			obj["msg"] = "注册成功";
			jsonStr = obj.str();
			return jsonStr;
		}
		//warn("user register fail");
	}
	// 注册失败
	Json obj;
	obj["flag"] = false;
	obj["msg"] = "此用户已存在";
	jsonStr = obj.str();
	return jsonStr;
}