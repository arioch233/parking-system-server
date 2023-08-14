#include "User.h"

User::User() : id(0), username(""), password(""),
nickname(""),
createdTime(""),
updatedTime("") {
}

User::User(const std::string& username, const std::string& password, const std::string& nickname,
		   const std::string& createdTime, const std::string& updatedTime) : username(username), password(password),
	nickname(nickname),
	createdTime(createdTime),
	updatedTime(updatedTime) {
	this->id = 0;
}

User::User(int id, const std::string& username, const std::string& password, const std::string& nickname,
		   const std::string& createdTime, const std::string& updatedTime) : id(id), username(username),
	password(password), nickname(nickname),
	createdTime(createdTime),
	updatedTime(updatedTime) {
}

User::~User() {
}

int User::getId() const {
	return id;
}

void User::setId(int id) {
	User::id = id;
}

const std::string& User::getUsername() const {
	return username;
}

void User::setUsername(const std::string& username) {
	User::username = username;
}

const std::string& User::getPassword() const {
	return password;
}

void User::setPassword(const std::string& password) {
	User::password = password;
}

const std::string& User::getNickname() const {
	return nickname;
}

void User::setNickname(const std::string& nickname) {
	User::nickname = nickname;
}

const std::string& User::getCreatedTime() const {
	return createdTime;
}

void User::setCreatedTime(const std::string& createdTime) {
	User::createdTime = createdTime;
}

const std::string& User::getUpdatedTime() const {
	return updatedTime;
}

void User::setUpdatedTime(const std::string& updatedTime) {
	User::updatedTime = updatedTime;
}