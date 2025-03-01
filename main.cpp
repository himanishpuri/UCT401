#include <iostream>
#include "OperatingSystemLab.h"

int main() {
    Logger::getInstance().setLogLevel(INFO);

    OperatingSystemLab osLab;

    int P = 4, R = 3;

    std::vector<int> processes = { 0, 1, 2, 3 };
    std::vector<int> available = { 0, 0, 1 };

    std::vector<std::vector<int>> max = {
        {1, 1, 2},
        {2, 1, 0},
        {0, 1, 1},
        {0, 3, 0}
    };

    std::vector<std::vector<int>> allocation = {
        {1, 0, 1},
        {1, 1, 0},
        {0, 1, 0},
        {0, 1, 0}
    };

    LOG_INFO("Executing Banker's Algorithm...");
    osLab.bankersAlgorithm(P, R, processes, available, max, allocation);

    return 0;
}
