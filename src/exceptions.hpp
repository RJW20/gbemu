#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <cstdint>
#include <string>

/* Memory Access Exception 
 * Exception for failed memory reads and writes for all components. */
class MemoryAccessException : public std::runtime_error {
public:
    MemoryAccessException(
        const std::string& location,
        const std::string& reason,
        const uint16_t address,
        const bool read
    ) : std::runtime_error(build_message(location, reason, address, read)) {}

private:
    std::string build_message(
        const std::string& location,
        const std::string& reason,
        const uint16_t address,
        const bool read
    ) const;
};

#endif