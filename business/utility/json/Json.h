#pragma once

#include <string>
#include <vector>
#include <map>

namespace json {
	class Json {
	public:
		// json 类型
		enum Type {
			JSON_NULL = 0,
			JSON_BOOL,
			JSON_INT,
			JSON_DOUBLE,
			JSON_STRING,
			JSON_ARRAY,
			JSON_OBJECT
		};

		Json();

		Json(bool value);

		Json(int value);

		Json(double value);

		Json(const char* value);

		Json(const std::string& value);

		Json(Type type);

		Json(const Json& other);

		// 基本类型运算符重载
		operator bool();

		operator int();

		operator double();

		operator std::string();

		// 数组重载
		Json& operator[](int index);

		// 对象运算符重载
		Json& operator[](const char* key);

		Json& operator[](const std::string& key);

		void operator=(const Json& other);

		// 比较运算符重载
		bool operator==(const Json& other);

		bool operator!=(const Json& other);

		// 数据添加
		void append(const Json& other);

		// 格式化输出
		std::string str() const;

		// 释放空间
		void clear();

		// JSON 数组迭代器
		typedef std::vector<Json>::iterator iterator;

		iterator begin() {
			return this->m_value.m_array->begin();
		}

		iterator end() {
			return this->m_value.m_array->end();
		}

		// 类型判断
		bool isNull() const { return this->m_type == JSON_NULL; }

		bool isBool() const { return this->m_type == JSON_BOOL; }

		bool isInt() const { return this->m_type == JSON_INT; }

		bool isDouble() const { return this->m_type == JSON_DOUBLE; }

		bool isString() const { return this->m_type == JSON_STRING; }

		bool isArray() const { return this->m_type == JSON_ARRAY; }

		bool isObject() const { return this->m_type == JSON_OBJECT; }

		// 基本类型判断
		bool asBool() const;

		int asInt() const;

		double asDouble() const;

		std::string asString() const;

		// 判断类型释放含有索引
		bool has(int index);

		bool has(const char* key);

		bool has(const std::string& key);

		// 擦除元素
		void remove(int index);

		void remove(const char* key);

		void remove(const std::string& key);

	private:
		void copy(const Json& other);

	private:
		// json 值
		union Value {
			bool m_bool;
			int m_int;
			double m_double;
			std::string* m_string;
			std::vector<Json>* m_array;
			std::map<std::string, Json>* m_object;
		};

		Type m_type;
		Value m_value;
	};
} // json