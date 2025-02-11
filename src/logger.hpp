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
 * Manages logging with a compile-time LogLevel for maximum perfomance. */
template <LogLevel log_level>
class Logger {
public:
    static void set_log_file(const std::string& file_name);

    static void error(const std::string& message);
    static void warning(const std::string& message);
    static void info(const std::string& message);
    static void debug(const std::string& message);

private:
    static void log(const std::string& level, const std::string& message);

    static inline std::ofstream log_file;
};

constexpr LogLevel log_level = LogLevel::WARNING;
using Log = Logger<log_level>;

#endif