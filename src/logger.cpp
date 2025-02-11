#include <iostream>
#include <string>
#include <utility>
#include <fstream>
#include "Logger.hpp"

// Log an ERROR message to std::cout.
template <LogLevel log_level>
void Logger<log_level>::error(const std::string& message) {
    if constexpr (log_level >= LogLevel::ERROR) {
        std::cout << create_log_message("ERROR", message);
    }
}

// Log a WANRING message to std::cout.
template <LogLevel log_level>
void Logger<log_level>::warning(const std::string& message) {
    if constexpr (log_level >= LogLevel::WARNING) {
        std::cout << create_log_message("WARNING", message);
    }
}

// Log an INFO message to std::cout.
template <LogLevel log_level>
void Logger<log_level>::info(const std::string& message) {
    if constexpr (log_level >= LogLevel::INFO) {
        std::cout << create_log_message("INFO", message);
    }
}

// Generate the log message for the given level and message.
template <LogLevel log_level>
void Logger<log_level>::create_log_message(
    const std::string& level,
    const std::string& message
) {
    return std::string("[" + level + "] " + message + "\n");
}


// Log a DEBUG message to debug.txt.
template <LogLevel log_level>
void Logger<log_level>::debug(const std::string& message) {
    if constexpr (log_level >= LogLevel::DEBUG) {
        if (!debug_file_opened) {
            open_debug_file();
        }
        if (debug_file.is_open()) {
            debug_file << message << "\n";
        }
    }
}

/* Log a DEBUG message to debug.txt, but only generate the message if LogLevel
 * includes DEBUG messages. */
template <LogLevel log_level>
template <typename MessageGenerator>
void Logger<log_level>::debug(MessageGenerator&& message_generator) {
    if constexpr (log_level >= LogLevel::DEBUG) {
        if (!debug_file_opened) {
            open_debug_file();
        }
        if (debug_file.is_open()) {
            debug_file <<
                std::forward<MessageGenerator>(message_generator)() << "\n";
        }
    }
}

template <LogLevel Level>
std::ofstream Logger<Level>::debug_file;

template <LogLevel Level>
bool Logger<Level>::debug_file_opened = false;

/* Open debug.txt for logging DEBUG messages. */
template <LogLevel log_level>
void Logger<log_level>::open_debug_file() {
    if constexpr (log_level >= LogLevel::DEBUG) {
        debug_file.open("debug.txt", std::ios::trunc);
        debug_file_opened = true;
    }
}

template class Logger<LogLevel::NONE>;
template class Logger<LogLevel::ERROR>;
template class Logger<LogLevel::WARNING>;
template class Logger<LogLevel::INFO>;
template class Logger<LogLevel::DEBUG>;