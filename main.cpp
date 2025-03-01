#include "include/OperatingSystemLab.h"

int main() {
    OperatingSystemLab osLab;

    Logger::getInstance().setLogLevel(INFO);

    LOG_INFO("Starting FCFS test case...");

    std::vector<Process> processes = {
        {0, 0, 6},  // PID 0, Arrival 0, Burst 6
        {1, 2, 8},  // PID 1, Arrival 2, Burst 8
        {2, 4, 7},  // PID 2, Arrival 4, Burst 7
        {3, 6, 3}   // PID 3, Arrival 6, Burst 3
    };

    osLab.firstComeFirstServe(processes);

    LOG_INFO("FCFS test case completed.");

    return 0;
}
