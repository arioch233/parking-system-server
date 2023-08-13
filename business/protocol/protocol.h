#pragma once
#include <cstdint>
#include <cstring>
#include <string>
#include <functional>
#include <ctime>

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
	 * JSON数据包
	 */
	constexpr size_t MaxJsonLength = 1024; // 最大允许的 JSON 数据长度
	struct JsonPacket {
		uint32_t packetLength;              // 数据包总长度
		BusinessEnum packetType;            // 数据包类型
		char jsonData[MaxJsonLength + 1];   // JSON 数据，加1留出字符串结尾的空字符

		JsonPacket() : packetLength(0), packetType(BusinessEnum::Heartbeat) {
			memset(jsonData, 0, sizeof(jsonData));
		}
		JsonPacket(BusinessEnum type, const std::string& json) : packetType(type) {
			packetLength = sizeof(packetLength) + sizeof(packetType) + json.length();
			memset(jsonData, 0, sizeof(jsonData));
			strncpy(jsonData, json.c_str(), MaxJsonLength);
		}
	};

	/**
	 * 文件传输数据包
	 */
	constexpr size_t MaxChunkSize = 1024;  // 定义最大分块大小
	struct FilePacket {
		uint32_t packetLength;  		// 包长度
		BusinessEnum packetType;  		// 业务类型
		uint64_t fileSize;  			// 文件总大小
		uint32_t chunkIndex;  			// 当前分块索引
		uint32_t totalChunks;  			// 总分块数
		uint32_t checksum;  			// 数据校验和
		char chunkData[MaxChunkSize];  	// 分块数据

		// 构造函数，用于初始化文件分块数据包
		FilePacket(BusinessEnum type, uint64_t size, uint32_t index, uint32_t total, const char* data, size_t dataSize)
			: packetType(type), fileSize(size), chunkIndex(index), totalChunks(total) {
			packetLength = sizeof(packetLength) + sizeof(packetType) + sizeof(fileSize) +
				sizeof(chunkIndex) + sizeof(totalChunks) + sizeof(checksum) + dataSize;

			checksum = calculateChecksum(data, dataSize);

			// 初始化分块数据，将传入的数据拷贝到 chunkData 中
			memset(chunkData, 0, sizeof(chunkData));
			memcpy(chunkData, data, std::min(dataSize, MaxChunkSize));
		}

		// 计算数据的哈希校验和，这里使用了 std::hash<std::string> 作为哈希函数
		static uint32_t calculateChecksum(const char* data, size_t dataSize) {
			return std::hash<std::string>{}(std::string(data, dataSize));
		}

		// 验证数据的校验和是否有效
		bool isChecksumValid() const {
			uint32_t calculatedChecksum = calculateChecksum(chunkData, sizeof(chunkData));
			return checksum == calculatedChecksum;
		}
	};

	/**
	 * 心跳数据包
	 */
	struct HeartbeatPacket {
		BusinessEnum type;        // 心跳包类型，例如 0x01 表示心跳包
		uint64_t timestamp;       // 时间戳，记录发送时间
		uint32_t sequenceNumber;  // 序列号，用于标识不同的心跳包
		char data[256];           // 数据字段，用于传输额外信息

		// 构造函数，初始化默认值
		HeartbeatPacket() : type(BusinessEnum::Heartbeat), timestamp(0), sequenceNumber(0) {
			memset(data, 0, sizeof(data));
		}

		// 设置当前时间戳
		void setCurrentTimestamp() {
			timestamp = static_cast<uint64_t>(std::time(nullptr));
		}
	};
}