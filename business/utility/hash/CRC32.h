#pragma once
#include <cstdint>
#include <string>

class CRC32 {
public:
	// 计算给定数据的CRC32校验和。
	// 返回CRC32校验和值。
	static uint32_t calculate(const void* data, size_t numBytes);

	// 计算给定字符串的CRC32校验和。
	// 返回CRC32校验和值。
	static uint32_t calculate(const std::string& text);

private:
	// 生成CRC32查找表。
	static void generateTable();

	// 预计算的CRC32查找表。
	static uint32_t crcTable[256];
};
