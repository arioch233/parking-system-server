#include "Json.h"

#include <stdexcept>
#include <sstream>

using namespace json;

Json::Json() : m_type(JSON_NULL) {
	this->m_value.m_int = 0;
}

Json::Json(bool value) : m_type(JSON_BOOL) {
	this->m_value.m_bool = value;
}

Json::Json(int value) : m_type(JSON_INT) {
	this->m_value.m_int = value;
}

Json::Json(double value) : m_type(JSON_DOUBLE) {
	this->m_value.m_double = value;
}

Json::Json(const char* value) : m_type(JSON_STRING) {
	this->m_value.m_string = new std::string(value);
}

Json::Json(const std::string& value) : m_type(JSON_STRING) {
	this->m_value.m_string = new std::string(value);
}

Json::Json(Json::Type type) : m_type(type) {
	switch (this->m_type) {
	case JSON_NULL:
		break;
	case JSON_BOOL:
		this->m_value.m_bool = false;
		break;
	case JSON_INT:
		this->m_value.m_int = 0;
		break;
	case JSON_DOUBLE:
		this->m_value.m_double = 0.0;
		break;
	case JSON_STRING:
		this->m_value.m_string = new std::string("");
		break;
	case JSON_ARRAY:
		this->m_value.m_array = new std::vector<Json>();
		break;
	case JSON_OBJECT:
		this->m_value.m_object = new std::map<std::string, Json>();
		break;
	default:
		break;
	}
}

Json::Json(const Json& other) : m_type(JSON_NULL) {
	this->copy(other);
}

/*
 * 运算符重载
 */
Json::operator bool() {
	if (this->m_type != JSON_BOOL) {
		throw new std::logic_error("type error, not bool value");
	}
	return this->m_value.m_bool;
}

Json::operator int() {
	if (this->m_type != JSON_INT) {
		throw new std::logic_error("type error, not int value");
	}
	return this->m_value.m_int;
}

Json::operator double() {
	if (this->m_type != JSON_DOUBLE) {
		throw new std::logic_error("type error, not double value");
	}
	return this->m_value.m_double;
}

Json::operator std::string() {
	if (this->m_type != JSON_STRING) {
		throw new std::logic_error("type error, not string value");
	}
	return *this->m_value.m_string;
}

Json& Json::operator[](int index) {
	if (this->m_type != JSON_ARRAY) {
		this->m_type = JSON_ARRAY;
		this->m_value.m_array = new std::vector<Json>();
	}
	if (index < 0) {
		throw new std::logic_error("array[] index < 0");
	}
	size_t size = (this->m_value.m_array)->size();
	if (index >= size) {
		for (int i = size; i <= index; ++i) {
			(this->m_value.m_array)->push_back(Json());
		}
	}
	return (this->m_value.m_array)->at(index);
}

void Json::append(const Json& other) {
	if (this->m_type != JSON_ARRAY) {
		this->clear();
		this->m_type = JSON_ARRAY;
		this->m_value.m_array = new std::vector<Json>();
	}
	this->m_value.m_array->push_back(other);
}

std::string Json::str() const {
	std::stringstream ss;
	switch (this->m_type) {
	case JSON_NULL:
		ss << "null";
		break;
	case JSON_BOOL:
		if (this->m_value.m_bool) {
			ss << "true";
		}
		else {
			ss << "false";
		}
		break;
	case JSON_INT:
		ss << this->m_value.m_int;
		break;
	case JSON_DOUBLE:
		ss << this->m_value.m_double;
		break;
	case JSON_STRING:
		ss << '\"' << *(this->m_value.m_string) << '\"';
		break;
	case JSON_ARRAY:
		ss << '[';
		for (auto iter = (this->m_value.m_array)->begin();
			 iter != (this->m_value.m_array)->end();
			 ++iter) {
			if (iter != (this->m_value.m_array)->begin()) {
				ss << ',';
			}
			ss << iter->str();
		}
		ss << ']';
		break;
	case JSON_OBJECT:
		ss << '{';
		for (auto iter = (this->m_value.m_object)->begin();
			 iter != (this->m_value.m_object)->end();
			 ++iter) {
			if (iter != (this->m_value.m_object)->begin()) {
				ss << ',';
			}
			ss << '\"' << iter->first << '\"' << ':' << iter->second.str();
		}
		ss << '}';
		break;
	}
	return ss.str();
}

