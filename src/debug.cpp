#include "debug.h"

void debugMsg(std::string source, std::string error) {
    std::cout << "[DEBUG] " << source << ": " << error << std::endl;
}