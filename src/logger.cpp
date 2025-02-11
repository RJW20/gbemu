#include <iostream>
#include <string>
#include "Logger.hpp"

/* Set the file to output the logs to.
 * Closes the current log_file if needed. */
template <LogLevel log_level>
void Logger<log_level>::set_log_file(const std::string& file_name) {
    if (log_file.is_open()) {
        log_file.close();
    }
    log_file.open(file_name, std::ios::app);
}

// Log an ERROR message.
template <LogLevel log_level>
void Logger<log_level>::error(const std::string& message) {
    if constexpr (log_level >= LogLevel::ERROR) {
        log("ERROR", message);
    }
}

// Log a WANRING message.
template <LogLevel log_level>
void Logger<log_level>::warning(const std::string& message) {
    if constexpr (log_level >= LogLevel::WARNING) {
        log("WARNING", message);
    }
}

// Log an INFO message.
template <LogLevel log_level>
void Logger<log_level>::info(const std::string& message) {
    if constexpr (log_level >= LogLevel::INFO) {
        log("INFO", message);
    }
}

// Log a DEBUG message.
template <LogLevel log_level>
void Logger<log_level>::debug(const std::string& message) {
    if constexpr (log_level >= LogLevel::DEBUG) {
        log("DEBUG", message);
    }
}

// Log the given message and its level to std::cout and to file_stream if set.
template <LogLevel log_level>
void Logger<log_level>::log(
    const std::string& level,
    const std::string& message
) {
    std::string output = "[" + level + "] " + message + "\n";
    std::cout << output;
    if (file_stream.is_open()) {
        file_stream << output;
    }
}

template class Logger<LogLevel::NONE>;
template class Logger<LogLevel::ERROR>;
template class Logger<LogLevel::WARNING>;
template class Logger<LogLevel::INFO>;
template class Logger<LogLevel::DEBUG>;