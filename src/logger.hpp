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
    static void error(const std::string& message);
    static void warning(const std::string& message);
    static void info(const std::string& message);
    static void debug(const std::string& message);

private:
    static void create_log_message(
        const std::string& level,
        const std::string& message
    );

    static std::ofstream debug_file;
    static bool debug_file_opened;
    static void open_debug_file();
};

constexpr LogLevel log_level = LogLevel::WARNING;
using Log = Logger<log_level>;

#endif