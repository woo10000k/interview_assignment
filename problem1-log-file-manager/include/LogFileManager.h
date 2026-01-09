#ifndef LOG_FILE_MANAGER_H
#define LOG_FILE_MANAGER_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <fstream>

class LogFileManager {
public:
    LogFileManager() = default;
    ~LogFileManager();

    LogFileManager(const LogFileManager&) = delete;
    LogFileManager& operator=(const LogFileManager&) = delete;
    LogFileManager(LogFileManager&&) noexcept = default;
    LogFileManager& operator=(LogFileManager&&) noexcept = default;

    void openLogFile(const std::string& filename);
    void writeLog(const std::string& filename, const std::string& message);
    std::vector<std::string> readLogs(const std::string& filename);
    void closeLogFile(const std::string& filename);

private:
    std::map<std::string, std::unique_ptr<std::fstream>> files_;

    std::string getCurrentTimestamp() const;
    std::string getLogFilePath(const std::string& filename) const;
};

#endif
