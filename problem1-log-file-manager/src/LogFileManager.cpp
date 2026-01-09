#include "LogFileManager.h"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>

LogFileManager::~LogFileManager() {
    for (auto& [name, file] : files_) {
        if (file && file->is_open()) {
            file->close();
        }
    }
}

void LogFileManager::openLogFile(const std::string& filename) {
    if (filename.empty()) {
        throw std::invalid_argument("Filename cannot be empty");
    }

    if (files_.find(filename) != files_.end()) {
        return;
    }

    std::string fullPath = getLogFilePath(filename);
    auto file = std::make_unique<std::fstream>();
    file->open(fullPath, std::ios::in | std::ios::out | std::ios::app);

    if (!file->is_open()) {
        throw std::runtime_error("Failed to open file: " + fullPath);
    }

    files_[filename] = std::move(file);
}

void LogFileManager::writeLog(const std::string& filename, const std::string& message) {
    if (filename.empty()) {
        throw std::invalid_argument("Filename cannot be empty");
    }
    if (message.empty()) {
        throw std::invalid_argument("Message cannot be empty");
    }

    auto it = files_.find(filename);
    if (it == files_.end()) {
        throw std::runtime_error("File not opened: " + filename);
    }

    auto& file = it->second;
    if (!file || !file->is_open()) {
        throw std::runtime_error("File is not open: " + filename);
    }

    std::string timestamp = getCurrentTimestamp();
    *file << timestamp << " " << message << std::endl;

    if (file->fail()) {
        throw std::runtime_error("Failed to write to file: " + filename);
    }
}

std::vector<std::string> LogFileManager::readLogs(const std::string& filename) {
    if (filename.empty()) {
        throw std::invalid_argument("Filename cannot be empty");
    }

    auto it = files_.find(filename);
    if (it == files_.end()) {
        throw std::runtime_error("File not opened: " + filename);
    }

    auto& file = it->second;
    if (!file || !file->is_open()) {
        throw std::runtime_error("File is not open: " + filename);
    }

    std::vector<std::string> logs;
    file->clear();
    file->seekg(0);

    std::string line;
    while (std::getline(*file, line)) {
        logs.push_back(line);
    }

    file->clear();
    file->seekp(0, std::ios::end);

    return logs;
}

void LogFileManager::closeLogFile(const std::string& filename) {
    if (filename.empty()) {
        throw std::invalid_argument("Filename cannot be empty");
    }

    auto it = files_.find(filename);
    if (it == files_.end()) {
        throw std::runtime_error("File not opened: " + filename);
    }

    auto& file = it->second;
    if (file && file->is_open()) {
        file->close();
    }

    files_.erase(it);
}

std::string LogFileManager::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") << "]";
    return ss.str();
}

std::string LogFileManager::getLogFilePath(const std::string& filename) const {
    return "logs/" + filename;
}
