#include "Utils.h"
#include <ctime>
#include <sstream>
#include <iostream>
#include <limits>

namespace Utils {
    std::string generateFilename(const std::string& prefix) {
        std::ostringstream oss;
        time_t now = time(0);
        tm localTime;
        localtime_s(&localTime, &now); // Secure version of localtime
        oss << prefix << "_"
            << 1900 + localTime.tm_year << "-"
            << 1 + localTime.tm_mon << "-"
            << localTime.tm_mday << "_"
            << localTime.tm_hour << localTime.tm_min << localTime.tm_sec << ".txt";
        return oss.str();
    }

    void validateInput(int& input, int min, int max, const std::string& prompt) {
        while (!(std::cin >> input) || input < min || input > max) {
            std::cout << "Invalid input. " << prompt;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    void validateCharInput(char& input, const std::string& prompt) {
        while (true) {
            std::cin >> input;
            if (input == 'y' || input == 'n' || input == 'Y' || input == 'N') {
                break;
            }
            std::cout << "Invalid input. " << prompt;
        }
    }
}
