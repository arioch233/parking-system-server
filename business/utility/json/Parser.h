#pragma once
#include <string>

#include "Json.h"

namespace json {
	class Parser {
	public:
		Parser();

		Parser(std::string  str);

		virtual ~Parser();

		// 加载JSON字符串
		void load(const std::string& str);

		// 解析返回一个JSON对象
		Json parse();

	private:
		void skipWhiteSpaces();

		char getNextToken();

		Json parseNull();

		Json parseBool();

		Json parseNumber();

		Json parseString();

		bool inRange(long x, long lower, long upper);

	private:
		std::string m_str;
		size_t m_index;
	};
} // json
