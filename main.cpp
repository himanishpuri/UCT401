#include "include/OperatingSystemLab.h"

int main() {
    OperatingSystemLab osLab;

    Logger::getInstance().setLogLevel(DEBUG);

    LOG_INFO("Starting Priority Scheduling test case...");

    std::vector<Process> processes = {
        {0, 0, 6},
        {1, 2, 3},
        {2, 4, 7},
    };

    osLab.shortestRemainingTimeFirst(processes);

    LOG_INFO("Priority Scheduling test case completed.");

    return 0;
}
