#pragma once
#include <string>
#include <fstream>
#include <mutex>
#include <iostream>
#include <include/UI/console_message.h>
#include <include/base/Event.h>

class Logger {
private:
    static inline std::ofstream log_file{ "../../../log.txt", std::ios::trunc };
    static inline std::mutex log_mutex;

    // Private constructor to prevent instantiation
    Logger() = default;

public:
    // Ensure log file is always initialized
    static void Init() {
        std::lock_guard<std::mutex> lock(log_mutex);
        if (!log_file.is_open()) {
            log_file.open("../../../log.txt", std::ios::trunc);
        }
    }

    static void Write(const std::string& content) {
        std::lock_guard<std::mutex> lock(log_mutex);
        if (log_file.is_open()) {
            log_file << content << std::endl;
            log_file.flush();
        }
        else {
            std::cerr << "Error: Log file is not open!" << std::endl;
        }
    }

    friend void DevLog(const std::string& content, console_color color);
    friend void DevMsg(const std::string& content, console_color color);
};

// Log message to console, logging to a file as you go
void DevLog(const std::string& content, console_color color = console_color::WHITE) {
    QKEvent::fire<console_message>(content, color);
    Logger::Write(content);
}

// Simply write a message to the console
void DevMsg(const std::string& content, console_color color = console_color::WHITE) {
    QKEvent::fire<console_message>(content, color);
}
