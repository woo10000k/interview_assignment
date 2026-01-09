#include "LogFileManager.h"
#include <iostream>

int main() {
    try {
        LogFileManager manager;

        manager.openLogFile("error.log");
        manager.openLogFile("debug.log");
        manager.openLogFile("info.log");

        manager.writeLog("error.log", "Database connection failed");
        manager.writeLog("debug.log", "User login attempt");
        manager.writeLog("info.log", "Server started successfully");

        std::vector<std::string> errorLogs = manager.readLogs("error.log");

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
