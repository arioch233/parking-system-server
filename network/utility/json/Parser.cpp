#include "Parser.h"
#include <stdexcept>

using namespace json;

Parser::Parser() : m_str(""), m_index(0) {
}

Parser::Parser(std::string str) : m_str(str), m_index(0) {
}

Parser::~Parser() {
}

void Parser::load(const std::string& str) {
	this->m_str = str;
	this->m_index = 0;
}

Json Parser::parse() {
	char ch = this->getNextToken();

	if (ch == 'n') {
		--(this->m_index);
		return this->parseNull();
	}

	if (ch == 't' || ch == 'f') {
		--(this->m_index);
		return this->parseBool();
	}

	if (ch == '-' || (ch >= '0' && ch <= '9')) {
		--(this->m_index);
		return this->parseNumber();
	}

	if (ch == '"') {
		return this->parseString();
	}

	if (ch == '[') {
		Json arr(Json::JSON_ARRAY);
		ch = this->getNextToken();
		if (ch == ']') {
			return arr;
		}
		while (true) {
			--(this->m_index);
			arr.append(parse());
			ch = this->getNextToken();
			if (ch == ']') {
				break;
			}
			if (ch != ',') {
				throw std::logic_error("expected ',' in array");
			}
			ch = this->getNextToken();
		}
		return arr;
	}

	if (ch == '{') {
		Json obj(Json::JSON_OBJECT);
		ch = this->getNextToken();
		if (ch == '}') {
			return obj;
		}

		while (true) {
			if (ch != '"') {
				throw std::logic_error("expected '\"' in object");
			}
			std::string key = this->parseString();

			ch = this->getNextToken();
			if (ch != ':') {
				throw std::logic_error("expected ':' in object");
			}

			obj[key] = parse();

			ch = this->getNextToken();
			if (ch == '}') {
				break;
			}
			if (ch != ',') {
				throw std::logic_error("expected ',' in object");
			}
			ch = this->getNextToken();
		}
		return obj;
	}

	throw std::logic_error("unexpected character in parse json");
}

void Parser::skipWhiteSpaces() {
	while (m_str[this->m_index] == ' ' ||
		   m_str[this->m_index] == '\r' ||
		   m_str[this->m_index] == '\n' ||
		   m_str[this->m_index] == '\t') {
		++(this->m_index);
	}
}

char Parser::getNextToken() {
	this->skipWhiteSpaces();
	if (this->m_index == this->m_str.size())
		throw std::logic_error("unexpected end of input");
	return this->m_str[(this->m_index)++];
}

Json Parser::parseNull() {
	if (this->m_str.compare(this->m_index, 4, "null") == 0) {
		(this->m_index) += 4;
		return Json();
	}
	throw std::logic_error("parse null error");
}

Json Parser::parseBool() {
	if (this->m_str.compare(this->m_index, 4, "true") == 0) {
		(this->m_index) += 4;
		return Json(true);
	}
	if (this->m_str.compare(this->m_index, 5, "false") == 0) {
		(this->m_index) += 5;
		return Json(false);
	}
	throw std::logic_error("parse bool error");
}

Json Parser::parseNumber() {
	size_t pos = this->m_index;

	if (this->m_str[this->m_index] == '-')
		this->m_index++;

	// integer part
	if (this->m_str[this->m_index] == '0') {
		this->m_index++;
		if (this->inRange(this->m_str[this->m_index], '0', '9')) {
			throw std::logic_error("leading 0s not permitted in numbers");
		}
	}
	else if (this->inRange(this->m_str[this->m_index], '1', '9')) {
		this->m_index++;
		while (this->inRange(this->m_str[this->m_index], '0', '9')) {
			this->m_index++;
		}
	}
	else {
		throw std::logic_error("invalid character in number");
	}

	if (this->m_str[this->m_index] != '.') {
		return std::atoi(this->m_str.c_str() + pos);
	}

	// decimal part
	if (this->m_str[this->m_index] == '.') {
		this->m_index++;
		if (!this->inRange(this->m_str[this->m_index], '0', '9')) {
			throw std::logic_error("at least one digit required in fractional part");
		}
		while (this->inRange(this->m_str[this->m_index], '0', '9')) {
			this->m_index++;
		}
	}

	return std::atof(this->m_str.c_str() + pos);
}

Json Parser::parseString() {
	std::string out;
	while (true) {
		if (this->m_index == m_str.size()) {
			throw std::logic_error("unexpected end of input in string");
		}

		char ch = m_str[(this->m_index)++];
		if (ch == '"') {
			return out;
		}

		// The usual case: non-escaped characters
		if (ch != '\\') {
			out += ch;
			continue;
		}

		// Handle escapes
		throw std::logic_error("not support escaped characters in string");
	}
}

bool Parser::inRange(long x, long lower, long upper) {
	return (x >= lower && x <= upper);
}