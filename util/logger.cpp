#include "logger.h"
#include <cstdarg>
#include <vector>
#include <filesystem>

Logger::Logger() : currentLevel(DEBUG) {
    std::filesystem::create_directories("../logs");
    logFile.open("../logs/logs.txt", std::ios::app);
    if (!logFile) {
        std::cerr << "Error: Could not open log file!" << std::endl;
    }
    logFile.write("--------------------------------------------------\n", 51);
}

Logger::Logger(LogLevel level) : currentLevel(level) {
    std::filesystem::create_directories("../logs");
    logFile.open("../logs/logs.txt", std::ios::app);
    if (!logFile) {
        std::cerr << "Error: Could not open log file!" << std::endl;
    }
    logFile.write("--------------------------------------------------\n", 51);
}

Logger::~Logger() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::setLogLevel(LogLevel level) {
    currentLevel = level;
}

void Logger::log(LogLevel level, const char* format, ...) {
    if (level < currentLevel) return;

    std::lock_guard<std::mutex> lock(logMutex);

    va_list args;
    va_start(args, format);
    std::vector<char> buffer(1024);
    vsnprintf(buffer.data(), buffer.size(), format, args);
    va_end(args);

    std::string logMessage = getTimeStamp() + " [" + logLevelToString(level) + "] " + std::string(buffer.data());

    std::cout << logMessage << std::endl;

    if (logFile.is_open()) {
        logFile << logMessage << std::endl;
    }
}

std::string Logger::getTimeStamp() {
    std::time_t now = std::time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

std::string Logger::logLevelToString(LogLevel level) {
    switch (level) {
    case DEBUG: return "DEBUG";
    case INFO:  return "INFO";
    case WARN:  return "WARN";
    case ERROR: return "ERROR";
    case FATAL: return "FATAL";
    default:    return "UNKNOWN";
    }
}