#include <string>
#include <sstream>
#include "exceptions.hpp"

// Generate the MemoryAccessException error message.
std::string MemoryAccessException::message() const {
    std::ostringstream msg;
    std::string read_write = read ? "read" : "write";
    msg << "Invalid " << location << " " << read_write << " at address "
        << std::hex << static_cast<int>(address) << " - " << reason;
    return msg.str();

}