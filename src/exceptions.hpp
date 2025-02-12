#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>
#include <cstdint>
#include <string>

/* Memory Access Exception 
 * Exception for failed memory reads and writes for all components. */
class MemoryAccessException : public std::exception {
public:
    MemoryAccessException(
        const std::string& location,
        const bool read,
        const uint8_t address,
        const std::string& reason
    ) : location(location), read(read), address(address), reason(reason) {}

    const char* what() const noexcept override { return message().c_str(); }

private:
    const std::string location;
    const bool read;
    const uint8_t address;
    const std::string reason;

    std::string message() const;
};

#endif