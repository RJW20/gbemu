#include <string>
#include <sstream>
#include "exceptions.hpp"

// Generate the MemoryAccessException warning message.
std::string MemoryAccessException::message() const {
    std::ostringstream msg;
    msg << "MemoryAccessException: Invalid ";
    if (read) {
        msg << "read from ";
    }
    else {
        msg << "write to ";
    }
    msg << location << " at address " << std::hex << address << " - " << reason
        << ".";
    return msg.str();
}