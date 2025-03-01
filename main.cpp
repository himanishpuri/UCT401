#include "include/OperatingSystemLab.h"
#include <iostream>

int main() {
    Logger::getInstance().setLogLevel(INFO);

    OperatingSystemLab osLab;

    std::vector<Process> processes1 = {
        {0, 0, 5},
        {1, 1, 3},
        {2, 2, 8},
        {3, 3, 6}
    };

    std::vector<Process> processes2 = {
        {0, 0, 7},
        {1, 2, 4},
        {2, 4, 1},
        {3, 5, 3}
    };

    LOG_INFO("Running Test Case 1 for Round Robin...");
    osLab.roundRobin(processes1, 3);

    LOG_INFO("Running Test Case 2 for Round Robin...");
    osLab.roundRobin(processes2, 2);

    return 0;
}