void Json::copy(const Json& other) {
	this->m_type = other.m_type;
	switch (this->m_type) {
	case JSON_NULL:
		break;
	case JSON_BOOL:
		this->m_value.m_bool = other.m_value.m_bool;
		break;
	case JSON_INT:
		this->m_value.m_int = other.m_value.m_int;
		break;
	case JSON_DOUBLE:
		this->m_value.m_double = other.m_value.m_double;
		break;
	case JSON_STRING:
		// 浅拷贝
		this->m_value.m_string = other.m_value.m_string;
		break;
	case JSON_ARRAY:
		// 浅拷贝
		this->m_value.m_array = other.m_value.m_array;
		break;
	case JSON_OBJECT:
		// 浅拷贝
		this->m_value.m_object = other.m_value.m_object;
		break;
	default:
		break;
	}
}

Json& Json::operator[](const char* key) {
	std::string name(key);
	return (*(this))[name];
}

Json& Json::operator[](const std::string& key) {
	if (this->m_type != JSON_OBJECT) {
		this->clear();
		this->m_type = JSON_OBJECT;
		this->m_value.m_object = new std::map<std::string, Json>();
	}
	return (*(this->m_value.m_object))[key];
}

void Json::operator=(const Json& other) {
	this->clear();
	this->copy(other);
}

void Json::clear() {
	switch (this->m_type) {
	case JSON_NULL:
		break;
	case JSON_BOOL:
		this->m_value.m_bool = false;
		break;
	case JSON_INT:
		this->m_value.m_int = 0;
		break;
	case JSON_DOUBLE:
		this->m_value.m_double = 0.0;
		break;
	case JSON_STRING: {
		delete (this->m_value).m_string;
		break;
	}
	case JSON_ARRAY: {
		for (auto iter = (this->m_value.m_array)->begin();
			 iter != (this->m_value.m_array)->end();
			 ++iter) {
			iter->clear();
		}
		delete (this->m_value.m_array);
		break;
	}

	case JSON_OBJECT: {
		for (auto iter = (this->m_value.m_object)->begin();
			 iter != (this->m_value.m_object)->end();
			 ++iter) {
			(iter->second).clear();
		}
		delete (this->m_value.m_object);
		break;
	}
	}
	this->m_type = JSON_NULL;
}

bool Json::operator==(const Json& other) {
	if (this->m_type != other.m_type) {
		return false;
	}
	switch (this->m_type) {
	case JSON_NULL:
		return true;
	case JSON_BOOL:
		return this->m_value.m_bool == other.m_value.m_bool;
	case JSON_INT:
		return this->m_value.m_int == other.m_value.m_int;
	case JSON_DOUBLE:
		return this->m_value.m_double == other.m_value.m_double;
	case JSON_STRING:
		return *(this->m_value.m_string) == *(other.m_value.m_string);
	case JSON_ARRAY:
		return this->m_value.m_array == other.m_value.m_array;
	case JSON_OBJECT:
		return this->m_value.m_object == other.m_value.m_object;
	}
	return false;
}

bool Json::operator!=(const Json& other) {
	return !((*this) == other);
}

bool Json::asBool() const {
	if (this->m_type != JSON_BOOL) {
		throw new std::logic_error("type error, not bool value");
	}
	return this->m_value.m_bool;
}

int Json::asInt() const {
	if (this->m_type != JSON_INT) {
		throw new std::logic_error("type error, not int value");
	}
	return this->m_value.m_int;
}

double Json::asDouble() const {
	if (this->m_type != JSON_DOUBLE) {
		throw new std::logic_error("type error, not double value");
	}
	return this->m_value.m_double;
}

std::string Json::asString() const {
	if (this->m_type != JSON_STRING) {
		throw new std::logic_error("type error, not string value");
	}
	return *(this->m_value.m_string);
}

bool Json::has(int index) {
	if (this->m_type != JSON_ARRAY) {
		return false;
	}
	size_t size = (this->m_value.m_array)->size();
	return (index >= 0 && index < size);
}

bool Json::has(const char* key) {
	std::string name(key);
	return has(name);
}

bool Json::has(const std::string& key) {
	if (this->m_type != JSON_OBJECT) {
		return false;
	}
	return ((this->m_value.m_object)->find(key) != (this->m_value.m_object->end()));
}

void Json::remove(int index) {
	if (m_type != JSON_ARRAY) {
		return;
	}
	size_t size = (this->m_value.m_array)->size();
	if (index < 0 || index >= size) {
		return;
	}
	(this->m_value.m_array)->at(index).clear();
	(this->m_value.m_array)->erase((this->m_value.m_array)->begin() + index);
}

void Json::remove(const char* key) {
	std::string name(key);
	return remove(name);
}

void Json::remove(const std::string& key) {
	auto iter = (this->m_value.m_object)->find(key);
	if (iter == (this->m_value.m_object)->end()) {
		return;
	}
	(*(this->m_value.m_object))[key].clear();
	(this->m_value.m_object)->erase(key);
}