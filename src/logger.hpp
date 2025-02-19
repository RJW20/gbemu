#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <string>
#include <fstream>

// Overload << for all objects that have a representation method.
template <typename T>
concept HasRepresentation = requires(T t) {
    { t.representation() } -> std::same_as<std::string>;
};
template <HasRepresentation T>
std::ostream& operator<<(std::ostream& os, const T& t) {
    os << t.representation();
    return os;
}

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
 * If enabled, DEBUG messages get logged to debug.txt. All other messages are
 * logged to std::cout. */
template <LogLevel log_level>
class Logger {
public:

    // Log an ERROR message to std::cout.
    template <typename T, typename = std::enable_if_t<!std::is_invocable_v<T>>>
    static void error(const T& message) {
        if constexpr (log_level >= LogLevel::ERROR) {
            std::cout << "[ERROR]: " << message << std::endl;
        }
    }

    /* Log an ERROR message to std::cout, but only generate the message if
     * LogLevel includes ERROR messages. */
    template <
        typename Func,
        typename = std::enable_if_t<std::is_invocable_r_v<std::string, Func>>
    >
    static void error(Func&& message_generator) {
        if constexpr (log_level >= LogLevel::ERROR) {
            std::cout << "[ERROR]: " << std::forward<Func>(message_generator)()
                << std::endl;
        }
    }

    // Log a WARNING message to std::cout.
    template <typename T, typename = std::enable_if_t<!std::is_invocable_v<T>>>
    static void warning(const T& message) {
        if constexpr (log_level >= LogLevel::WARNING) {
            std::cout << "[WARNING]: " << message << std::endl;
        }
    }

    /* Log a WARNING message to std::cout, but only generate the message if
     * LogLevel includes WARNING messages. */
    template <
        typename Func,
        typename = std::enable_if_t<std::is_invocable_r_v<std::string, Func>>
    >
    static void warning(Func&& message_generator) {
        if constexpr (log_level >= LogLevel::WARNING) {
            std::cout << "[WARNING]: "
                << std::forward<Func>(message_generator)() << std::endl;
        }
    }

    // Log an INFO message to std::cout.
    template <typename T, typename = std::enable_if_t<!std::is_invocable_v<T>>>
    static void info(const T& message) {
        if constexpr (log_level >= LogLevel::INFO) {
            std::cout << "[INFO]: " << message << std::endl;
        }
    }

    /* Log an INFO message to std::cout, but only generate the message if
     * LogLevel includes INFO messages. */
    template <
        typename Func,
        typename = std::enable_if_t<std::is_invocable_r_v<std::string, Func>>
    >
    static void info(Func&& message_generator) {
        if constexpr (log_level >= LogLevel::INFO) {
            std::cout << "[INFO]: " << std::forward<Func>(message_generator)()
                << std::endl;
        }
    }

    // Log a DEBUG message to debug.txt.
    template <typename T, typename = std::enable_if_t<!std::is_invocable_v<T>>>
    static void debug(const T& message) {
        if constexpr (log_level >= LogLevel::DEBUG) {
            if (!debug_file_opened) {
                open_debug_file();
            }
            if (debug_file.is_open()) {
                debug_file << message << std::endl;
            }
        }
    }

    /* Log a DEBUG message to debug.txt, but only generate the message if
     * LogLevel includes DEBUG messages. */
    template <
        typename Func,
        typename = std::enable_if_t<std::is_invocable_r_v<std::string, Func>>
    >
    static void debug(Func&& message_generator) {
        if constexpr (log_level >= LogLevel::DEBUG) {
            if (!debug_file_opened) {
                open_debug_file();
            }
            if (debug_file.is_open()) {
                debug_file << std::forward<Func>(message_generator)()
                    << std::endl;
            }
        }
    }

private:
    static std::ofstream debug_file;
    static bool debug_file_opened;
    static void open_debug_file();
};

constexpr LogLevel log_level = LogLevel::INFO;
using Log = Logger<log_level>;

#endif