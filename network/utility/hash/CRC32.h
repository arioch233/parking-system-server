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

// 将CRC32查找表初始化为零。
uint32_t CRC32::crcTable[256] = { 0 };

// 生成CRC32查找表。
void CRC32::generateTable() {
	for (int i = 0; i < 256; ++i) {
		uint32_t crc = i;
		for (int j = 0; j < 8; ++j) {
			if (crc & 1) {
				crc = (crc >> 1) ^ 0xEDB88320;
			}
			else {
				crc >>= 1;
			}
		}
		crcTable[i] = crc;
	}
}

// 计算给定数据的CRC32校验和。
uint32_t CRC32::calculate(const void* data, size_t numBytes) {
	// 如果尚未生成CRC32查找表，则进行初始化。
	if (crcTable[1] == 0) {
		generateTable();
	}

	// 将数据指针转换为uint8_t类型，以便更容易处理。
	auto bytes = static_cast<const uint8_t*>(data);

	// 将CRC值初始化为0xFFFFFFFF（初始值）。
	uint32_t crc = 0xFFFFFFFF;

	// 使用查找表计算CRC32校验和。
	for (size_t i = 0; i < numBytes; ++i) {
		uint8_t index = (crc ^ bytes[i]) & 0xFF;
		crc = (crc >> 8) ^ crcTable[index];
	}

	// 通过与0xFFFFFFFF异或来完成CRC值的最终处理。
	return crc ^ 0xFFFFFFFF;
}

// 计算给定字符串的CRC32校验和。
uint32_t CRC32::calculate(const std::string& text) {
	// 使用字符串数据调用数据计算函数。
	return calculate(text.c_str(), text.size());
}