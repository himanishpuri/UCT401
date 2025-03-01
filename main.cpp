#include "include/OperatingSystemLab.h"
#include <iostream>

int main() {
    OperatingSystemLab osLab;

    std::vector<Process> processes1 = { {0, 0, 6}, {1, 2, 8}, {2, 4, 7}, {3, 6, 3} };
    osLab.shortestJobFirst(processes1);

    std::vector<Process> processes2 = { {0, 1, 4}, {1, 2, 2}, {2, 3, 3}, {3, 4, 5} };
    osLab.shortestJobFirst(processes2);

    return 0;
}
