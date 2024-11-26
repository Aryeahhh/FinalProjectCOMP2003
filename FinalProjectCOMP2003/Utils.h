#ifndef UTILS_H
#define UTILS_H

#include <string>

namespace Utils {
    std::string generateFilename(const std::string& prefix);
    void validateInput(int& input, int min, int max, const std::string& prompt);
    void validateCharInput(char& input, const std::string& prompt);
}

#endif
