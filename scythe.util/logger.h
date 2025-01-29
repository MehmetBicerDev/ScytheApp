#pragma once
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <sstream>
#include <windows.h>
#include <mutex>
#include <format>
#include <string>

class logger {
public:
    enum class LogLevel {
        INFO,
        WARNING,
        ZERROR
    };

    // Static log function for variadic template (formatting with parameters)
    template<typename... Args>
    static void log(LogLevel level, const std::string& format, Args&&... args) {
        std::lock_guard<std::mutex> guard(mutex_);  // Ensure thread safety

        std::string logLevelStr = getLogLevelString(level);
        std::string timestamp = getCurrentTime();
        std::string formattedMessage = std::vformat(format, std::make_format_args(args...));

        // Set colors and log
        setColor(GREY); std::cout << "["; // Opening bracket
        setColor(LIGHT_CYAN); std::cout << timestamp; // Timestamp
        setColor(GREY); std::cout << "] ["; // Separator and opening log level
        setColor(LIGHT_CYAN); std::cout << logLevelStr; // Log level
        setColor(GREY); std::cout << "] "; // Closing log level

        setColor(GREY); std::cout << formattedMessage << std::endl; // Log message
    }

    // Convenience functions for specific log levels
    template<typename... Args>
    static void info(const std::string& format, Args&&... args) {
        log(LogLevel::INFO, format, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void warning(const std::string& format, Args&&... args) {
        log(LogLevel::WARNING, format, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void error(const std::string& format, Args&&... args) {
        log(LogLevel::ZERROR, format, std::forward<Args>(args)...);
    }

private:
    // Color constants
    static constexpr WORD GREY = 8;
    static constexpr WORD LIGHT_CYAN = 11;

    // Static mutex to ensure thread safety
    static std::mutex mutex_;

    // Static function to set color
    static void setColor(WORD color) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
    }

    // Static function to get the current time in hh:mm:ss format
    static std::string getCurrentTime() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm buf;
        localtime_s(&buf, &in_time_t);

        std::stringstream ss;
        ss << std::put_time(&buf, "%H:%M:%S");
        return ss.str();
    }

    // Static function to get the log level as a string
    static std::string getLogLevelString(LogLevel level) {
        switch (level) {
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ZERROR:   return "ERROR";
        default:                return "UNKNOWN";
        }
    }
};

// Initialize the static mutex
