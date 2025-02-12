#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <cstdint>
#include <string>
#include <fstream>

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
    template <typename T>
    static void error(const T& message);
    template <typename MessageGenerator>
    static void error(MessageGenerator&& message_generator);

    template <typename T>
    static void warning(const T& message);
    template <typename MessageGenerator>
    static void warning(MessageGenerator&& message_generator);

    template <typename T>
    static void info(const T& message);
    template <typename MessageGenerator>
    static void info(MessageGenerator&& message_generator);

    template <typename T>
    static void debug(const T& message);
    template <typename MessageGenerator>
    static void debug(MessageGenerator&& message_generator);

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