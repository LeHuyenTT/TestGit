#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

enum class LogLevel { DEBUG, INFO, WARNING, ERROR };

class Logger {
public:
    static Logger &getInstance(const std::string &filePath = "app.log") {
        static Logger instance(filePath);
        return instance;
    }

    Logger(Logger const &) = delete;
    void operator=(Logger const &) = delete;

    void log(LogLevel level, const std::string &functionName, int lineNumber,
             const std::string &message) {
        std::stringstream ss;
        ss << "[" << getTimeString() << "] ";
        switch (level) {
            case LogLevel::DEBUG:
                ss << "[DEBUG] ";
                break;
            case LogLevel::INFO:
                ss << "[INFO] ";
                break;
            case LogLevel::WARNING:
                ss << "[WARNING] ";
                break;
            case LogLevel::ERROR:
                ss << "[ERROR] ";
                break;
        }
        ss << "Function: " << functionName << "(), Line: " << lineNumber << ", Message: " << message
           << std::endl;
        std::cout << ss.str();
        if (logFile.good()) {
            logFile << ss.str();
            logFile.flush();
        }
    }

private:
    std::ofstream logFile;

    Logger(const std::string &filePath) : logFile(filePath) {}

    std::string getTimeString() {
        auto              now    = std::chrono::system_clock::now();
        auto              time_t = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
};

// Macro to simplify logging
#define LOG(level, message) Logger::getInstance().log(level, __FUNCTION__, __LINE__, message)

#endif // LOGGER_H
