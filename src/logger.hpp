#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <cstdint>
#include <string>
#include <sstream>
#include <fstream>
#include <utility>

/* LogLevel
 * Enum containing the 5 levels of detail the Logger can output.
 * Each level includes all those less than it. */
enum class LogLevel : uint8_t {
    NONE,
    ERROR,
    WARNING,
    INFO,
    DEBUG
};

/* Logger 
 * Manages logging with a compile-time LogLevel for maximum perfomance.
 * If enabled, DEBUG messages get logged to debug.txt. All other logging is
 * outputted to std::cout. */
template <LogLevel log_level>
class Logger {
public:

    // Log an ERROR message to std::cout.
    template <typename T>
    static void error(const T& message) {
        if constexpr (log_level >= LogLevel::ERROR) {
            std::cout << create_log_message("ERROR", message);
        }
    }

    /* Log an ERROR message to std::cout, but only generate the message if
     * LogLevel
     * includes ERROR messages. */
    template <typename MessageGenerator>
    static void error(MessageGenerator&& message_generator) {
        if constexpr (log_level >= LogLevel::ERROR) {
            std::cout << create_log_message(
                "ERROR",
                std::forward<MessageGenerator>(message_generator)()
            );
        }
    }

    // Log a WARNING message to std::cout.
    template <typename T>
    static void warning(const T& message) {
        if constexpr (log_level >= LogLevel::WARNING) {
            std::cout << create_log_message("WARNING", message);
        }
    }

    /* Log a WARNING message to std::cout, but only generate the message if
     * LogLevel includes WARNING messages. */
    template <typename MessageGenerator>
    static void warning(MessageGenerator&& message_generator) {
        if constexpr (log_level >= LogLevel::WARNING) {
            std::cout << create_log_message(
                "WARNING",
                std::forward<MessageGenerator>(message_generator)()
            );
        }
    }

    // Log an INFO message to std::cout.
    template <typename T>
    static void info(const T& message) {
        if constexpr (log_level >= LogLevel::INFO) {
            std::cout << create_log_message("INFO", message);
        }
    }

    /* Log an INFO message to std::cout, but only generate the message if
     * LogLevel includes INFO messages. */
    template <typename MessageGenerator>
    void info(MessageGenerator&& message_generator) {
        if constexpr (log_level >= LogLevel::INFO) {
            std::cout << create_log_message(
                "INFO",
                std::forward<MessageGenerator>(message_generator)()
            );
        }
    }

    // Log a DEBUG message to debug.txt.
    template <typename T>
    static void debug(const T& message) {
        if constexpr (log_level >= LogLevel::DEBUG) {
            if (!debug_file_opened) {
                open_debug_file();
            }
            if (debug_file.is_open()) {
                debug_file << message << "\n";
            }
        }
    }

    /* Log a DEBUG message to debug.txt, but only generate the message if
     * LogLevel includes DEBUG messages. */
    template <typename MessageGenerator>
    static void debug(MessageGenerator&& message_generator) {
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

private:
    template <typename T>
    static std::string create_log_message(
        const std::string& level,
        const T& message
    );

    static std::ofstream debug_file;
    static bool debug_file_opened;
    static void open_debug_file();
};

constexpr LogLevel log_level = LogLevel::WARNING;
using Log = Logger<log_level>;

#endif