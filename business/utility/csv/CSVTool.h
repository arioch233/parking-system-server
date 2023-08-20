#pragma once

/************************************************************************/
/*  CSV 工具类                                                          */
/************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

namespace utility {
	class CSVTool {
	public:
		CSVTool(const std::string& filename, const std::vector<std::string>& headers)
			: filename(filename), headers(headers) {}

		bool readCSV(std::vector<std::vector<std::string>>& data) {
			std::ifstream file(filename);
			if (!file.is_open()) {
				std::cout << "File does not exist. Creating " << filename << std::endl;
				return false;
			}

			std::string line;
			while (std::getline(file, line)) {
				std::vector<std::string> row;
				std::istringstream lineStream(line);
				std::string cell;
				while (std::getline(lineStream, cell, ',')) {
					row.push_back(cell);
				}
				data.push_back(row);
			}

			file.close();
			return true;
		}

		bool writeCSV(const std::vector<std::vector<std::string>>& data) {
			std::ofstream file(filename);
			if (!file.is_open()) {
				std::cout << "Error creating or opening file: " << filename << std::endl;
				return false;
			}

			// Write headers
			for (size_t i = 0; i < headers.size(); ++i) {
				file << headers[i];
				if (i < headers.size() - 1) {
					file << ",";
				}
			}
			file << std::endl;
			// Write data 无法处理JSON， 使用"包括JSON字符串"
			/*for (const auto& row : data) {
				for (size_t i = 0; i < row.size(); ++i) {
					file << row[i];
					if (i < row.size() - 1) {
						file << ",";
					}
				}
				file << std::endl;
			}*/
			// Write data 无法处理  {"password":"3149592fb961d03e16505acf268ee648","username":"admin123"} 加入转义字符
			/*for (const auto& row : data) {
				for (size_t i = 0; i < row.size(); ++i) {
					const std::string& field = row[i];
					bool needsQuoting = field.find(',') != std::string::npos || field.find('"') != std::string::npos;

					if (needsQuoting) {
						file << '"' << field << '"';
					}
					else {
						file << field;
					}

					if (i < row.size() - 1) {
						file << ",";
					}
				}
				file << std::endl;
			}*/
			for (const auto& row : data) {
				for (size_t i = 0; i < row.size(); ++i) {
					const std::string& field = row[i];
					bool needsQuoting = field.find(',') != std::string::npos || field.find('"') != std::string::npos;

					if (needsQuoting) {
						file << '"';
						for (char c : field) {
							if (c == '"') {
								file << "\"\"";
							}
							else {
								file << c;
							}
						}
						file << '"';
					}
					else {
						file << field;
					}

					if (i < row.size() - 1) {
						file << ",";
					}
				}
				file << std::endl;
			}

			file.close();
			return true;
		}

	private:
		std::string filename;
		std::vector<std::string> headers;
	};
}