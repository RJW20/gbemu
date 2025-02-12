#include <fstream>
#include "logger.hpp"

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