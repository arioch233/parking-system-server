#include "WriteTask.h"

#include <unistd.h>
#include <cerrno>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "UploadFileHandle.h"
#include "../protocol/protocol.h"
#include "../utility/logger/Logger.h"
#include "../utility/Singleton.h"
#include "../utility/json/Parser.h"
#include "../utility/json/Json.h"
#include "../share/SharedMemoryFIFO.h"
#include "../domain/controller/UserController.h"
#include "../domain/controller/ParkingController.h"
#include "../domain/controller/SnapshotController.h"

using namespace task;
using namespace utility;
using namespace json;
using namespace protocol;
using namespace shm;

WriteTask::WriteTask(MemoryBlock* block) :BaseTask(block)
{
}

WriteTask::~WriteTask() {
}

void WriteTask::run()
{
	debug("write task run");
	// 获取数据
	MemoryBlock* block = static_cast<MemoryBlock*>(this->data);
	// 解析头数据
	PacketHeader packetHeader;
	memcpy(&packetHeader, block->data, sizeof(PacketHeader));
	// 业务处理
	switch (packetHeader.packetType)
	{
	case BusinessEnum::UploadEntryFeatureImage:
	case BusinessEnum::UploadExitFeatureImage:
	case BusinessEnum::UploadFeatureImage:
	{
		FileContent fileReq;
		memcpy(&fileReq, block->data + sizeof(PacketHeader), sizeof(FileContent));
		info("filename = %s, index = %d, total = %d, size = %d", fileReq.filename, fileReq.chunkIndex, fileReq.totalChunks, fileReq.fileSize);
		// 加入文件块管理类
		std::vector<int> indexList; // 待重发的索引列表
		std::string path = ""; // 上传成功路径
		UploadFileHandler* uploadHander = Singleton<UploadFileHandler>::getInstance();
		uploadHander->addFileChunk(fileReq.userid, fileReq.chunkIndex, fileReq);
		// 初始化Json对象
		Json result;
		int res = uploadHander->saveFile(fileReq.userid, fileReq.totalChunks, fileReq.fileSize, indexList, path);
		if (res == 1)
		{
			result["flag"] = true;
			result["msg"] = "图片 " + std::string(fileReq.filename) + "上传成功";
			result["data"] = path;
		}
		else if (res == 2)
		{
			result["flag"] = false;
			result["msg"] = "图片 " + std::string(fileReq.filename) + "上传失败";
			Json arr;
			for (int i = 0; i < indexList.size(); ++i)
			{
				arr.append(indexList[i]);
			}
			result["data"] = arr;
		}
		else
		{
			break;
		}

		// 写入共享内存
		JsonPacket resultPacket(packetHeader.packetType, result.str());
		MemoryBlock data(block->socketfd, &resultPacket, resultPacket.header.packetLength);
		Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
		break;
	}
	case BusinessEnum::Login:
	{
		JsonContent loginReq;
		memcpy(&loginReq, block->data + sizeof(PacketHeader), sizeof(JsonContent));
		info("json data = ", loginReq.jsonData);
		Parser p;
		p.load(loginReq.jsonData);
		Json obj = p.parse();
		UserController userController;
		std::string jsonStr = userController.userLogin(obj["username"], obj["password"]);
		// 写入共享内存
		JsonPacket resultPacket(BusinessEnum::Login, jsonStr);
		MemoryBlock data(block->socketfd, &resultPacket, resultPacket.header.packetLength);
		Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
		break;
	}
	case BusinessEnum::Register:
	{
		JsonContent registerReq;
		memcpy(&registerReq, block->data + sizeof(PacketHeader), sizeof(JsonContent));
		info("json data = ", registerReq.jsonData);
		Parser p;
		p.load(registerReq.jsonData);
		Json obj = p.parse();
		UserController userController;
		std::string jsonStr = userController.userRegister(obj["username"], obj["password"], obj["nickname"]);
		// 写入共享内存
		JsonPacket resultPacket(BusinessEnum::Register, jsonStr);
		MemoryBlock data(block->socketfd, &resultPacket, resultPacket.header.packetLength);
		Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
		break;
	}
	case BusinessEnum::GetParkingInfo:
	{
		JsonContent registerReq;
		memcpy(&registerReq, block->data + sizeof(PacketHeader), sizeof(JsonContent));
		info("json data = ", registerReq.jsonData);
		Parser p;
		p.load(registerReq.jsonData);
		Json obj = p.parse();
		ParkingController parkingController;
		std::string jsonStr = parkingController.getParkingInfo();
		// 写入共享内存
		JsonPacket resultPacket(BusinessEnum::GetParkingInfo, jsonStr);
		MemoryBlock data(block->socketfd, &resultPacket, resultPacket.header.packetLength);
		Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
		break;
	}
	// 上传出场信息
	case BusinessEnum::UploadExitInfo:
	{
		JsonContent req;
		memcpy(&req, block->data + sizeof(PacketHeader), sizeof(JsonContent));
		info("json data = ", req.jsonData);
		Parser p;
		p.load(req.jsonData);
		Json obj = p.parse();
		ParkingController parkingController;
		std::string jsonStr = parkingController.exitCar(obj["user_id"], obj["car_number"], obj["exit_url"], obj["exit_address"], obj["exit_time"], obj["parking_fee"]);
		// 写入共享内存
		JsonPacket resultPacket(BusinessEnum::UploadExitInfo, jsonStr);
		MemoryBlock data(block->socketfd, &resultPacket, resultPacket.header.packetLength);
		Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
		break;
	}
	// 上传入场信息
	case BusinessEnum::UploadEntryInfo:
	{
		JsonContent req;
		memcpy(&req, block->data + sizeof(PacketHeader), sizeof(JsonContent));
		info("json data = ", req.jsonData);
		Parser p;
		p.load(req.jsonData);
		Json obj = p.parse();
		ParkingController parkingController;
		std::string jsonStr = parkingController.entryCar(obj["user_id"], obj["car_number"], obj["entry_url"], obj["entry_address"], obj["entry_time"]);
		// 写入共享内存
		JsonPacket resultPacket(BusinessEnum::UploadEntryInfo, jsonStr);
		MemoryBlock data(block->socketfd, &resultPacket, resultPacket.header.packetLength);
		Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
		break;
	}
	// 获取停车记录信息
	case BusinessEnum::GetParkingRecords:
	{
		JsonContent req;
		memcpy(&req, block->data + sizeof(PacketHeader), sizeof(JsonContent));
		info("json data = ", req.jsonData);
		Parser p;
		p.load(req.jsonData);
		Json obj = p.parse();
		ParkingController parkingController;
		std::string jsonStr = parkingController.getParkingRecords(obj["currentPage"], obj["pageSize"], obj["carNumber"], obj["startTime"], obj["endTime"]);
		// 写入共享内存
		JsonPacket resultPacket(BusinessEnum::GetParkingRecords, jsonStr);
		MemoryBlock data(block->socketfd, &resultPacket, resultPacket.header.packetLength);
		Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
		break;
	}
	// 获取入场记录信息
	case BusinessEnum::GetEntryRescords:
	{
		JsonContent req;
		memcpy(&req, block->data + sizeof(PacketHeader), sizeof(JsonContent));
		info("json data = ", req.jsonData);
		Parser p;
		p.load(req.jsonData);
		Json obj = p.parse();
		ParkingController parkingController;
		std::string jsonStr = parkingController.getEntryCarRecords();
		// 写入共享内存
		JsonPacket resultPacket(BusinessEnum::GetEntryRescords, jsonStr);
		MemoryBlock data(block->socketfd, &resultPacket, resultPacket.header.packetLength);
		Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
		break;
	}
	// 根据车牌号获取入场信息
	case BusinessEnum::ExGetEntryRecord:
	{
		JsonContent req;
		memcpy(&req, block->data + sizeof(PacketHeader), sizeof(JsonContent));
		info("json data = ", req.jsonData);
		Parser p;
		p.load(req.jsonData);
		Json obj = p.parse();
		ParkingController parkingController;
		std::string jsonStr = parkingController.getCarEntryInfo(obj["car_number"]);
		// 写入共享内存
		JsonPacket resultPacket(BusinessEnum::ExGetEntryRecord, jsonStr);
		MemoryBlock data(block->socketfd, &resultPacket, resultPacket.header.packetLength);
		Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
		break;
	}
	// 上传录制信息
	case BusinessEnum::UploadVideoInfo:
	{
		JsonContent req;
		memcpy(&req, block->data + sizeof(PacketHeader), sizeof(JsonContent));
		info("json data = ", req.jsonData);
		Parser p;
		p.load(req.jsonData);
		Json obj = p.parse();
		SnapshotController snapshotController;
		std::string jsonStr = snapshotController.addSnapshotRecord(obj["userId"], obj["captureTime"], obj["desc"],
			obj["filename"], obj["cameraName"], obj["location"], obj["videoUrl"], obj["coverUrl"]);
		// 写入共享内存
		JsonPacket resultPacket(BusinessEnum::UploadVideoInfo, jsonStr);
		MemoryBlock data(block->socketfd, &resultPacket, resultPacket.header.packetLength);
		Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
		break;
	}
	// 日
	case BusinessEnum::GetDaysMenuList:
	{
		JsonContent req;
		memcpy(&req, block->data + sizeof(PacketHeader), sizeof(JsonContent));
		info("json data = ", req.jsonData);
		Parser p;
		p.load(req.jsonData);
		Json obj = p.parse();
		SnapshotController snapshotController;
		std::string jsonStr = snapshotController.getMenuOfDay();
		// 写入共享内存
		JsonPacket resultPacket(BusinessEnum::GetDaysMenuList, jsonStr);
		MemoryBlock data(block->socketfd, &resultPacket, resultPacket.header.packetLength);
		Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
		break;
	}
	// 月
	case BusinessEnum::GetMouthsMenuList:
	{
		JsonContent req;
		memcpy(&req, block->data + sizeof(PacketHeader), sizeof(JsonContent));
		info("json data = ", req.jsonData);
		Parser p;
		p.load(req.jsonData);
		Json obj = p.parse();
		SnapshotController snapshotController;
		std::string jsonStr = snapshotController.getMenuOfMouth();
		// 写入共享内存
		JsonPacket resultPacket(BusinessEnum::GetMouthsMenuList, jsonStr);
		MemoryBlock data(block->socketfd, &resultPacket, resultPacket.header.packetLength);
		Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
		break;
	}
	// 列表
	case BusinessEnum::GetVideoList:
	{
		JsonContent req;
		memcpy(&req, block->data + sizeof(PacketHeader), sizeof(JsonContent));
		info("json data = ", req.jsonData);
		Parser p;
		p.load(req.jsonData);
		Json obj = p.parse();
		SnapshotController snapshotController;
		std::string jsonStr = snapshotController.getSnapShotRecordList(obj["currentPage"], obj["pageSize"], obj["dateTime"]);
		// 写入共享内存
		JsonPacket resultPacket(BusinessEnum::GetVideoList, jsonStr);
		MemoryBlock data(block->socketfd, &resultPacket, resultPacket.header.packetLength);
		Singleton<SharedMemoryFIFO>::getInstance()->write(&data);
		break;
	}
	default:
		error("unknown service");
		break;
	}
}

void WriteTask::destroy()
{
	debug("write task destory");
	delete this;
}