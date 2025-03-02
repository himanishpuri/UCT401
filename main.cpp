#include "include/OperatingSystemLab.h"

int main() {
    OperatingSystemLab osLab;

    Logger::getInstance().setLogLevel(INFO);

    LOG_INFO("Starting Priority Scheduling test case...");

    std::vector<PProcess> processes = {
        {0, 0, 6, 1},  // PID 0, Arrival 0, Burst 6, Priority 2
        {1, 2, 3, 2},  // PID 1, Arrival 2, Burst 8, Priority 1
        {2, 4, 7, 4},  // PID 2, Arrival 4, Burst 7, Priority 3
        {3, 6, 2, 3}   // PID 3, Arrival 6, Burst 3, Priority 4
    };

    osLab.priorityScheduling(processes);

    LOG_INFO("Priority Scheduling test case completed.");

    return 0;
}
