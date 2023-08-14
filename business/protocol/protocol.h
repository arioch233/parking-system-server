#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <ctime>
#include "../utility/hash/CRC32.h"

namespace protocol {
	/**
	 * 业务类型枚举类
	 */
	enum class BusinessEnum : uint8_t {
		Heartbeat = 0x00,		// 心跳包
		Login,					// 登录
		Register,				// 注册
		GetVideoList,			// 获取视频列表
		UploadFeatureImage,		// 上传特征图片
		// 更多业务，后期编写根据实际需求增加
	};

	/**
	* 数据包头部
	*/
	struct PacketHeader {
		uint32_t packetLength;        // 包长度
		BusinessEnum packetType;        // 业务类型
		PacketHeader() : packetLength(0), packetType(BusinessEnum::Heartbeat) {
		}
		PacketHeader(uint32_t length, BusinessEnum type) : packetLength(length), packetType(type) {
		}
	};

	/**
	 * JSON数据包
	 */
	constexpr size_t MaxJsonLength = 1024; // 最大允许的 JSON 数据长度

	struct JsonContent {
		uint32_t crc;                       // CRC 校验
		char jsonData[MaxJsonLength + 1];   // JSON 数据，加1留出字符串结尾的空字符
		JsonContent() :crc(0) {
			memset(jsonData, 0, sizeof(jsonData));
		}
		JsonContent(uint32_t check, char* data) {
			memset(jsonData, 0, sizeof(jsonData));
			strncpy(jsonData, data, MaxJsonLength);
		}
	};

	struct JsonPacket {
		PacketHeader header;
		JsonContent content;

		JsonPacket() {
			memset(&header, 0, sizeof(header));
			memset(&content, 0, sizeof(content));
		}
		JsonPacket(BusinessEnum type, const std::string& json) {
			content.crc = CRC32::calculate(json);
			header.packetType = type;
			header.packetLength = sizeof(PacketHeader) + json.length() + sizeof(content.crc);
			content.crc = CRC32::calculate(json);
			memset(content.jsonData, 0, sizeof(content.jsonData));
			strncpy(content.jsonData, json.c_str(), MaxJsonLength);
		}
		JsonPacket(PacketHeader head, JsonContent body) {
			memset(&header, 0, sizeof(header));
			memset(&content, 0, sizeof(content));
			memcpy(&header, &head, sizeof(PacketHeader));
			memcpy(&content, &body, sizeof(JsonContent));
		}
	};

	/**
	 * 文件传输数据包
	 */
	constexpr size_t MaxChunkSize = 1024;  // 定义最大分块大小

	struct FileContent {
		uint64_t fileSize;            // 文件总大小
		uint32_t chunkIndex;            // 当前分块索引
		uint32_t totalChunks;            // 总分块数
		uint32_t checksum;            // CRC32
		uint32_t userid;                // 用户id
		char filename[100];                // 文件名 定长
		char chunkData[MaxChunkSize];    // 分块数据 不定长
		FileContent() : fileSize(0), chunkIndex(0), totalChunks(0), checksum(0), userid(0) {
			memset(filename, 0, sizeof(filename));
			memset(chunkData, 0, sizeof(MaxChunkSize));
		}
		FileContent(uint64_t size, uint32_t index, uint32_t total, const char* data, size_t dataSize, uint32_t userid, const char* file)
			: fileSize(size), chunkIndex(index), totalChunks(total), userid(userid) {
			// 初始化分块数据，将传入的数据拷贝到 chunkData 中
			memset(chunkData, 0, sizeof(chunkData));
			memcpy(chunkData, data, std::min(dataSize, MaxChunkSize));
			// 拷贝文件名
			memset(filename, 0, sizeof(filename));
			strncpy(filename, file, sizeof(filename));
			// CRC32 处理
			checksum = CRC32::calculate(chunkData);
		}
	};

	struct FilePacket {
		PacketHeader header;
		FileContent content;

		FilePacket() {
			memset(&header, 0, sizeof(header));
			memset(&content, 0, sizeof(content));
		}

		// 构造函数，用于初始化文件分块数据包
		FilePacket(BusinessEnum type, uint64_t size, uint32_t index, uint32_t total,
				   const char* data, size_t dataSize, uint32_t userid, const char* file) {
			// 初始化包长度
			header.packetLength = sizeof(PacketHeader) + sizeof(content.fileSize) + sizeof(content.filename) +
				sizeof(content.chunkIndex) + sizeof(content.totalChunks) + sizeof(content.checksum) +
				dataSize + sizeof(userid);
			// 初始化分块数据，将传入的数据拷贝到 chunkData 中
			memset(content.chunkData, 0, sizeof(content.chunkData));
			memcpy(content.chunkData, data, std::min(dataSize, MaxChunkSize));
			// 拷贝文件名
			memset(content.filename, 0, sizeof(content.filename));
			strncpy(content.filename, file, sizeof(content.filename));
			// CRC32 处理
			content.checksum = CRC32::calculate(content.chunkData);
		}
		FilePacket(PacketHeader head, FileContent body) {
			memset(&header, 0, sizeof(header));
			memset(&content, 0, sizeof(content));
			memcpy(&header, &head, sizeof(PacketHeader));
			memcpy(&content, &body, sizeof(FileContent));
		}
	};
}
