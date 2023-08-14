#pragma once

#include <string>

class User {
public:
	User();

	User(const std::string& username, const std::string& password, const std::string& nickname,
		 const std::string& createdTime, const std::string& updatedTime);

	User(int id, const std::string& username, const std::string& password, const std::string& nickname,
		 const std::string& createdTime, const std::string& updatedTime);

	virtual ~User();

public:
	int getId() const;

	void setId(int id);

	const std::string& getUsername() const;

	void setUsername(const std::string& username);

	const std::string& getPassword() const;

	void setPassword(const std::string& password);

	const std::string& getNickname() const;

	void setNickname(const std::string& nickname);

	const std::string& getCreatedTime() const;

	void setCreatedTime(const std::string& createdTime);

	const std::string& getUpdatedTime() const;

	void setUpdatedTime(const std::string& updatedTime);

private:
	int id;
	std::string username;
	std::string password;
	std::string nickname;
	std::string createdTime;
	std::string updatedTime;
};
