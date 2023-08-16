#include "UploadFileHandle.h"

#include <iostream>
#include <cstdint>
#include <algorithm>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <cstring>

#include "../server/System.h"
#include "../utility/hash/CRC32.h"

using namespace task;
using namespace utility;

UploadFileHandler::UploadFileHandler()
{
}

UploadFileHandler::~UploadFileHandler()
{
}

void UploadFileHandler::addFileChunk(int userId, int blockIndex, FileContent& data)
{
	// 自动锁上锁
	AutoLock locker(&(this->mutex));
	// 将数据加入map容器中
	this->fileMap[userId][blockIndex] = data;
}

std::vector<int> UploadFileHandler::verifyAndSortChunks(int& userId, int& fileSize)
{
	// 自动锁上锁
	//AutoLock locker(&(this->mutex));
	std::vector<std::pair<int, FileContent>> pairs;
	// 将哈希映射的键-值对存入向量中
	for (auto& pair : this->fileMap[userId]) {
		pairs.push_back(pair);
	}
	// 按键升序排序
	std::sort(pairs.begin(), pairs.end(), [](const std::pair<int, FileContent>& a, const std::pair<int, FileContent>& b)->bool {
		return a.first < b.first;
		});
	// CRC32校验失败的索引数组
	std::vector<int> failedChecksumIndices;
	// 遍历排序后的键-值对
	for (size_t i = 0; i < pairs.size(); ++i) {
		const FileContent& content = pairs[i].second;
		uint32_t calculatedChecksum = CRC32::calculate(content.chunkData);
		fileSize += content.chunkSize;
		//std::cout << "index = " << content.chunkIndex << " 当前 " << calculatedChecksum << " 原本 " << content.checksum << std::endl;
		// 如果校验失败，记录索引
		if (calculatedChecksum != content.checksum) {
			failedChecksumIndices.push_back(static_cast<int>(i));
		}
	}
	return failedChecksumIndices;
}

int UploadFileHandler::saveFile(int userId, int total, int size, std::vector<int>& indexList, std::string& path)
{
	// 自动锁上锁
	AutoLock locker(&(this->mutex));
	//std::cout << "size = " << this->fileMap[userId].size() << " total = " << total << std::endl;
	int fileSize = 0;
	// 判断当前总块数是否等于总块数
	std::cout << this->fileMap[userId].size() << std::endl;
	if (this->fileMap[userId].size() == total)
	{
		indexList = this->verifyAndSortChunks(userId, fileSize);
		if (indexList.empty() && indexList.size() == 0 && fileSize == size)
		{
			// 文件校验完成，保存文件
			path = this->saveChunkToFile(userId);
			return 1;
		}
		return 2;
	}
	return 3;
}

void UploadFileHandler::clear()
{
	// 自动锁上锁
	AutoLock locker(&(this->mutex));
}

std::string UploadFileHandler::saveChunkToFile(int& userId)
{
	std::string uploadPath = "";
	// 判断文件上传是否存在
	this->makeDir("./upload");
	// 判断用户文件夹是否存在
	std::string filePath = "./upload/" + std::to_string(userId) + '/';
	std::cout << filePath << std::endl;
	this->makeDir(filePath);
	// 遍历 map 容器保存文件
	// 自动锁上锁
	//AutoLock locker(&(this->mutex));
	std::vector<std::pair<int, FileContent>> pairs;
	for (auto& pair : this->fileMap[userId]) {
		pairs.push_back(pair);
	}
	const FileContent& content = pairs[0].second;
	std::string fileName = std::string(content.filename);
	size_t lastSlashPos = fileName.find_last_of('/');
	// 提取最后一个斜杠后的子字符串
	std::string result = fileName.substr(lastSlashPos + 1);
	filePath += result;
	int fileDescriptor = open(filePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

	// 遍历排序后的键-值对
	std::cout << "当前缓存块数: " << this->fileMap[userId].size() << std::endl;
	for (size_t i = 0; i < pairs.size(); ++i) {
		const FileContent& content = pairs[i].second;
		ssize_t bytesWritten = write(fileDescriptor, content.chunkData, content.chunkSize);
		std::cout << "成功写入 " << bytesWritten << " 字节到文件中。\n";
	}
	// 删除缓存
	this->fileMap[userId].clear();
	std::cout << "当前缓存块数: " << this->fileMap[userId].size() << std::endl;
	// 关闭文件描述符
	close(fileDescriptor);
	uploadPath = filePath;
	return uploadPath;
}

void UploadFileHandler::makeDir(std::string path)
{
	// 尝试打开文件夹
	DIR* dir = opendir(path.c_str());
	mode_t dirMode = 0666;
	if (dir == nullptr) {
		if (mkdir(path.c_str(), dirMode) == 0) {
			printf("directory created.\n");
			// 重新尝试打开文件夹
			dir = opendir(path.c_str());
		}
		else {
			perror("error creating directory");
		}
	}

	if (dir) {
		printf("directory opened successfully.\n");
		closedir(dir); // 关闭打开的文件夹
	}
	else {
		perror("error opening directory");
	}
}